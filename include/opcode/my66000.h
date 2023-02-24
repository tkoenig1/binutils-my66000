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

/* Instructios and instruction classes.  */

#ifndef MY66000_H
#define MY66000_H

enum my66000_encoding
{
 MY66000_END = -1,
 MY66000_BAD = 0,  /* Empty.  */
 MY66000_ILL,      /* Reserved as non-opcodes.  */
 MY66000_OPIMM,
 MY66000_MEM,
 MY66000_OM6,
 MY66000_OM7,
 MY66000_OP1,
 MY66000_OP2,
 MY66000_OP4,
 MY66000_OP5,
 MY66000_PB1,
 MY66000_PCND,
 MY66000_SHIFT,
};

typedef struct my66000_opc_info_t
{
  int p_opc;	/* Primary opcode */
  int s_opc;	/* Secondary opcode, if applicable.  */
  const char *name;
  enum my66000_encoding enc;
  struct my66000_opc_info_t const *sub;  /* Subtable, if applicable.  */
} my66000_opc_info_t;

extern const my66000_opc_info_t my66000_opc_info[];
extern const my66000_opc_info_t my66000_opc_om6[];
extern const my66000_opc_info_t my66000_opc_om7[];
extern const my66000_opc_info_t my66000_opc_op1[];
extern const my66000_opc_info_t my66000_opc_op2[];
#endif
