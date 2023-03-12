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
   or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with this file; see the file COPYING.	If not, write to the
   Free Software Foundation, 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.	*/

#include "sysdep.h"
#include <stdint.h>
#include "opcode/my66000.h"

/* Macro so we can fill up the frag_opc, frag_mask and shift
   members more easily.  */

#define SHFT_OFFS 12
#define SHFT_MASK 15 << SHFT_OFFS

#define MINOR_OFFS 5
#define MINOR_MASK 63 << MINOR_OFFS

#define XOP4_OFFS 12
#define XOP4_MASK 7 << XOP4_OFFS

#define BB1_OFFS 21
#define BB1_MINOR(c) ((c) << BB1_OFFS)
#define BB1_MASK 31 << BB1_OFFS


#define BCND_OFFS 21
#define BCND_MASK 31 << BCND_OFFS

#define TT_OFFS 23
#define TT_MASK 7 << TT_OFFS

#define SHFT_MINOR(c) (c) << SHFT_OFFS
#define MINOR(c) (c) << MINOR_OFFS

#define XOP_I_SHFT 15
#define XOP_I(c) ((c) << XOP_I_SHFT)
#define XOP_I_MASK XOP_I(1)

#define XOP_S1_SHFT 14
#define XOP_S1(c) ((c) << XOP_S1_SHFT)
#define XOP_S1_MASK XOP_S1(1)

#define XOP_S2_SHFT 13
#define XOP_S2(c) ((c) << XOP_S2_SHFT)
#define XOP_S2_MASK XOP_S2(1)

/* Bit 12 is the signed bit.  */

#define XOP2_d_SHFT 11
#define XOP2_d(c) ((c) << XOP2_d_SHFT)
#define XOP2_d_MASK XOP2_d(1)

#define OPC6_MAJOR MY66000_MAJOR(6)
#define OPC7_MAJOR MY66000_MAJOR(7)

#define OP2_MAJOR MY66000_MAJOR(10)

