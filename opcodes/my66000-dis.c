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

/* Sign-extend an n-bit value, for offsets.  */
static int64_t
sign_extend (uint64_t a, int32_t bit)
{
  if (a & ((uint64_t) 1 << (bit - 1)))
    return (((uint64_t) -1) << (bit - 1) ) | a;
  else
    return a;
}

static int
print_operands (uint32_t iword, my66000_opc_info_t const *opc, bfd_vma addr,
		disassemble_info *info)
{
  my66000_encoding enc = opc->enc;
  int status;
  const my66000_opcode_fmt_t *opcode_fmt;
  const my66000_operand_info_t *op_info;
  const my66000_fmt_spec_t *spec;
  const char *f;
  uint32_t res;
  uint32_t size_1, size_2;
  bfd_byte buf1[8], buf2[8];
  uint32_t val_32;
  uint64_t val_64;

  opcode_fmt = &my66000_opcode_fmt[enc];
  spec = opcode_fmt->spec;
  if (spec == NULL)
    return 0;

  /* Loop over the table of formats until a match is found.  FIXME:
     This could be made more elegant, and probably faster.  */

  while (spec)
    {
      res = (spec->patt ^ iword) & spec->mask;
      if (res == 0)
	break;

      spec ++;
    }
  if (res != 0)
    return 0;

  if (spec->fmt == NULL)
    {
      opcodes_error_handler ("Internal error: empty format string for %s",
			     opc->name);
      exit (EXIT_FAILURE);
    }

  /* Look at immediates for the instructions in the format string
     and mark them.  */

  size_1 = size_2 = 0;
  for (f = spec->fmt; *f; f++)
    {
      if (ISUPPER (*f)) {
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
  for (f = spec->fmt; *f; f++)
    {
      if (ISUPPER(*f))
      {
	uint32_t val;
	int16_t v;;

	op_info = &my66000_operand_table[*f - 'A'];
	val = (iword & op_info->mask) >> op_info->shift;

	switch (op_info->size)
	  {
	  case 0:
	    break;
	  case 4:
	    val_32 = bfd_getl32 (buf1);
	    fpr (stream, "%u", val_32);
	    continue;
	  case 8:
	    val_64 = bfd_getl64 (buf1);
	    fpr (stream, "%lu", val_64);
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

	  case MY66000_OPS_IMM16:
	  case MY66000_OPS_I1:
	  case MY66000_OPS_I2:
	  case MY66000_OPS_BB1:
	    /* An integer constant.  */
	    v = val;
	    fpr (stream, "%d", v);
	    break;
	    /* An IP-relative offset.  */
	  case MY66000_OPS_B16:
	    (*info->print_address_func) ((bfd_vma) (addr + (sign_extend(val,16) << 2)), info);
	    break;
	  case MY66000_OPS_B26:
	    (*info->print_address_func) ((bfd_vma) (addr + (sign_extend(val,26) << 2)), info);
	    break;

	  default:
	    opcodes_error_handler ("Internal error: Unhandled format '%c' for %s",
				   *f, opc->name);
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

int
print_insn_my66000 (bfd_vma addr, struct disassemble_info *info)
{
  int status;
  int length, o_length;
  bfd_byte buffer[4];
  uint32_t iword, opcode;
  my66000_opc_info_t const *p, *tab, *found;
  uint32_t shift, mask;

  fpr = info->fprintf_func;
  stream = info->stream;

  if ((status = info->read_memory_func (addr, buffer, 4, info)))
    goto fail;

  info->bytes_per_chunk = 4;

  length = 4;
  iword = (uint32_t) bfd_getl32 (buffer);
  shift = MY66000_MAJOR_SHIFT;
  mask = MY66000_MAJOR_MASK;
  tab = &my66000_opc_info[0];
  found = NULL;
  o_length = 0;
  do
    {
      opcode = (iword & mask) >> shift;
      p = &tab[opcode];
      if (p->enc == MY66000_END)
	break;

      if (p->name)
	  found = p;

      tab = p->sub;
      mask = p->frag_mask;
      shift = p->shift;
    } while(tab);

  if (found)
    {
      fpr (stream, "%s\t", found->name);
      o_length = print_operands (iword, found, addr, info);
      if (o_length < 0)
	goto fail;
    }
  else
    opcodes_error_handler ("Error: unknown opcode %8.8x", iword);

  return o_length + length;

 fail:
  info->memory_error_func (status, addr, info);
  return -1;
}
