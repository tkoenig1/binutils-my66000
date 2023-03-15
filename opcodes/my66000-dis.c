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

static void
print_operands (uint32_t iword, my66000_opc_info_t const *opc, bfd_vma addr)
{
  my66000_encoding enc = opc->enc;
  const my66000_opcode_fmt_t *opcode_fmt;
  const my66000_operand_info_t *op_info;
  const my66000_fmt_spec_t *spec;
  const char *f;
  uint32_t res;

  opcode_fmt = &my66000_opcode_fmt[enc];
  spec = opcode_fmt->spec;
  if (spec == NULL)
    return;

  /* Loop over the table of formats until a match is found.  FIXME:
     This could be made more elegant, and probably faster.  */

  while (spec)
    {
      res = (spec->frag ^ iword) & spec->mask;
      if (res == 0)
	break;

      spec ++;
    }
  if (res != 0)
      return;

  for (f = spec->fmt; *f; f++)
    {
      if (ISUPPER(*f))
	{
	  uint32_t val;
	  int16_t v;;

	  op_info = &my66000_operand_table[*f - 'A'];
	  val = (iword & op_info->mask) >> op_info->shift;
	  switch (op_info->oper)
	    {
	    case MY66000_OPS_DST:
	    case MY66000_OPS_SRC1:
	    case MY66000_OPS_SRC2:
	      /* A register in normal notation.  */
	      fpr (stream, "%s", my66000_rname[val]);
	      break;

	    case MY66000_OPS_RINDEX:
	      /* An index register.  */
	      fpr (stream, "%s", my66000_rind[val]);
	      break;

	    case MY66000_OPS_IMM16:
	    case MY66000_OPS_I1:
	    case MY66000_OPS_I2:
	    case MY66000_OPS_BB1:
	      /* An integer constant.  */
	      v = val;
	      fpr (stream, "%d", v);
	      break;
	    case MY66000_OPS_B16:
	    case MY66000_OPS_B26:
	      fpr (stream, "0x%lx", (unsigned long) addr + (val << 2));
	      break;
	    default:
	      fprintf (stderr,"Increased unhappiness\n");
	    }
	}
      else
	fpr (stream, "%c", *f);
    }
}

int
print_insn_my66000 (bfd_vma addr, struct disassemble_info *info)
{
  int status;
  int length;
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
      print_operands (iword, found, addr);
    }
  else
    opcodes_error_handler ("Error: unknown opcode %8.8x", iword);

  return length;

 fail:
  info->memory_error_func (status, addr, info);
  return -1;
}
