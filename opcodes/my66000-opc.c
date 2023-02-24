/* my66000-opc.c -- Definitions for moxie opcodes.
   Copyright (C) 2023 Free Software Foundation, Inc.
   Contributed by Thomas König (tkoenig@gcc.gnu.org).

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
   along with this file; see the file COPYING.  If not, write to the
   Free Software Foundation, 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#include "sysdep.h"
#include "opcode/my66000.h"

const my66000_opc_info_t my66000_opc_info[] =
{
 {  0, 0, "ill0", MY66000_ILL,   NULL },
 {  1, 0, NULL,   MY66000_BAD,   NULL },
 {  2, 0, NULL,   MY66000_BAD,   NULL },
 {  3, 0, NULL,   MY66000_BAD,   NULL },
 {  4, 0, NULL,   MY66000_BAD,   NULL },
 {  5, 0, NULL,   MY66000_BAD,   NULL },
 {  6, 0, NULL,   MY66000_OM6,   my66000_opc_om6},
 {  7, 0, NULL,   MY66000_OM7,   my66000_opc_om7},
 {  8, 0, NULL,   MY66000_BAD,   NULL },
 {  9, 0, NULL,   MY66000_MEM,   my66000_opc_op1},
 { 10, 0, NULL,   MY66000_OP2,   my66000_opc_op2},
 { 0b100000, 0, "ldub",	MY66000_MEM,   NULL },
 { 0b110001, 0, "add",	MY66000_OPIMM, NULL },
 { 31, 0,  NULL,  MY66000_BAD,   NULL },
 { 32, 0,  NULL,  MY66000_END,   NULL }
};

const my66000_opc_info_t my66000_opc_om6[] =
{
 {  6,  0, "pb1",  MY66000_PB1,    NULL},
 {  6,  1, "pcnd", MY66000_PCND,   NULL},
 {  6,  2, NULL,   MY66000_BAD,    NULL},
 {  6,  3, NULL,   MY66000_BAD,    NULL},
 {  6,  4, NULL,   MY66000_BAD,    NULL},
 {  6,  5, NULL,   MY66000_BAD,    NULL},
 {  6,  6, NULL,   MY66000_BAD,    NULL},
 {  6,  7, NULL,   MY66000_BAD,    NULL},
 {  6,  8, "srl",  MY66000_SHIFT,  NULL},
 {  6,  9, "sra",  MY66000_SHIFT,  NULL},
 {  6,  9, "sll",  MY66000_SHIFT,  NULL},
 {  6, 10, "sla",  MY66000_SHIFT,  NULL},
 {  6, 11, "bitr", MY66000_SHIFT,  NULL},
 {  6, 12, NULL,   MY66000_BAD,    NULL},
 {  6, 13, NULL,   MY66000_BAD,    NULL},
 {  6, 14, NULL,   MY66000_BAD,    NULL},
 {  6, 15, NULL,   MY66000_BAD,    NULL},
};

const my66000_opc_info_t my66000_opc_om7[] =
{
 {  7,  0, "pb1",  MY66000_PB1,    NULL},
 {  7,  1, "pcnd", MY66000_PCND,   NULL},
 {  7,  2, NULL,   MY66000_BAD,    NULL},
 {  7,  3, NULL,   MY66000_BAD,    NULL},
 {  7,  4, NULL,   MY66000_BAD,    NULL},
 {  7,  5, NULL,   MY66000_BAD,    NULL},
 {  7,  6, NULL,   MY66000_BAD,    NULL},
 {  7,  7, NULL,   MY66000_BAD,    NULL},
 {  7,  8, NULL,   MY66000_BAD,    NULL},
 {  7,  9, NULL,   MY66000_BAD,    NULL},
 {  7,  9, NULL,   MY66000_BAD,    NULL},
 {  7, 10, NULL,   MY66000_BAD,    NULL},
 {  7, 11, NULL,   MY66000_BAD,    NULL},
 {  7, 12, NULL,   MY66000_BAD,    NULL},
 {  7, 13, NULL,   MY66000_BAD,    NULL},
 {  7, 14, NULL,   MY66000_BAD,    NULL},
 {  7, 15, NULL,   MY66000_BAD,    NULL},
};

const my66000_opc_info_t my66000_opc_op1[] =
{
};

const my66000_opc_info_t my66000_opc_op2[] =
{
};