const my66000_opc_info_t opc_om6[] =
{
 { "pb1",  OPC6_MAJOR | SHFT_MINOR( 0), MY66000_PB1A,  NULL, 0, 0},
 { "pcnd", OPC6_MAJOR | SHFT_MINOR( 1), MY66000_PCND,  NULL, 0, 0},
 { NULL,   OPC6_MAJOR | SHFT_MINOR( 2), MY66000_BAD,   NULL, 0, 0},
 { NULL,   OPC6_MAJOR | SHFT_MINOR( 3), MY66000_BAD,   NULL, 0, 0},
 { NULL,   OPC6_MAJOR | SHFT_MINOR( 4), MY66000_BAD,   NULL, 0, 0},
 { NULL,   OPC6_MAJOR | SHFT_MINOR( 5), MY66000_BAD,   NULL, 0, 0},
 { NULL,   OPC6_MAJOR | SHFT_MINOR( 6), MY66000_BAD,   NULL, 0, 0},
 { NULL,   OPC6_MAJOR | SHFT_MINOR( 7), MY66000_BAD,   NULL, 0, 0},
 { "srl",  OPC6_MAJOR | SHFT_MINOR( 8), MY66000_SHIFT, NULL, 0, 0},
 { "sra",  OPC6_MAJOR | SHFT_MINOR( 9), MY66000_SHIFT, NULL, 0, 0},
 { "sll",  OPC6_MAJOR | SHFT_MINOR(10), MY66000_SHIFT, NULL, 0, 0},
 { "sla",  OPC6_MAJOR | SHFT_MINOR(11), MY66000_SHIFT, NULL, 0, 0},
 { "bitr", OPC6_MAJOR | SHFT_MINOR(12), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   OPC6_MAJOR | SHFT_MINOR(13), MY66000_BAD,   NULL, 0, 0},
 { NULL,   OPC6_MAJOR | SHFT_MINOR(14), MY66000_BAD,   NULL, 0, 0},
 { NULL,   OPC6_MAJOR | SHFT_MINOR(15), MY66000_BAD,   NULL, 0, 0},
 { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

const my66000_opc_info_t opc_om7[] =
{
 { "pb1",  OPC7_MAJOR | SHFT_MINOR( 0), MY66000_PB1B,  NULL, 0, 0},
 { "pcnd", OPC7_MAJOR | SHFT_MINOR( 1), MY66000_PCND,  NULL, 0, 0},
 { NULL,   OPC7_MAJOR | SHFT_MINOR( 2), MY66000_BAD,   NULL, 0, 0},
 { NULL,   OPC7_MAJOR | SHFT_MINOR( 3), MY66000_BAD,   NULL, 0, 0},
 { NULL,   OPC7_MAJOR | SHFT_MINOR( 4), MY66000_BAD,   NULL, 0, 0},
 { NULL,   OPC7_MAJOR | SHFT_MINOR( 5), MY66000_BAD,   NULL, 0, 0},
 { NULL,   OPC7_MAJOR | SHFT_MINOR( 6), MY66000_BAD,   NULL, 0, 0},
 { NULL,   OPC7_MAJOR | SHFT_MINOR( 7), MY66000_BAD,   NULL, 0, 0},
 { NULL,   OPC7_MAJOR | SHFT_MINOR( 8), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   OPC7_MAJOR | SHFT_MINOR( 9), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   OPC7_MAJOR | SHFT_MINOR(10), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   OPC7_MAJOR | SHFT_MINOR(11), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   OPC7_MAJOR | SHFT_MINOR(12), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   OPC7_MAJOR | SHFT_MINOR(13), MY66000_BAD,   NULL, 0, 0},
 { NULL,   OPC7_MAJOR | SHFT_MINOR(14), MY66000_BAD,   NULL, 0, 0},
 { NULL,   OPC7_MAJOR | SHFT_MINOR(15), MY66000_BAD,   NULL, 0, 0},
 { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

#define SIGNED_OFFS 12
#define SIGNED(c) ((c) << SIGNED_OFFS)
#define SIGNED_MASK SIGNED (1)

/* The S bit for the XOP2 group forces a different name, and is not
   adjacent to the minor opcode, so it is not staightforward to
   encode.  Use separate tables for each instruction, then.  If you
   add a table here, do not forget to add it to
   my66000_opc_info_list.  */


/* Definition for the various modifier bits in the XOP2 group.  */

#define XOP2_I_OFFS 15
#define XOP2_I(c) ((c) << XOP2_I_OFFS)
#define XOP2_I_MASK XOP2_I(1)

#define XOP2_S1_OFFS 14
#define XOP2_S1(c) ((c) << XOP2_S1_OFFS)
#define XOP2_S1_MASK XOP2_S1(1)

#define XOP2_S2_OFFS 13
#define XOP2_S2(c) ((c) << XOP2_S2_OFFS)
#define XOP2_S2_MASK XOP2_S2(1)

/* The S bit, at position 12, is already used in the opcode tables
   above.  */

#define XOP2_MOD_MASK (XOP2_I_MASK | XOP2_S1_MASK | XOP2_S2_MASK | XOP2_d_MASK)

const my66000_opc_info_t opc_op1[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
};


const my66000_opc_info_t opc_add [] =
{
 {"add",  OP2_MAJOR | MINOR(33) | SIGNED(0), MY66000_OP2, NULL, 0, 0},
 {"adds", OP2_MAJOR | MINOR(33) | SIGNED(1), MY66000_OP2, NULL, 0, 0},
 { NULL,   0,        MY66000_END, NULL, 0, 0}
};
  
const my66000_opc_info_t opc_op2[] =
{
 { NULL,    MINOR ( 0), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR ( 1), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR ( 2), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR ( 3), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR ( 4), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR ( 5), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR ( 6), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR ( 7), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR ( 8), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR ( 9), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (10), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (11), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (12), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (13), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (14), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (15), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (16), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (17), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (18), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (19), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (20), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (21), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (22), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (23), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (24), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (25), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (26), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (27), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (28), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (29), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (30), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (31), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (32), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (33), MY66000_BAD,  opc_add, SIGNED_MASK, SIGNED_OFFS},
 { NULL,    MINOR (34), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (35), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (36), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (37), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (38), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (39), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (40), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (41), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (42), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (43), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (44), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (45), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (46), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (47), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (48), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (49), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (50), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (51), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (52), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (53), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (54), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (55), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (56), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (57), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (58), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (59), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (60), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (61), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (62), MY66000_BAD,  NULL, 0, 0},
 { NULL,    MINOR (63), MY66000_BAD,  NULL, 0, 0},
 { NULL,    0,          MY66000_END,  NULL, 0, 0}
};

const my66000_opc_info_t opc_op4[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

const my66000_opc_info_t opc_op5[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

const my66000_opc_info_t opc_bcnd[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

const my66000_opc_info_t opc_jt[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

/* FIXME: Disassembling is ambigous.  */

/* The two halves of the branch on bit instructions.  */
const my66000_opc_info_t opc_bb1a[] =
{
 { "bne",  MY66000_MAJOR(24) | BB1_MINOR( 0), MY66000_BB1A, NULL, 0, 0},
 { "beq",  MY66000_MAJOR(24) | BB1_MINOR( 1), MY66000_BB1A, NULL, 0, 0},
 { "bgt",  MY66000_MAJOR(24) | BB1_MINOR( 2), MY66000_BB1A, NULL, 0, 0},
 { "bge",  MY66000_MAJOR(24) | BB1_MINOR( 3), MY66000_BB1A, NULL, 0, 0},
 { "blt",  MY66000_MAJOR(24) | BB1_MINOR( 4), MY66000_BB1A, NULL, 0, 0},
 { "ble",  MY66000_MAJOR(24) | BB1_MINOR( 5), MY66000_BB1A, NULL, 0, 0},
 { "bor",  MY66000_MAJOR(24) | BB1_MINOR( 6), MY66000_BB1A, NULL, 0, 0},
 { "bun",  MY66000_MAJOR(24) | BB1_MINOR( 7), MY66000_BB1A, NULL, 0, 0},
 { "bnne", MY66000_MAJOR(24) | BB1_MINOR( 8), MY66000_BB1A, NULL, 0, 0},
 { "bneq", MY66000_MAJOR(24) | BB1_MINOR( 9), MY66000_BB1A, NULL, 0, 0},
 { "bhi",  MY66000_MAJOR(24) | BB1_MINOR(10), MY66000_BB1A, NULL, 0, 0},
 { "bhs",  MY66000_MAJOR(24) | BB1_MINOR(11), MY66000_BB1A, NULL, 0, 0},
 { "blo",  MY66000_MAJOR(24) | BB1_MINOR(12), MY66000_BB1A, NULL, 0, 0},
 { "bls",  MY66000_MAJOR(24) | BB1_MINOR(13), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MY66000_MAJOR(24) | BB1_MINOR(14), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MY66000_MAJOR(24) | BB1_MINOR(15), MY66000_BB1A, NULL, 0, 0},
 { "sin",  MY66000_MAJOR(24) | BB1_MINOR(16), MY66000_BB1A, NULL, 0, 0},
 { "fin",  MY66000_MAJOR(24) | BB1_MINOR(17), MY66000_BB1A, NULL, 0, 0},
 { "cin",  MY66000_MAJOR(24) | BB1_MINOR(18), MY66000_BB1A, NULL, 0, 0},
 { "cin",  MY66000_MAJOR(24) | BB1_MINOR(19), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MY66000_MAJOR(24) | BB1_MINOR(20), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MY66000_MAJOR(24) | BB1_MINOR(21), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MY66000_MAJOR(24) | BB1_MINOR(23), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MY66000_MAJOR(24) | BB1_MINOR(24), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MY66000_MAJOR(24) | BB1_MINOR(25), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MY66000_MAJOR(24) | BB1_MINOR(26), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MY66000_MAJOR(24) | BB1_MINOR(27), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MY66000_MAJOR(24) | BB1_MINOR(28), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MY66000_MAJOR(24) | BB1_MINOR(29), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MY66000_MAJOR(24) | BB1_MINOR(30), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MY66000_MAJOR(24) | BB1_MINOR(31), MY66000_BB1A, NULL, 0, 0},
 { NULL,   0,                                 MY66000_END , NULL, 0, 0}
};

const my66000_opc_info_t opc_bb1b[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
  { NULL,   0,                                 MY66000_END , NULL, 0, 0},
} ;

/* The major table - the only one we export as a global symbol.  */

const my66000_opc_info_t my66000_opc_info[] =
{
 { "ill0", MY66000_MAJOR( 0), MY66000_ILL,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR( 1), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR( 2), MY66000_BAD,   NULL, 0, 0 },
 { NULL,   MY66000_MAJOR( 3), MY66000_BAD,   NULL, 0, 0 },
 { NULL,   MY66000_MAJOR( 4), MY66000_BAD,   NULL, 0, 0 },
 { NULL,   MY66000_MAJOR( 5), MY66000_BAD,   NULL, 0, 0 },
 { NULL,   MY66000_MAJOR( 6), MY66000_BAD,   opc_om6, SHFT_MASK, SHFT_OFFS},
 { NULL,   MY66000_MAJOR( 7), MY66000_BAD,   opc_om7, SHFT_MASK, SHFT_OFFS},
 { NULL,   MY66000_MAJOR( 8), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR( 9), MY66000_MEM,   opc_op1, MINOR_MASK, MINOR_OFFS},
 { NULL,   MY66000_MAJOR(10), MY66000_BAD,   opc_op2, MINOR_MASK, MINOR_OFFS},
 { NULL,   MY66000_MAJOR(11), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR(12), MY66000_BAD,   opc_op4, XOP4_MASK, XOP4_OFFS},
 { NULL,   MY66000_MAJOR(13), MY66000_BAD,   opc_op5, MINOR_MASK, MINOR_OFFS},
 { NULL,   MY66000_MAJOR(14), MY66000_BAD,   NULL, 0, 0},
 { "ill1", MY66000_MAJOR(15), MY66000_ILL,   NULL, 0, 0},
 { "ill2", MY66000_MAJOR(16), MY66000_ILL,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR(17), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR(18), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR(19), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR(20), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR(21), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR(22), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR(23), MY66000_BAD,   NULL, 0, 0},
 { "bb1",  MY66000_MAJOR(24), MY66000_BB1A,  opc_bb1a, BB1_MASK, BB1_OFFS},
 { "bb1",  MY66000_MAJOR(25), MY66000_BB1B,  opc_bb1b, BB1_MASK, BB1_OFFS},
 { "bcnd", MY66000_MAJOR(26), MY66000_BCND,  opc_bcnd, BCND_MASK, BCND_OFFS},
 { NULL,   MY66000_MAJOR(27), MY66000_BAD,   opc_jt, TT_MASK, TT_OFFS},
 { NULL,   MY66000_MAJOR(28), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR(29), MY66000_BAD,   NULL, 0, 0},
 { "br",   MY66000_MAJOR(30), MY66000_BR,    NULL, 0, 0},
 { "call", MY66000_MAJOR(31), MY66000_CALL,  NULL, 0, 0},
 { "ldub", MY66000_MAJOR(32), MY66000_MEM,   NULL, 0, 0},
 { "lduh", MY66000_MAJOR(33), MY66000_MEM,   NULL, 0, 0},
 { "lduw", MY66000_MAJOR(34), MY66000_MEM,   NULL, 0, 0},
 { "ldd",  MY66000_MAJOR(35), MY66000_MEM,   NULL, 0, 0},
 { "ldsb", MY66000_MAJOR(36), MY66000_MEM,   NULL, 0, 0},
 { "ldsh", MY66000_MAJOR(37), MY66000_MEM,   NULL, 0, 0},
 { "ldsw", MY66000_MAJOR(38), MY66000_MEM,   NULL, 0, 0},
 { "exit", MY66000_MAJOR(39), MY66000_EXIT,  NULL, 0, 0},
 { "stb",  MY66000_MAJOR(40), MY66000_MEM,   NULL, 0, 0},
 { "sth",  MY66000_MAJOR(41), MY66000_MEM,   NULL, 0, 0},
 { "stw",  MY66000_MAJOR(42), MY66000_MEM,   NULL, 0, 0},
 { "std",  MY66000_MAJOR(43), MY66000_MEM,   NULL, 0, 0},
 { "enter", MY66000_MAJOR(44), MY66000_EXIT,  NULL, 0, 0},
 { "ldm",  MY66000_MAJOR(45), MY66000_MEMM,  NULL, 0, 0},
 { "stm",  MY66000_MAJOR(46), MY66000_MEMM,  NULL, 0, 0},
 { "ill3", MY66000_MAJOR(47), MY66000_ILL,   NULL, 0, 0},
 { "ill4", MY66000_MAJOR(48), MY66000_ILL,   NULL, 0, 0},
 { "add",  MY66000_MAJOR(49), MY66000_OPIMM, NULL, 0, 0},
 { "mul",  MY66000_MAJOR(50), MY66000_OPIMM, NULL, 0, 0},
 { "div",  MY66000_MAJOR(51), MY66000_OPIMM, NULL, 0, 0},
 { "cmp",  MY66000_MAJOR(52), MY66000_OPIMM, NULL, 0, 0},
 { NULL,   MY66000_MAJOR(53), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR(54), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR(55), MY66000_BAD,   NULL, 0, 0},
 { "or",   MY66000_MAJOR(56), MY66000_OPIMM, NULL, 0, 0},
 { "xor",  MY66000_MAJOR(57), MY66000_OPIMM, NULL, 0, 0},
 { "and",  MY66000_MAJOR(58), MY66000_OPIMM, NULL, 0, 0},
 { "mov",  MY66000_MAJOR(59), MY66000_OPIMM, NULL, 0, 0},
 { "carry", MY66000_MAJOR(60), MY66000_CARRY, NULL, 0, 0},
 { "vec",  MY66000_MAJOR(61), MY66000_VEC,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR(62), MY66000_BAD,   NULL, 0, 0},
 { "ill5", MY66000_MAJOR(63), MY66000_BAD,   NULL, 0, 0},
 { NULL,   0,                 MY66000_END,   NULL, 0, 0},
};

/* List of all the tables containing opcodes, for initializing the
   hashtabs for gas.  Keep up to date!  */

const my66000_opc_info_t *my66000_opc_info_list[] =
{
 my66000_opc_info, opc_om6, opc_om7, opc_add, opc_op1, opc_op2, opc_op4, opc_op5,
 opc_bcnd, opc_jt, opc_bb1a, opc_bb1b, NULL
};
  
const char *my66000_rname[32] =
  {
    "r0",  "r1",  "r2",	 "r3",	"r4",  "r5",  "r6",  "r7",
    "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15",
    "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23",
    "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31"
  };

const char *my66000_rbase[32] =
  {
    "ip",  "r1",  "r2",	 "r3",	"r4",  "r5",  "r6",  "r7",
    "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15",
    "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23",
    "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31"
  };

const char *my66000_rind[32] =
  {
    NULL,  "r1",  "r2",	 "r3",	"r4",  "r5",  "r6",  "r7",
    "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15",
    "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23",
    "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31"
  };

const char *my66000_rnum[32] =
  {
    "0",  "1",	"2",  "3",  "4",  "5",	"6",  "7",
    "8",  "9",	"10", "11", "12", "13", "14", "15",
    "16", "17", "18", "19", "20", "21", "22", "23",
    "24", "25", "26", "27", "28", "29", "30", "31"
  };

const char my66000_numtab[32] =
  {
   0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
   16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
  };

#define OPERAND_MASK(width,shift) ((1u << (width)) - 1) << (shift)
#define OPERAND_ENTRY(width,shift) OPERAND_MASK(width,shift), shift

/* Dept. of dirty tricks: Operands are encoded in the format string
   by capital letters starting with A.  Lookup is then via
   my66000_operand_table[c - 'A'].  An alternative would be to have
   strings using more descriptive names, like "dst" and put them in a
   hash, but that would create unnecessary overhead during
   parsing.  */

const my66000_operand_info_t my66000_operand_table[] =
{
 {MY66000_OPS_DST,    OPERAND_ENTRY ( 5, 21), "Destination register",    'A' },
 {MY66000_OPS_SRC1,   OPERAND_ENTRY ( 5, 16), "Source register 1",       'B' },
 {MY66000_OPS_SRC2,   OPERAND_ENTRY ( 5,  0), "Source register 2",       'C' },
 {MY66000_OPS_RINDEX, OPERAND_ENTRY ( 5,  0), "Inded register",          'D' },
 {MY66000_OPS_IMM16,  OPERAND_ENTRY (16,  0), "16-bit signed immediate", 'E' },
 {MY66000_OPS_I1,     OPERAND_ENTRY ( 5, 16), "5-bit constant source 1", 'F' },
 {MY66000_OPS_I2,     OPERAND_ENTRY ( 5,  0), "5-bit constant source 2", 'G' },
 {MY66000_OPS_BB1,    OPERAND_ENTRY ( 6, 21), "Bit number",		 'H' },
};

/* My 66000 has instructions for which modifiers depend on the
   operands.  This section deals with encoding those.  */

/* Some patterns have a requirement of zeros where otherwise
   registers would be.  Compare to my66000_operand_table above.  */

#define SRC1_MASK OPERAND_MASK(5,16)
#define SRC2_MASK OPERAND_MASK(5, 0)

static const my66000_fmt_spec_t opimm_fmt_list[] =
{
 { "A,B,#E", 0, 0 },
 { NULL,     0, 0 },
 
};

static const my66000_fmt_spec_t mem_fmt_list[] =
{
 { "A,[B,E]", 0, 0 },
 { NULL,      0, 0 },
};

#define XOP2_MAP (XOP2_I(1) | XOP2_S1(1) | XOP2_S2(1) | XOP2_d(1))

static const my66000_fmt_spec_t op2_fmt_list [] =
{
 { "A,B,C",    XOP2_I(0) | XOP2_d(0) | XOP2_S1(0) | XOP2_S2(0) , XOP2_MAP },
 { "A,B,-C",   XOP2_I(0) | XOP2_d(0) | XOP2_S1(0) | XOP2_S2(1) , XOP2_MAP },
 { "A,-B,C",   XOP2_I(0) | XOP2_d(0) | XOP2_S1(1) | XOP2_S2(0) , XOP2_MAP },
 { "A,-B,-C",  XOP2_I(0) | XOP2_d(0) | XOP2_S1(1) | XOP2_S2(1) , XOP2_MAP },
 { "A,B,#F",   XOP2_I(0) | XOP2_d(1) | XOP2_S1(0) | XOP2_S2(0) , XOP2_MAP },
 { "A,#G,C",   XOP2_I(0) | XOP2_d(1) | XOP2_S1(0) | XOP2_S2(1) , XOP2_MAP },
 { "A,B,#-F",  XOP2_I(0) | XOP2_d(1) | XOP2_S1(1) | XOP2_S2(0) , XOP2_MAP },
 { "A,#-G,C",  XOP2_I(0) | XOP2_d(1) | XOP2_S1(1) | XOP2_S2(1) , XOP2_MAP },
 { NULL,      0, 0 }, 
};

static const my66000_fmt_spec_t bb1_fmt_list [] =
{
 { "H,B,E", 0, 0},
 { NULL,    0, 0},
};

/* Warning: Keep this table in the same order as my66000_encoding in
   include/opcode/my66000.h, this will be checked on startup of gas.  */

const my66000_opcode_fmt_t my66000_opcode_fmt[] =
  {
   { NULL,              MY66000_BAD,    0 },
   { NULL,              MY66000_ILL,    0 },
   { opimm_fmt_list,    MY66000_OPIMM,  0 },
   { mem_fmt_list,      MY66000_MEM,    0 },
   { op2_fmt_list,      MY66000_OP2,    0 },
   { bb1_fmt_list,	MY66000_BB1A,	0 },
   { bb1_fmt_list,	MY66000_BB1B,	0 },
   { NULL,	        MY66000_END,    0 },
  };
