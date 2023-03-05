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

#define MINOR_OFFS 6
#define MINOR_MASK 63 << MINOR_OFFS

#define XOP4_OFFS 12
#define XOP4_MASK 7 << XOP4_OFFS

#define BB1_OFFS 21
#define BB1_MASK 31 << BB1_OFFS

#define BCND_OFFS 21
#define BCND_MASK 31 << BCND_OFFS

#define TT_OFFS 23
#define TT_MASK 7 << TT_OFFS

const my66000_opc_info_t my66000_opc_info[] =
{
 { "ill0", MY66000_MAJOR( 0), MY66000_ILL,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR( 1), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR( 2), MY66000_BAD,   NULL, 0, 0 },
 { NULL,   MY66000_MAJOR( 3), MY66000_BAD,   NULL, 0, 0 },
 { NULL,   MY66000_MAJOR( 4), MY66000_BAD,   NULL, 0, 0 },
 { NULL,   MY66000_MAJOR( 5), MY66000_BAD,   NULL, 0, 0 },
 { NULL,   MY66000_MAJOR( 6), MY66000_BAD,   my66000_opc_om6, SHFT_MASK, SHFT_OFFS},
 { NULL,   MY66000_MAJOR( 7), MY66000_BAD,   my66000_opc_om7, SHFT_MASK, SHFT_OFFS},
 { NULL,   MY66000_MAJOR( 8), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR( 9), MY66000_MEM,   my66000_opc_op1, MINOR_MASK, MINOR_OFFS},
 { NULL,   MY66000_MAJOR(10), MY66000_BAD,   my66000_opc_op2, MINOR_MASK, MINOR_OFFS},
 { NULL,   MY66000_MAJOR(11), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MY66000_MAJOR(12), MY66000_BAD,   my66000_opc_op4, XOP4_MASK, XOP4_OFFS},
 { NULL,   MY66000_MAJOR(13), MY66000_BAD,   my66000_opc_op5, MINOR_MASK, MINOR_OFFS},
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
 { "bb1",  MY66000_MAJOR(24), MY66000_BB1A,  my66000_opc_bb1a, BB1_MASK, BB1_OFFS},
 { "bb1",  MY66000_MAJOR(25), MY66000_BB1B,  my66000_opc_bb1b, BB1_MASK, BB1_OFFS},
 { "bcnd", MY66000_MAJOR(26), MY66000_BCND,  my66000_opc_bcnd, BCND_MASK, BCND_OFFS},
 { NULL,   MY66000_MAJOR(27), MY66000_BAD,   my66000_opc_jt, TT_MASK, TT_OFFS},
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

#define SHFT_MINOR(c) (c) << SHFT_MASK

#define MAJ6 MY66000_MAJOR(6)

/* Remark: Move above and make static.  */

const my66000_opc_info_t my66000_opc_om6[] =
{
 { "pb1",  MAJ6 | SHFT_MINOR( 0), MY66000_PB1A,  NULL, 0, 0},
 { "pcnd", MAJ6 | SHFT_MINOR( 1), MY66000_PCND,  NULL, 0, 0},
 { NULL,   MAJ6 | SHFT_MINOR( 2), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJ6 | SHFT_MINOR( 3), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJ6 | SHFT_MINOR( 4), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJ6 | SHFT_MINOR( 5), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJ6 | SHFT_MINOR( 6), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJ6 | SHFT_MINOR( 7), MY66000_BAD,   NULL, 0, 0},
 { "srl",  MAJ6 | SHFT_MINOR( 8), MY66000_SHIFT, NULL, 0, 0},
 { "sra",  MAJ6 | SHFT_MINOR( 9), MY66000_SHIFT, NULL, 0, 0},
 { "sll",  MAJ6 | SHFT_MINOR(10), MY66000_SHIFT, NULL, 0, 0},
 { "sla",  MAJ6 | SHFT_MINOR(11), MY66000_SHIFT, NULL, 0, 0},
 { "bitr", MAJ6 | SHFT_MINOR(12), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   MAJ6 | SHFT_MINOR(13), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJ6 | SHFT_MINOR(14), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJ6 | SHFT_MINOR(15), MY66000_BAD,   NULL, 0, 0},
 { NULL,   0,                 MY66000_END,   NULL, 0, 0}
};

#define MAJ7 MY66000_MAJOR(7)
const my66000_opc_info_t my66000_opc_om7[] =
{
 { "pb1",  MAJ7 | SHFT_MINOR( 0), MY66000_PB1B,  NULL, 0, 0},
 { "pcnd", MAJ7 | SHFT_MINOR( 1), MY66000_PCND,  NULL, 0, 0},
 { NULL,   MAJ7 | SHFT_MINOR( 2), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJ7 | SHFT_MINOR( 3), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJ7 | SHFT_MINOR( 4), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJ7 | SHFT_MINOR( 5), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJ7 | SHFT_MINOR( 6), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJ7 | SHFT_MINOR( 7), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJ7 | SHFT_MINOR( 8), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   MAJ7 | SHFT_MINOR( 9), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   MAJ7 | SHFT_MINOR(10), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   MAJ7 | SHFT_MINOR(11), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   MAJ7 | SHFT_MINOR(12), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   MAJ7 | SHFT_MINOR(13), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJ7 | SHFT_MINOR(14), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJ7 | SHFT_MINOR(15), MY66000_BAD,   NULL, 0, 0},
 { NULL,   0,                     MY66000_END,   NULL, 0, 0}
};


const my66000_opc_info_t my66000_opc_op1[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

const my66000_opc_info_t my66000_opc_op2[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

const my66000_opc_info_t my66000_opc_op4[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

const my66000_opc_info_t my66000_opc_op5[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

const my66000_opc_info_t my66000_opc_bcnd[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

const my66000_opc_info_t my66000_opc_jt[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

const my66000_opc_info_t my66000_opc_bb1a[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

const my66000_opc_info_t my66000_opc_bb1b[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
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

#define OPERAND_MASK(width,shift) ((1u << width) - 1) << shift, shift

/* Dept. of dirty tricks: Operands are encoded in the format string
   by consecutive capital letters.  Lookup is then via
   my66000_operand_table[c - 'A'].  An alternative would be to have
   strings using more descriptive names, like "dst", but that would
   create unnecessary overhead during parsing.  */

const my66000_operand_info_t my66000_operand_table[] =
{
 {MY66000_OPS_DST,     OPERAND_MASK ( 5, 21) /* A */ },
 {MY66000_OPS_SRC1,    OPERAND_MASK ( 5, 16) /* B */ },
 {MY66000_OPS_SRC2,    OPERAND_MASK ( 5,  0) /* C */ },
 {MY66000_OPS_RINDEX,  OPERAND_MASK ( 5,  0) /* D */ },
 {MY66000_OPS_IMMED16, OPERAND_MASK (16,  0) /* E */ },
};

/* My 66000 has instructions for which modifiers depend on the
   operands.  This section deals with encoding those.  */

/* Tables listing permissible arguments with modifiers.  */

/* For instructions which have no modifiers, the lists have
   only one entry.  */

static const my66000_fmt_spec_t opimm_fmt_list[] =
{
 { "A,B,#E", 0 },
 { NULL,     0 },
 
};

static const my66000_fmt_spec_t mem_fmt_list[] =
{
 { "A,[B,E]", 0 },
 { NULL,      0 },
};

/* Warning: Keep this table in the same order as my66000_encoding,
   this will be checked on startup.  */

const my66000_opcode_fmt_t my66000_opcode_fmt[] =
  {
   { NULL,              MY66000_BAD,        0 },
   { NULL,              MY66000_ILL,        0 },
   { opimm_fmt_list,    MY66000_OPIMM,      0 },
   { mem_fmt_list,      MY66000_MEM,        0 },
   { NULL,	        MY66000_END,        0 },
  };
