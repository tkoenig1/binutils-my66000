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

#define STATIC_TABLE
#define DEFINE_TABLE

#include "disassemble.h"
#include "opcode/my66000.h"

static fprintf_ftype fpr;
static void *stream;

inline static int dst (uint32_t word)
{
  return (word >> 21) & 0x1f;
}

inline static int
src1 (uint32_t word)
{
  return (word >> 16) & 0x1f;
}

inline static int
immed16 (uint32_t word)
{
  int16_t a = word;
  return a;
}

static void
print_operands (uint32_t iword, my66000_opc_info_t const *opc)
{
  switch (opc->enc)
    {
    case MY66000_MEM:
      fpr (stream,"%s,[%s,%d]", my66000_rname[dst(iword)], my66000_rname[src1(iword)],
	   immed16(iword));
      break;
    case MY66000_OPIMM:
      fpr (stream,"%s,%s,#%d", my66000_rname[dst(iword)], my66000_rname[src1(iword)],
	   immed16(iword));
      break;
    default:
      break;
    }
}

/* Get the minor opcode from the instruction.  The position varies a
   bit.  */

static int
get_minor_opcode (uint32_t iword, my66000_encoding enc)
{
  switch (enc)
    {
    case MY66000_OM6:
    case MY66000_OM7:
      return (iword >> 12) & 0xf;
      break;
      
    default:
      return -1;
    }
}

int
print_insn_my66000 (bfd_vma addr, struct disassemble_info *info)
{
  int status;
  int length;
  bfd_byte buffer[4];
  int opcode, minor_opcode;
  uint32_t iword;
  my66000_opc_info_t const *primary, *secondary, *opc;

  fpr = info->fprintf_func;
  stream = info->stream;

  if ((status = info->read_memory_func (addr, buffer, 4, info)))
    goto fail;

  length = 4;
  iword = (uint32_t) bfd_getl32 (buffer);
  opcode = iword >> 26;

  primary = &my66000_opc_info[opcode];

  /* Some opcodes can be disassembled to a specific version, or a general
     version.  Use the specific version if available.  */

  if (primary->sub)
    {
      minor_opcode = get_minor_opcode (iword, primary->enc);
      assert (minor_opcode != -1);
      secondary = &primary->sub[minor_opcode];
      if (secondary->name == NULL)
	secondary = NULL;
    }
  else
    secondary = NULL;

  if (secondary)
    {
      assert (secondary->name);
      fpr (stream, "%s\t", secondary->name);
    }
  else
    fpr (stream, "%s\t", primary->name);

  opc = secondary ? secondary : primary;
  print_operands (iword, opc);
  return length;

 fail:
  info->memory_error_func (status, addr, info);
  return -1;
}
