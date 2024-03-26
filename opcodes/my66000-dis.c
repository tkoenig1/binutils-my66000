/* Disassemble My 66000 instructions.
   Copyright (C) 2023 Free Software Foundation, Inc.
   Contributed by Thomas König.

   Based on the mrisc32 disassembler.

   This file is part of the GNU opcodes library.

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   It is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#include "sysdep.h"
#include <stdio.h>
#include <assert.h>
#include "safe-ctype.h"

#define STATIC_TABLE
#define DEFINE_TABLE

#include "disassemble.h"
#include "opcode/my66000.h"

static fprintf_ftype fpr;
static void *stream;

static const char *carry_modifier[] = {"-","I","O","IO"};

static int start_carry, end_carry;
static uint32_t carry_mod[8];

static void
print_modifier_list (uint32_t io8)
{
  uint32_t mod;

  start_carry = 0;
  while (io8 > 0)
    {
      mod = io8 & 3;
      fpr (stream, "%s", carry_modifier[mod]);
      carry_mod[end_carry] = mod;
      io8 = io8 >> 2;
      if (io8 > 0)
	fpr (stream, ",");
      end_carry ++;
    }
}

static void
print_ins (uint32_t ins)
{
  int32_t v1, v2;
  v1 = (ins >> 6) & 0x3f;
  v1 = v1 == 0 ? 64 : v1;
  v2 = ins & 0x3f;
  v2 = v2 == 0 ? 64 : v2;
  fpr (stream, "%d:%d", v1, v2);
}

/* Sign-extend an n-bit value, for offsets.  */
static int64_t
sign_extend (uint64_t a, int32_t bit)
{
  if (a & ((uint64_t) 1 << (bit - 1)))
    return (((uint64_t) -1) << (bit - 1) ) | a;
  else
    return a;
}

static void
print_vec (uint32_t ins, bool use_vec)
{
  const char **regs;
  int n;
  regs = use_vec ? my66000_vec_reg : my66000_rname;
  n = use_vec ? MY66000_VEC_BITS : 32;

  for (int i = 0; i < n; i++)
    {
      if (ins & 1)
	{
	  fpr (stream, "%s", regs[i]);
	  ins = ins >> 1;
	  if (ins != 0)
	    fpr (stream, ",");
	}
      else
	ins = ins >> 1;
    }
}

/* Print out a true/false list for predicates.  */

static void
print_tf (uint32_t v)
{
  int n_then = v & 63;
  int n_else = v >> 6;

  for (int i=0; i<n_then; i++)
    fpr (stream, "%c", 'T');

  for (int i=0; i<n_else; i++)
    fpr (stream, "%c", 'F');

}

/* Get the first vaild format string for the iword, return it
   or NULL on failure.  */

static char *
get_fmt (uint32_t iword, my66000_opc_info_t const *opc)
{
  const my66000_opcode_fmt_t *opcode_fmt;
  const my66000_fmt_spec_t *spec;
  uint32_t res;
  opcode_fmt = &my66000_opcode_fmt[opc->enc];
  spec = opcode_fmt->spec;

  if (spec == NULL)
    return NULL;

  while (spec)
    {
      res = (spec->patt ^ iword) & spec->mask;
      if (res == 0)
	break;

      spec ++;
    }
  if (res != 0)
    return NULL;

  return spec->fmt;
}


