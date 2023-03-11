/* My 66000 specific support for 64-bit ELF.
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
#include "elf/my66000.h"

#define TARGET_LITTLE_SYM	my66000_elf64_vec
#define TARGET_LITTLE_NAME	"elf64-my66000"
#define ELF_ARCH		bfd_arch_my66000
#define ELF_MACHINE_CODE	EM_MY66000
#define ELF_MAXPAGESIZE		1

static reloc_howto_type my66000_elf_howto_table [] =
{
 /* Do-nothing relocation.  */
  HOWTO (R_MY66000_NONE,       	/* type */
	 0,			/* rightshift */
	 0,			/* size */
	 0,			/* bitsize */
	 false,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_dont, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_MY66000_NONE",	/* name */
	 false,			/* partial_inplace */
	 0,			/* src_mask */
	 0,			/* dst_mask */
	 false),		/* pcrel_offset */

  /* 16 bit PC-relative relocation, shifted two bits.  */
  HOWTO (R_MY66000_PCREL16,	/* type  */
	 2,			/* rightshift  */
	 2,			/* size  */
	 16,			/* bitsize  */
	 true,			/* pc_relative */
	 0,			/* bitpos  */
	 complain_overflow_signed, /* complain_on_overflow  */
	 bfd_elf_generic_reloc, /* special function  */
	 "R_MY66000_PCREL16",	/* name  */
	 false,			/* partial_inplace */
	 0,			/* src_mask  */
	 0xffff,		/* dst_mask  */
	 true),			/* pcrel_offset  */
};

/* Map BFD reloc types to My 66000 ELF reloc types.  */

typedef struct my66000_reloc_map_t
{
  bfd_reloc_code_real_type bfd_reloc_val;
  unsigned int my66000_reloc_val;
} my66000_reloc_map_t;

static const my66000_reloc_map_t my66000_reloc_map[] =
{
 {BFD_RELOC_NONE,      	 R_MY66000_NONE},
 {BFD_RELOC_16_PCREL_S2, R_MY66000_PCREL16},
};

static reloc_howto_type *
my66000_reloc_type_lookup (bfd *abfd ATTRIBUTE_UNUSED,
			 bfd_reloc_code_real_type code)
{
  unsigned int i;

  for (i = sizeof (my66000_reloc_map) / sizeof (my66000_reloc_map[0]);
       i--;)
    if (my66000_reloc_map [i].bfd_reloc_val == code)
      return & my66000_elf_howto_table [my66000_reloc_map[i].my66000_reloc_val];

  return NULL;
}

static reloc_howto_type *
my66000_reloc_name_lookup (bfd *abfd ATTRIBUTE_UNUSED, const char *r_name)
{
  unsigned int i;

  for (i = 0;
       i < sizeof (my66000_elf_howto_table) / sizeof (my66000_elf_howto_table[0]);
       i++)
    if (my66000_elf_howto_table[i].name != NULL
	&& strcasecmp (my66000_elf_howto_table[i].name, r_name) == 0)
      return &my66000_elf_howto_table[i];

  return NULL;
}

/* Set the howto pointer for an My 66000 ELF reloc.  */

static bool
my66000_info_to_howto_rela (bfd *abfd,
			    arelent *cache_ptr,
			    Elf_Internal_Rela *dst)
{
  unsigned int r_type;

  r_type = ELF32_R_TYPE (dst->r_info);
  if (r_type >= (unsigned int) R_MY66000_max)
    {
      /* xgettext:c-format */
      _bfd_error_handler (_("%pB: unsupported relocation type %#x"),
			  abfd, r_type);
      bfd_set_error (bfd_error_bad_value);
      return false;
    }
  cache_ptr->howto = & my66000_elf_howto_table [r_type];
  return true;
}

#define bfd_elf64_bfd_reloc_type_lookup		\
  my66000_reloc_type_lookup

#define bfd_elf64_bfd_reloc_name_lookup		\
  my66000_reloc_name_lookup

#define elf_info_to_howto \
  my66000_info_to_howto_rela

#include "elf64-target.h"

