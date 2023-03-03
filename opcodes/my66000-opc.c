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

const my66000_opc_info_t my66000_opc_info[] =
{
 {  0, 0, "ill0",  MY66000_ILL,	  NULL },
 {  1, 0, NULL,	   MY66000_BAD,	  NULL },
 {  2, 0, NULL,	   MY66000_BAD,	  NULL },
 {  3, 0, NULL,	   MY66000_BAD,	  NULL },
 {  4, 0, NULL,	   MY66000_BAD,	  NULL },
 {  5, 0, NULL,	   MY66000_BAD,	  NULL },
 {  6, 0, NULL,	   MY66000_OM6,	  my66000_opc_om6},
 {  7, 0, NULL,	   MY66000_OM7,	  my66000_opc_om7},
 {  8, 0, NULL,	   MY66000_BAD,	  NULL },
 {  9, 0, NULL,	   MY66000_MEM,	  my66000_opc_op1},
 { 10, 0, NULL,	   MY66000_OP2,	  my66000_opc_op2},
 { 11, 0, NULL,	   MY66000_BAD,	  NULL },
 { 12, 0, NULL,	   MY66000_OP4,	  my66000_opc_op4},
 { 13, 0, NULL,	   MY66000_OP5,	  my66000_opc_op5},
 { 14, 0, NULL,	   MY66000_BAD,	  NULL },
 { 15, 0, "ill1",  MY66000_ILL,	  NULL },
 { 16, 0, "ill2",  MY66000_ILL,	  NULL },
 { 17, 0, NULL,	   MY66000_BAD,	  NULL },
 { 18, 0, NULL,	   MY66000_BAD,	  NULL },
 { 19, 0, NULL,	   MY66000_BAD,	  NULL },
 { 20, 0, NULL,	   MY66000_BAD,	  NULL },
 { 21, 0, NULL,	   MY66000_BAD,	  NULL },
 { 22, 0, NULL,	   MY66000_BAD,	  NULL },
 { 23, 0, NULL,	   MY66000_BAD,	  NULL },
 { 24, 0, NULL,	   MY66000_BB,	  my66000_opc_bb1a},
 { 25, 0, NULL,	   MY66000_BB,	  my66000_opc_bb1b},
 { 26, 0, NULL,	   MY66000_BCND,  my66000_opc_bcnd},
 { 27, 0, NULL,	   MY66000_JT,	  my66000_opc_jt},
 { 28, 0, NULL,	   MY66000_BAD,	  NULL },
 { 29, 0, NULL,	   MY66000_BAD,	  NULL },
 { 30, 0, "br",	   MY66000_BR,	  NULL },
 { 31, 0, "call",  MY66000_CALL,  NULL },
 { 32, 0, "ldub",  MY66000_MEM,	  NULL },
 { 33, 0, "lduh",  MY66000_MEM,	  NULL },
 { 34, 0, "lduw",  MY66000_MEM,	  NULL },
 { 35, 0, "ldd",   MY66000_MEM,	  NULL },
 { 36, 0, "ldsb",  MY66000_MEM,	  NULL },
 { 37, 0, "ldsh",  MY66000_MEM,	  NULL },
 { 38, 0, "ldsw",  MY66000_MEM,	  NULL },
 { 39, 0, "exit",  MY66000_EXIT,  NULL },
 { 40, 0, "stb",   MY66000_MEM,	  NULL },
 { 41, 0, "sth",   MY66000_MEM,	  NULL },
 { 42, 0, "stw",   MY66000_MEM,	  NULL },
 { 43, 0, "std",   MY66000_MEM,	  NULL },
 { 44, 0, "enter", MY66000_EXIT,  NULL },
 { 45, 0, "ldm",   MY66000_MEMM,  NULL },
 { 46, 0, "stm",   MY66000_MEMM,  NULL },
 { 47, 0, "ill3",  MY66000_ILL,	  NULL },
 { 48, 0, "ill4",  MY66000_ILL,	  NULL },
 { 49, 0, "add",   MY66000_OPIMM, NULL },
 { 50, 0, "mul",   MY66000_OPIMM, NULL },
 { 51, 0, "div",   MY66000_OPIMM, NULL },
 { 52, 0, "cmp",   MY66000_OPIMM, NULL },
 { 53, 0, NULL,	   MY66000_BAD,	  NULL },
 { 54, 0, NULL,	   MY66000_BAD,	  NULL },
 { 55, 0, NULL,	   MY66000_BAD,	  NULL },
 { 56, 0, "or",	   MY66000_OPIMM, NULL },
 { 57, 0, "xor",   MY66000_OPIMM, NULL },
 { 58, 0, "and",   MY66000_OPIMM, NULL },
 { 59, 0, "mov",   MY66000_OPIMM, NULL },
 { 60, 0, "carry", MY66000_CARRY, NULL },
 { 61, 0, "vec",   MY66000_VEC,	  NULL },
 { 62, 0, NULL,	   MY66000_BAD,	  NULL },
 { 63, 0, "ill5",  MY66000_ILL,	  NULL },
 { -1, 0,  NULL,   MY66000_END,	  NULL }
};