static int
print_operands (uint32_t iword, const char *fmt, bfd_vma addr,
		disassemble_info *info, const char *name)
{
  int status;
  const my66000_operand_info_t *op_info;
  const char *f;
  uint32_t size_1, size_2;
  bfd_byte buf1[8], buf2[8];
  int32_t val_32;
  int64_t val_64;

  /* Check for nothing to do.  */
  if (*fmt == '\0')
    return 0;

  fpr (stream, "%c",'\t');

  size_1 = size_2 = 0;
  for (f = fmt; *f; f++)
    {
      if (ISALPHA (*f)) {
	op_info = &my66000_operand_table[*f - 'A'];
	if (op_info->seq == 1)
	  {
	    if (size_1 != 0)
	      {
		opcodes_error_handler ("Internal error: Duplicate seq 1 for %c",
				       *f);
		exit(EXIT_FAILURE);
	      }
	    size_1 = op_info->size;

	  }
	else if (op_info->seq == 2)
	  {
	    if (size_2 != 0)
	      {
		opcodes_error_handler ("Inernal error: Duplicate seq 2 for %c",
				       *f);
		exit(EXIT_FAILURE);
	      }
	    size_2 = op_info->size;
	  }
      }
    }

  if (size_1 > 0)
    {
      status = info->read_memory_func (addr + 4, buf1, size_1, info);
      if (status != 0)
	goto fail;
    }
  if (size_2 > 0)
    {
      status = info->read_memory_func (addr + 4 + size_1, buf2,
				       size_2, info);
      if (status != 0)
	goto fail;
    }
  for (f = fmt; *f; f++)
    {
      if (ISALPHA(*f))
      {
	uint32_t val;
	int16_t v;;
	const char *out_fmt;
	bfd_byte *buf;

	op_info = &my66000_operand_table[*f - 'A'];
	val = (iword & op_info->mask) >> op_info->shift;

	switch (op_info->size)
	  {
	  case 0:
	    break;
	  case 4:
	    buf = op_info->seq == 1 ? buf1 : buf2;
	    val_32 = bfd_getl32 (buf);
	    if (op_info->oper == MY66000_OPS_INS)
	      {
		print_ins (val_32);
	      }
	    else if (op_info->oper == MY66000_OPS_VEC32)
	      {
		print_vec (val_32, false);
	      }
	    else
	      {
		out_fmt = op_info->oper == MY66000_OPS_I32_HEX ? "0x%8.8x" : "%d";
		fpr (stream, out_fmt, val_32);
	      }

	    continue;
	  case 8:
	    buf = op_info->seq == 1 ? buf1 : buf2;
	    val_64 = bfd_getl64 (buf);
	    out_fmt = op_info->oper == MY66000_OPS_I64_HEX ? "0x%16.16lx" : "%ld";
	    fpr (stream, out_fmt, val_64);
	    continue;
	  default:
	    opcodes_error_handler ("Internal error: size");
	    exit (EXIT_FAILURE);
	  }

	switch (op_info->oper)
	  {
	  case MY66000_OPS_DST:
	  case MY66000_OPS_SRC1:
	  case MY66000_OPS_SRC2:
	  case MY66000_OPS_SRC3:
	    /* A register in normal notation.  */
	    fpr (stream, "%s", my66000_rname[val]);
	    break;

	  case MY66000_OPS_RINDEX:
	    /* An index register.  */
	    fpr (stream, "%s", my66000_rind[val]);
	    break;

	  case MY66000_OPS_RBASE:
	    /* Base register.  */
	    fpr (stream, "%s", my66000_rbase[val]);
	    break;

	  case MY66000_OPS_HRFCN:
	    fpr (stream, "%s", my66000_hr_fcn[val]);
	    break;

	  case MY66000_OPS_LOOP_U:
	    fpr (stream, "%s", my66000_loop_u[val]);
	    break;

	  case MY66000_OPS_LOOP_S:
	    fpr (stream, "%s", my66000_loop_s[val]);
	    break;

	  case MY66000_OPS_SI5:
	    v = sign_extend (val, 5);
	    fpr (stream, "%d", v);
	    break;

	  case MY66000_OPS_IMM13:
	    /* Special case, the lower-order bits are used as flags.  */
	    val &= ~0x7;
	    /* Fallthrough */

	  case MY66000_OPS_IMM16:
	  case MY66000_OPS_BB1:
	  case MY66000_OPS_WIDTH:
	  case MY66000_OPS_OFFSET:
	  case MY66000_OPS_FL_ENTER:
	  case MY66000_OPS_MSCALE:
	    /* An integer constant.  */
	    v = val;
	    fpr (stream, "%d", v);
	    break;

	  case MY66000_OPS_TF:
	    print_tf(val);
	    break;

	  case MY66000_OPS_I1:
	  case MY66000_OPS_I2:
	  case MY66000_OPS_I3:
	  case MY66000_OPS_UIMM16:
	    fpr (stream, "%u", val);
	    break;

	  case MY66000_OPS_W_BITR:
	    /* Six-bit contsant, has to be a power of two.  */
	    v = val;
	    switch (v)
	      {
	      case 1:
	      case 2:
	      case 4:
	      case 8:
	      case 16:
	      case 32:
		fpr (stream, "%d", v);
		break;
	      default:
		return -1;
	      }
	    break;
	  case MY66000_OPS_VEC:
	    print_vec (val, true);
	    break;

	    /* An IP-relative offset.  */
	  case MY66000_OPS_B16:
	    (*info->print_address_func) ((bfd_vma) (addr + (sign_extend(val,16) << 2)), info);
	    break;
	  case MY66000_OPS_B26:
	    (*info->print_address_func) ((bfd_vma) (addr + (sign_extend(val,26) << 2)), info);
	    break;

	  case MY66000_OPS_CARRY:
	    print_modifier_list (val);
	    break;

	  default:
	    opcodes_error_handler ("Internal error: Unhandled format '%c' (%s) for %s",
				   *f, op_info->desc, name);
	    exit (EXIT_FAILURE);
	  }
      }
    else
      fpr (stream, "%c", *f);

  }
  return size_1 + size_2;
 fail:
 info->memory_error_func (status, addr, info);
 return -1;
}

/* Read a sign-extended constant of size n=1,2,4,8 from the input stream.  */

