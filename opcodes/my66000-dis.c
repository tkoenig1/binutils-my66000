/* Disassemble My66000 instructions.
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

#define STATIC_TABLE
#define DEFINE_TABLE

#include "disassemble.h"
#include "opcode/my66000.h"

static fprintf_ftype fpr;
static void *stream;

int
print_insn_my66000 (bfd_vma addr, struct disassemble_info *info)
{
  int status;
  int length;
  bfd_byte buffer[4];
  unsigned int opcode;
  uint32_t iword;
  fpr = info->fprintf_func;
  stream = info->stream;

  if ((status = info->read_memory_func (addr, buffer, 4, info)))
    goto fail;

  length = 4;
  iword = (uint32_t) bfd_getl32 (buffer);
  opcode = iword >> 26;

  fpr (stream, "%s", my66000_opc_info[opcode].name);

  return length;

 fail:
  info->memory_error_func (status, addr, info);
  return -1;
}
