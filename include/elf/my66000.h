/* My 66000 ELF support for BFD.
   Copyright (C) 2003-2023 Free Software Foundation, Inc.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#ifndef _ELF_MY66000_H
#define _ELF_MY66000_H

#include "elf/reloc-macros.h"

/* Relocations.  Take care to keep this in sync with
   my66000_elf_howto_table in elf64-my66000.c. */
START_RELOC_NUMBERS (elf_my66000_reloc_type)
  RELOC_NUMBER (R_MY66000_NONE,    0)
  RELOC_NUMBER (R_MY66000_PCREL8,  1)
  RELOC_NUMBER (R_MY66000_PCREL16, 2)
  RELOC_NUMBER (R_MY66000_PCREL26, 3)
  RELOC_NUMBER (R_MY66000_PCREL32, 4)
  RELOC_NUMBER (R_MY66000_PCREL64, 5)
  RELOC_NUMBER (R_MY66000_8,       6)
  RELOC_NUMBER (R_MY66000_16,      7)
  RELOC_NUMBER (R_MY66000_32,      8)
  RELOC_NUMBER (R_MY66000_64,      9)
END_RELOC_NUMBERS (R_MY66000_max)

#endif