static int64_t
read_signed_constant (bfd_vma addr, int size, struct disassemble_info *info)
{
  bfd_byte buffer[8];
  int64_t ret = 0;
  int status;
  status = info->read_memory_func (addr, buffer, size, info);
  if (status)
    {
      info->memory_error_func (status, addr, info);
      return 0;
    }
  switch (size)
    {
    case 1:
      ret = (signed char) buffer[0];
      break;
    case 2:
      ret = bfd_getl_signed_16 (buffer);
      break;
    case 4:
      ret = bfd_getl_signed_32 (buffer);
      break;
    case 8:
      ret = bfd_getl_signed_64 (buffer);
      break;
    }
  return ret;
}

static int jt_size;
int jt_needed;
int jt_fill;
bfd_vma jt_addr;

/* Print a jump table.  This one is so special that it uses straightforward
   code instad of being table-driven.  */

static void
print_jt_entry (bfd_vma addr, struct disassemble_info *info)
{
  int64_t offs;
  info->bytes_per_chunk = jt_size;
  offs = read_signed_constant (addr, jt_size, info);
  (*info->print_address_func) ((bfd_vma) (jt_addr + (offs << 2)), info);
}

/* Print a comment for carry.  */

static void
comment_carry(void)
{

  if (carry_mod[start_carry] != 0)
    fpr (stream,"\t; {%s}", carry_modifier[carry_mod[start_carry]]);
}

/* We put found opcodes on a mini-stack because we prefer to print
   instructions which come lower, but the format strings don't always
   match.  Put up to DEPTH_MAX opcoddes on the stack and complain with
   an internal error if it overflows.  */

#define DEPTH_MAX 4

int
print_insn_my66000 (bfd_vma addr, struct disassemble_info *info)
{
  int status;
  int length, o_length;
  bfd_byte buffer[4];
  uint32_t iword, opcode;
  my66000_opc_info_t const *p, *tab, *found;
  uint32_t shift, mask;
  my66000_opc_info_t const *fnd[DEPTH_MAX];
  int depth;
  const char *fmt;

  fpr = info->fprintf_func;
  stream = info->stream;

  if (jt_needed > 0)
    {
      print_jt_entry (addr, info);
      jt_needed --;
      return jt_size;
    }
  else if (jt_fill > 0)
    {
      jt_fill --;
      return jt_size;
    }

  if ((status = info->read_memory_func (addr, buffer, 4, info)))
    goto fail;

  iword = (uint32_t) bfd_getl32 (buffer);
  info->bytes_per_chunk = 4;

  if (my66000_is_tt (iword))
    {
      jt_needed = (iword & 0xffff) + 1;
      jt_size = my66000_get_tt_size (iword);
      jt_addr = addr;
      jt_fill = (4 - jt_size * jt_needed) % 4;
    }
  else
    jt_needed = jt_fill = 0;

  length = 4;
  shift = MY66000_MAJOR_SHIFT;
  mask = MY66000_MAJOR_MASK;
  tab = &my66000_opc_info[0];
  found = NULL;
  o_length = 0;
  depth = 0;
  do
    {
      opcode = (iword & mask) >> shift;
      //      fprintf (stderr,"mask = %8.8x, shift = %2d, mask >> shift = %8.8x, opcode=%2d\n", mask, shift, mask>>shift,
      //	       opcode);
      p = &tab[opcode];
      if (p->enc == MY66000_END)
	break;

      if (p->name)
	{
	  if (depth >= 4)
	    {
	      opcodes_error_handler ("Internal error: depth too low");
	      exit(EXIT_FAILURE);
	    }
	  fnd[depth] = p;
	  depth ++;
	}

      tab = p->sub;
      mask = p->patt_mask;
      shift = p->shift;
    } while(tab);

  fmt = NULL;
  for (int i=depth-1; i>=0; i--)
    {
      found = fnd[i];
      fmt = get_fmt (iword, found);
      if (fmt != NULL)
	break;
    }
  if (fmt)
    {
      if (found->enc == MY66000_CARRY)
	{
	  if (end_carry > 0)
	    {
	      opcodes_error_handler ("Overlapping carry not permitted");
	      return -1;
	    }
	}
      fpr (stream, "%s", found->name);
      //      fprintf (stderr,"%s\n", get_fmt (iword, found));
      o_length = print_operands (iword, fmt, addr, info, found->name);
      if (o_length < 0)
	goto error;
      if (found->enc != MY66000_CARRY && end_carry > 0)
	{
	  comment_carry ();
	  start_carry ++;
	  if (start_carry == end_carry)
	    {
	      start_carry = 0;
	      end_carry = 0;
	    }
	}
    }
  else
    goto error;

  return o_length + length;

 error:
  fpr (stream, ".word\t%8.8x", iword);
  fprintf (stderr, "Unrecognized instruction %8.8x\n", iword);
  return length;

 fail:
  info->memory_error_func (status, addr, info);
  return -1;
}