const my66000_opc_info_t my66000_opc_om6[] =
{
 {  6,	0, "pb1",  MY66000_PB1,	   NULL},
 {  6,	1, "pcnd", MY66000_PCND,   NULL},
 {  6,	2, NULL,   MY66000_BAD,	   NULL},
 {  6,	3, NULL,   MY66000_BAD,	   NULL},
 {  6,	4, NULL,   MY66000_BAD,	   NULL},
 {  6,	5, NULL,   MY66000_BAD,	   NULL},
 {  6,	6, NULL,   MY66000_BAD,	   NULL},
 {  6,	7, NULL,   MY66000_BAD,	   NULL},
 {  6,	8, "srl",  MY66000_SHIFT,  NULL},
 {  6,	9, "sra",  MY66000_SHIFT,  NULL},
 {  6,	9, "sll",  MY66000_SHIFT,  NULL},
 {  6, 10, "sla",  MY66000_SHIFT,  NULL},
 {  6, 11, "bitr", MY66000_SHIFT,  NULL},
 {  6, 12, NULL,   MY66000_BAD,	   NULL},
 {  6, 13, NULL,   MY66000_BAD,	   NULL},
 {  6, 14, NULL,   MY66000_BAD,	   NULL},
 {  6, 15, NULL,   MY66000_BAD,	   NULL},
 { -1, 0,  NULL,   MY66000_END,	    NULL }
};

const my66000_opc_info_t my66000_opc_om7[] =
{
 {  7,	0, "pb1",  MY66000_PB1,	   NULL},
 {  7,	1, "pcnd", MY66000_PCND,   NULL},
 {  7,	2, NULL,   MY66000_BAD,	   NULL},
 {  7,	3, NULL,   MY66000_BAD,	   NULL},
 {  7,	4, NULL,   MY66000_BAD,	   NULL},
 {  7,	5, NULL,   MY66000_BAD,	   NULL},
 {  7,	6, NULL,   MY66000_BAD,	   NULL},
 {  7,	7, NULL,   MY66000_BAD,	   NULL},
 {  7,	8, NULL,   MY66000_BAD,	   NULL},
 {  7,	9, NULL,   MY66000_BAD,	   NULL},
 {  7,	9, NULL,   MY66000_BAD,	   NULL},
 {  7, 10, NULL,   MY66000_BAD,	   NULL},
 {  7, 11, NULL,   MY66000_BAD,	   NULL},
 {  7, 12, NULL,   MY66000_BAD,	   NULL},
 {  7, 13, NULL,   MY66000_BAD,	   NULL},
 {  7, 14, NULL,   MY66000_BAD,	   NULL},
 {  7, 15, NULL,   MY66000_BAD,	   NULL},
 { -1,	0, NULL,   MY66000_END,	   NULL }
};

const my66000_opc_info_t my66000_opc_op1[] =
{
 { -1, 0,  NULL,  MY66000_END,	 NULL }
};

const my66000_opc_info_t my66000_opc_op2[] =
{
 { -1, 0,  NULL,  MY66000_END,	 NULL }
};

const my66000_opc_info_t my66000_opc_op4[] =
{
 { -1, 0,  NULL,  MY66000_END,	 NULL }
};

const my66000_opc_info_t my66000_opc_op5[] =
{
 { -1, 0,  NULL,  MY66000_END,	 NULL }
};

const my66000_opc_info_t my66000_opc_bb1a[] =
{
 { -1, 0,  NULL,  MY66000_END,	 NULL }
};

const my66000_opc_info_t my66000_opc_bb1b[] =
{
 { -1, 0,  NULL,  MY66000_END,	 NULL }
};

const my66000_opc_info_t my66000_opc_bcnd[] =
{
 { -1, 0,  NULL,  MY66000_END,	 NULL }
};

const my66000_opc_info_t my66000_opc_jt[] =
{
 { -1, 0,  NULL,  MY66000_END,	 NULL }
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

const my66000_operand_info_t my66000_operand_table[] =
{
 {"dst",   MY66000_OPS_DST,	   5, 21 },
 {"src1",  MY66000_OPS_SRC1,	   5, 16 },
 {"src2",  MY66000_OPS_SRC2,     5,  0 },
 {"rind",  MY66000_OPS_RINDEX,   5,  0 },
 {"i16",   MY66000_OPS_IMMED16, 16,  0 },
 {NULL,    MY66000_OPS_END,      0,  0 },
};

/* Tables for the formats to be encoded for each instruction format.
   Warning: Keep this table in the same order as my66000_encoding,
   this will be checked on startup.  */

const my66000_opcode_fmt_t my66000_opcode_fmt[] =
  {
   {"dst,src1,#immed16",   MY66000_OPIMM, -1},
   {"dst,[src1,immed16]",  MY66000_MEM,   -1},
   {NULL,		   MY66000_END,   -1},
  };
