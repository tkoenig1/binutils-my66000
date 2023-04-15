/* My66000 specific support for 64-bit ELF
   Copyright (C) 2023 Free Software Foundation, Inc.
   Contributed by Thomas König.

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

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "elf-bfd.h"

#define TARGET_LITTLE_SYM	bfd_elf64_my66000_vec
#define TARGET_LITTLE_NAME	"elf64-my66000"
#define ELF_ARCH		bfd_arch_my66000
#define ELF_MACHINE_CODE	EM_MY66000
#define ELF_MAXPAGESIZE		1

#define bfd_elf64_bfd_reloc_type_lookup		\
  bfd_default_reloc_type_lookup

#define bfd_elf64_bfd_reloc_name_lookup		\
  _bfd_norelocs_bfd_reloc_name_lookup

#define elf_info_to_ho \
  _bfd_elf_no_info_to_howto

#include "elf64-target.h"

