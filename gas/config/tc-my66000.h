/* tc-my66000.h -- header file for tc-my66000.c.
   Copyright (C) 2018 Free Software Foundation, Inc.
   Contributed by Thomas König (tkoenig@gcc.gnu.org).

   This file is part of GAS.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING3. If not,
   see <http://www.gnu.org/licenses/>.  */

#ifndef TC_MY66000
#define TC_MY66000

#include "opcode/my66000.h"

#define TARGET_BYTES_BIG_ENDIAN 0

#define TARGET_ARCH bfd_arch_my66000

#define WORKING_DOT_WORD	1
#define LOCAL_LABELS_FB 	1

#define TARGET_FORMAT "elf64-my66000"

#define md_undefined_symbol(NAME)           0

/* These macros must be defined, but it will be a fatal assembler
   error if we ever hit them.  */
#define md_estimate_size_before_relax(A, B) (as_fatal (_("estimate size\n")), 0)
#define md_convert_frag(B, S, F)            as_fatal (_("convert_frag\n"))

/* Call md_pcrel_from_section(), not md_pcrel_from().  */
extern long md_pcrel_from_section (struct fix *, segT);
#define MD_PCREL_FROM_SECTION(FIX, SEC) md_pcrel_from_section (FIX, SEC)

#define md_section_align(SEGMENT, SIZE)     (SIZE)

#endif /* TC_MY66000 */
