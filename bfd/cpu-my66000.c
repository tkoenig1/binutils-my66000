/* BFD support for My 66000.
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
   along with this program; see the file COPYING3. If not,
   see <http://www.gnu.org/licenses/>.  */

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"


const bfd_arch_info_type bfd_my66000_arch =
  {
   64,               /* 64 bits in a word.  */
   64,               /* 64 bits in an address.  */
   8,                /*  8 bits in a byte.  */
   bfd_arch_my66000,   /* enum bfd_architecture arch.  */
   bfd_mach_my66000,
   "my66000",          /* Arch name.  */
   "my66000",          /* Printable name.  */
   3,                /* Unsigned int section alignment power.  */
   1,             /* The one and only.  */
   bfd_default_compatible, 
   bfd_default_scan ,
   bfd_arch_default_fill,
   NULL,
   0,
 };
