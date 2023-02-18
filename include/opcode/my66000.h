/* Definitions for decoding the my66000 opcodes.
   Copyright (C) 2009-2023 Free Software Foundation, Inc.
   Contributed by Thomas Koenig.

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
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA
   02110-1301, USA.  */

/* Instruction formats.  */

enum my66000_primary
{
 MY66000_FMT_BAD = 0,
 MY66000_MAJOR,
 MY66000_XOM6,
 MY66000_XOM7,
 MY66000_XOP1,
 MY66000_XOP2,
 MY66000_XOP4,
 MY66000_XOP5, 
};

typedef struct my66000_opc_info_t
{
  unsigned int primary_opcode;
  const char *name;
} my66000_opc_info_t;

extern const my66000_opc_info_t my66000_opc_info[32];
