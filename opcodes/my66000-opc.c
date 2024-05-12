/* my66000-opc.c -- Definitions for my66000 opcodes.
   Copyright (C) 2023, 2024 Free Software Foundation, Inc.
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

/* Things to do for adding new instructions:

   If a new instruction needs a new type of operand:

   Go into include/opcode/my66000.h and extend the my66000_operands
   enum.  Put the new operand into the my66000_operand_table (using a
   new letter).  If single letters run out, then you will have to
   extend the way that the format strings are represented.  For
   historical reasons, there are also a few unused letters in between
   the other ones, which can be used.

   Then, handle the new operand in the big switch statement in
   match_arglist() in gas/config/tc-my66000.c for assembly and in the
   corresponding big switch statement in print_operands() in
   opcodes/my66000-dis.c.

   If a new instruction needs a new format:

   Extend the my66000_encoding enum in include/opcode/my66000.h with
   the new format, and add a corresponding entry to the
   my66000_opcode_fmt table in this file. It has to be at the same
   place.  This table entry contains the address of a table which
   contains a list of the format strings.  The list of the format
   string contains:  The format string (as a C string), te bits
   (ORed together) which have to be equal to one for the format
   to match, and the mask under which the bits have to match, which
   then also includes zero.

   How to add a new instruction:

   The my66000_opc_info tables contain the name of the instruction (if
   any), the bit pattern used for assembly, the type of the
   instruction (used for looking up the formats), a subtable (if any)
   and a mask and offset for looking up the instruction in the
   disassember; see the code in print_insn_my66000.  If a new table is
   needed, create a new one and add it to my66000_opc_info_list[] so
   the assembler can find it.

*/

#include "sysdep.h"
#include <stdint.h>
#include <assert.h>
#include "disassemble.h"
#include "opcode/my66000.h"

/* Macro so we can fill up the patt_opc, patt_mask and shift
   members more easily.  */

#define MAJOR(c) ((c) << MY66000_MAJOR_SHIFT)
#define MAJOR_MASK (63 << MY66000_MAJOR_SHIFT)

#define SHFT_OFFS 12
#define SHFT_MASK 15 << SHFT_OFFS
#define SHFT_MINOR(c) (c) << SHFT_OFFS

#define MINOR_OFFS 5
#define MINOR(c) ((c) << MINOR_OFFS)
#define MINOR_MASK MINOR(63)

#define XOP4_OFFS 10
#define XOP4_MINOR(c) ((c) << XOP4_OFFS)
#define XOP4_MASK XOP4_MINOR(7)

#define XOP5_OFFS 0

#define XOP5_FCN(c) ((c) << XOP5_OFFS)
#define XOP5_FCN_MASK XOP5_FCN(31)
#define XOP5_ABS_MASK 1

#define XOP5_FMT_ABS_MASK XOP5_FCN(30)

#define BB1_OFFS 21
#define BB1_MINOR(c) ((c) << BB1_OFFS)
#define BB1_MASK 31 << BB1_OFFS

#define CND_OFFS 21
#define CND_MINOR(c) ((c) << CND_OFFS)
#define CND_MASK 31 << CND_OFFS

/* Only two of the three bits of the BOP are currently used, and
   at the moment, we do not support calls to remote tables,
   so BOP is 0 or 1 only.  */

#define TT_OFFS 23
#define TT_MINOR(c) ((c) << TT_OFFS)
#define TT_MASK TT_MINOR(1)

#define TT_SIZE_OFFS 21
#define TT_SIZE(c) ((c) << TT_SIZE_OFFS)
#define TT_SIZE_MASK TT_SIZE(3)

#define MOD_OFFS 16
#define MOD_MINOR(x) ((x) << MOD_OFFS)
#define MOD_MASK MOD_MINOR (3)

#define OP5_OFFS 0
#define OP5_MINOR(x) ((x) << OPF_OFFS)
#define OP5_MASK OP5_MINOR(31)

#define LOOP_OFFS 10
#define LOOP_MINOR(x) ((x) << LOOP_OFFS)
#define LOOP_MASK LOOP_MINOR(3)

const my66000_opc_info_t opc_info_nop[];
static const my66000_opc_info_t opc_abs[];
const my66000_opc_info_t my66000_opc_info[];
static const my66000_opc_info_t opc_om6[];
static const my66000_opc_info_t opc_om7[];
static const my66000_opc_info_t opc_mrr[];
static const my66000_opc_info_t opc_op1[];
static const my66000_opc_info_t opc_arith[];
static const my66000_opc_info_t opc_float[];
static const my66000_opc_info_t opc_op2[];
static const my66000_opc_info_t opc_mpx[];
static const my66000_opc_info_t opc_op4[];
static const my66000_opc_info_t opc_op5[];
static const my66000_opc_info_t opc_bcnd[];
static const my66000_opc_info_t opc_tt[];
static const my66000_opc_info_t opc_jtt[];
static const my66000_opc_info_t opc_bb1a[];
static const my66000_opc_info_t opc_bb1b[];
static const my66000_opc_info_t opc_mod[];
static const my66000_opc_info_t opc_pb1a[];
static const my66000_opc_info_t opc_pb1b[];
static const my66000_opc_info_t opc_pcnd[];
static const my66000_opc_info_t opc_hr[];
static const my66000_opc_info_t opc_jmp[];
static const my66000_opc_info_t opc_calli[];
static const my66000_opc_info_t opc_single[];
static const my66000_opc_info_t opc_ff1[];
static const my66000_opc_info_t opc_trans[];
static const my66000_opc_info_t opc_transd[];
static const my66000_opc_info_t opc_transf[];
static const my66000_opc_info_t opc_cvt[];
static const my66000_opc_info_t opc_loop[];
static const my66000_opc_info_t opc_loopu[];
static const my66000_opc_info_t opc_loops[];

/* List of all the tables containing opcodes, for initializing the
   hashtabs for gas.  Keep up to date if you add anything below,
   otherwise the instructions will not be found.  The order in this
   table is the order in which opcodes will be searched for.  Easiest
   if you keep this in the same order as the declarations above.  */

const my66000_opc_info_t *my66000_opc_info_list[] =
{
 opc_info_nop,
 opc_abs,
 my66000_opc_info,
 opc_om6,
 opc_om7,
 opc_mrr,
 opc_op1,
 opc_arith,
 opc_float,
 opc_op2,
 opc_mpx,
 opc_op4,
 opc_op5,
 opc_bcnd,
 opc_tt,
 opc_jtt,
 opc_bb1a,
 opc_bb1b,
 opc_mod,
 opc_pb1a,
 opc_pb1b,
 opc_pcnd,
 opc_hr,
 opc_jmp,
 opc_calli,
 opc_single,
 opc_trans,
 opc_transd,
 opc_transf,
 opc_cvt,
 /* opc_loop we don't need, no instructions in there.  */
 opc_loopu,
 opc_loops,
 NULL,
};

/* Table for special opcodes which are not well suited for recursive
   table lookup.  Will be looked at first by the assembler and
   disassembler. nop is "or r0, r0, #0000", jmp is "hrw ip,rs",
   calli is "hrx r0,ip,rs.  */

const my66000_opc_info_t opc_info_nop[] =
{
 { "nop", MAJOR(56), MY66000_NOP, NULL, 0, 0},
 { NULL,  0,         MY66000_END, NULL, 0, 0},
};

#define SIGNED_SHFT 12
#define SIGNED(c) ((c) << SIGNED_SHFT)
#define SIGNED_MASK SIGNED (1)

#define FCN_SHIFT 0
#define FF1(c) ((c) << FCN_SHIFT)
#define FF1_MASK FF1(3)
#define FF1_FMT_MASK (XOP5_FCN(31) - FF1_MASK)

/* The major table - the only one we export as a global symbol.  */

const my66000_opc_info_t my66000_opc_info[] =
{
 { "ill0", MAJOR( 0), MY66000_ILL,   NULL, 0, 0},
 { NULL,   MAJOR( 1), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR( 2), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR( 3), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR( 4), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR( 5), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR( 6), MY66000_BAD,   opc_om6, SHFT_MASK, SHFT_OFFS},
 { NULL,   MAJOR( 7), MY66000_BAD,   opc_om7, SHFT_MASK, SHFT_OFFS},
 { NULL,   MAJOR( 8), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR( 9), MY66000_MEM,   opc_op1, MINOR_MASK, MINOR_OFFS},
 { NULL,   MAJOR(10), MY66000_BAD,   opc_op2, MINOR_MASK, MINOR_OFFS},
 { NULL,   MAJOR(11), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(12), MY66000_BAD,   opc_op4, XOP4_MASK, XOP4_OFFS},
 { NULL,   MAJOR(13), MY66000_BAD,   opc_op5, MINOR_MASK, MINOR_OFFS},
 { NULL,   MAJOR(14), MY66000_BAD,   NULL, 0, 0},
 { "ill1", MAJOR(15), MY66000_ILL,   NULL, 0, 0},
 { "ill2", MAJOR(16), MY66000_ILL,   NULL, 0, 0},
 { NULL,   MAJOR(17), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(18), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(19), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(20), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(21), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(22), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(23), MY66000_BAD,   NULL, 0, 0},
 { "bb1",  MAJOR(24), MY66000_BB1,   opc_bb1a, BB1_MASK,  BB1_OFFS},
 { "bb1",  MAJOR(25), MY66000_BB1 ,  opc_bb1b, BB1_MASK,  BB1_OFFS},
 { NULL,   MAJOR(26), MY66000_BAD,   opc_bcnd, CND_MASK, CND_OFFS},
 { NULL,   MAJOR(27), MY66000_BAD,   opc_tt,   TT_MASK,   TT_OFFS},
 { NULL,   MAJOR(28), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(29), MY66000_BAD,   opc_loop, SIGNED_MASK, SIGNED_SHFT},
 { "br",   MAJOR(30), MY66000_BR,    NULL, 0, 0},
 { "call", MAJOR(31), MY66000_BR,    NULL, 0, 0},
 { "ldub", MAJOR(32), MY66000_MEM,   NULL, 0, 0},
 { "lduh", MAJOR(33), MY66000_MEM,   NULL, 0, 0},
 { "lduw", MAJOR(34), MY66000_MEM,   NULL, 0, 0},
 { "ldd",  MAJOR(35), MY66000_MEM,   NULL, 0, 0},
 { "ldsb", MAJOR(36), MY66000_MEM,   NULL, 0, 0},
 { "ldsh", MAJOR(37), MY66000_MEM,   NULL, 0, 0},
 { "ldsw", MAJOR(38), MY66000_MEM,   NULL, 0, 0},
 { "exit", MAJOR(39), MY66000_ENTER, NULL, 0, 0},
 { "stb",  MAJOR(40), MY66000_MEM,   NULL, 0, 0},
 { "sth",  MAJOR(41), MY66000_MEM,   NULL, 0, 0},
 { "stw",  MAJOR(42), MY66000_MEM,   NULL, 0, 0},
 { "std",  MAJOR(43), MY66000_MEM,   NULL, 0, 0},
 { "enter", MAJOR(44), MY66000_ENTER, NULL, 0, 0},
 { "ldm",  MAJOR(45), MY66000_MM,    NULL, 0, 0},
 { "stm",  MAJOR(46), MY66000_MM,    NULL, 0, 0},
 { "ill3", MAJOR(47), MY66000_ILL,   NULL, 0, 0},
 { "ill4", MAJOR(48), MY66000_ILL,   NULL, 0, 0},
 { "add",  MAJOR(49), MY66000_OPIMM, NULL, 0, 0},
 { "mul",  MAJOR(50), MY66000_OPIMM, NULL, 0, 0},
 { "div",  MAJOR(51), MY66000_OPIMM, NULL, 0, 0},
 { "cmp",  MAJOR(52), MY66000_OPIMM, NULL, 0, 0},
 { NULL,   MAJOR(53), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(54), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(55), MY66000_BAD,   NULL, 0, 0},
 { "or",   MAJOR(56), MY66000_OPIMM, opc_info_nop, 0, 0},
 { "xor",  MAJOR(57), MY66000_OPIMM, NULL, 0, 0},
 { "and",  MAJOR(58), MY66000_OPIMM, NULL, 0, 0},
 { "mov",  MAJOR(59), MY66000_MVIMM, NULL, 0, 0},
 { NULL,   MAJOR(60), MY66000_CARRY, opc_mod, MOD_MASK, MOD_OFFS},
 { "vec",  MAJOR(61), MY66000_VEC,   NULL, 0, 0},
 { NULL,   MAJOR(62), MY66000_BAD,   NULL, 0, 0},
 { "ill5", MAJOR(63), MY66000_ILL,   NULL, 0, 0},
 { NULL,   0,         MY66000_END,   NULL, 0, 0},
};

static const my66000_opc_info_t opc_om6[] =
{
 { "pb1",  MAJOR(6) | SHFT_MINOR( 0), MY66000_PB1,  opc_pb1a, BB1_MASK, BB1_OFFS},
 { "pcnd", MAJOR(6) | SHFT_MINOR( 1), MY66000_BAD,  opc_pcnd, CND_MASK, CND_OFFS},
 { NULL,   MAJOR(6) | SHFT_MINOR( 2), MY66000_BAD,  NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR( 3), MY66000_BAD,  NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR( 4), MY66000_BAD,  NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR( 5), MY66000_BAD,  NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR( 6), MY66000_BAD,  NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR( 7), MY66000_BAD,  NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR( 8), MY66000_BAD,  NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR( 9), MY66000_BAD,  NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR(10), MY66000_BAD,  NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR(11), MY66000_BAD,  NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR(12), MY66000_BAD,  NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR(13), MY66000_BAD,  NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR(14), MY66000_BAD,  NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR(15), MY66000_BAD,  NULL, 0, 0},
 { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

static const my66000_opc_info_t opc_om7[] =
{
 { "pb1",  MAJOR(7) | SHFT_MINOR( 0), MY66000_PB1,   opc_pb1b, BB1_MASK, BB1_OFFS},
 { NULL,   MAJOR(7) | SHFT_MINOR( 1), MY66000_PCND,  NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR( 2), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR( 3), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR( 4), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR( 5), MY66000_BAD,   NULL, 0, 0},
 { "ror",  MAJOR(7) | SHFT_MINOR( 6), MY66000_SHIFT, NULL, 0, 0},
 { "rol",  MAJOR(7) | SHFT_MINOR( 7), MY66000_SHIFT, NULL, 0, 0},
 { "srl",  MAJOR(7) | SHFT_MINOR( 8), MY66000_SHIFT, NULL, 0, 0},
 { "sra",  MAJOR(7) | SHFT_MINOR( 9), MY66000_SHIFT, NULL, 0, 0},
 { "sll",  MAJOR(7) | SHFT_MINOR(10), MY66000_SHIFT, NULL, 0, 0},
 { "sla",  MAJOR(7) | SHFT_MINOR(11), MY66000_SHIFT, NULL, 0, 0},
 { "bitr", MAJOR(7) | SHFT_MINOR(12), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR(13), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR(14), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR(15), MY66000_BAD,   NULL, 0, 0},
 { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

/* Definition for the various modifier bits in the XOP1 group,
   including scale.  */

#define XOP1_D_SHFT 15
#define XOP1_D(c) ((c) << XOP1_D_SHFT)
#define XOP1_D_MASK XOP1_D(1)

#define XOP1_SCALE_SHFT 13
#define XOP1_SCALE(c) ((c) << XOP1_SCALE_SHFT)
#define XOP1_SCALE_MASK XOP1_SCALE(3)

#define XOP1_L_SHFT 12
#define XOP1_L(c) ((c) << XOP1_L_SHFT)
#define XOP1_L_MASK XOP1_L(1)

#define XOP1_d_SHFT 11
#define XOP1_d(c) ((c) << XOP1_d_SHFT)
#define XOP1_d_MASK XOP1_d(1)

#define MRR_FMT_MASK (XOP1_D_MASK | XOP1_d_MASK)

/* Mask for rindex=0 */

#define RIND_ZERO_MASK 31

/* Mask for DST=0.  */
#define DST_MASK (31u << 21)

/* This is for reaching the JMP and CALLI instructions.  */

#define JMPCALL_MASK 7
#define JMPCALL_OFFS 0

/* Mask for instruction pointer as SRC1.  */

#define IP_MASK (31 << 16)

#define XOP1_BITS(D,d) (XOP1_D(D) | XOP1_d(d))

/* Definition for the various modifier bits in the XOP2 group.  */

#define XOP2_I_SHFT 15
#define XOP2_I(c) ((c) << XOP2_I_SHFT)
#define XOP2_I_MASK XOP2_I(1)

#define XOP2_S1_SHFT 14
#define XOP2_S1(c) ((c) << XOP2_S1_SHFT)
#define XOP2_S1_MASK XOP2_S1(1)

#define XOP5_S1(c) XOP2_S1(c)
#define XOP5_S1_MASK XOP2_S1_MASK

#define XOP2_S2_SHFT 13
#define XOP2_S2(c) ((c) << XOP2_S2_SHFT)
#define XOP2_S2_MASK XOP2_S2(1)

/* XOP5 reuses the S2 bit of XOP2.  */

#define XOP5_SD(c) XOP2_S2(c)
#define XOP5_SD_MASK XOP2_S2_MASK

#define XOP5_MASK (7<<13)

#define XOP5_I(c) XOP2_I(c)
#define XOP5_I_MASK XOP2_I_MASK

/* The S bit, at position 12, is already used in the opcode tables
   above.  */

#define XOP2_d_SHFT 11
#define XOP2_d(c) ((c) << XOP2_d_SHFT)
#define XOP2_d_MASK XOP2_d(1)

/* And we recyle the d bit from XOP2.  */

#define XOP5_d(c) XOP2_d(c)

#define XOP2_MASK (XOP2_I(1) | XOP2_S1(1) | XOP2_S2(1) | XOP2_d(1))

#define XOP5_S_SHFT 12
#define XOP5_S(c) ((c) << XOP5_S_SHFT)
#define XOP5_S_MASK XOP5_S(1)

/* Write the above more compactly.  */

#define XOP2_BITS(I,d,S1,S2) (XOP2_I(I) | XOP2_d(d) | XOP2_S1(S1) | XOP2_S2(S2))

#define XOP5_BITS(I,d,S1,SD) (XOP5_I(I) | XOP5_d(d) | XOP5_S1(S1) | XOP5_SD(SD))

#define XOP5_FMT_MASK XOP5_BITS(1,1,1,1)

#define HR_FUNCTION_MASK 31

/* Make sure this agrees with the position of ip in my66000_hr_fcn.  */

#define HR_FCN_IP 30

/* For HR, the I and d bits need to be checked, S is always zero, and
   the S1 and SD bits double as R and W.  */

#define HR_FMT_MASK XOP5_BITS(1,1,0,0) | XOP5_S_MASK
#define HR_BITS(I,d) (XOP5_I(I) | XOP5_d(d))

#define HR_RW_OFFS 13
#define HR_RW(c) ((c) << HR_RW_OFFS)
#define HR_RW_MASK HR_RW(3)

/* FMAC uses some of same bits as XOP2.  */

#define XOP4_BITS(I,S1,S2) (XOP2_I(I) | XOP2_S1(S1) | XOP2_S2(S2))
#define XOP4_FMT_MASK XOP4_BITS (1,1,1)
#define XOP4_FMT_SHFT 13

#define ENTER_MASK 7

/* We use masking for the predicate field lengths. There are four
   valid cases:

   then = 0..7
   then = 8
   else = 0..7
   else = 8

*/

#define THEN_FMT_MASK_7 (((1u<<6)-1) ^ 7)
#define THEN_FMT_MASK_8 (((1u<<6)-1) ^ 8)
#define ELSE_FMT_MASK_7 (THEN_FMT_MASK_7 << 6)
#define ELSE_FMT_MASK_8 (THEN_FMT_MASK_8 << 6)

#define PRED_FMT_MASK ((1<<12)-1)

/* We use a single table here and index in here from opc_op1.  It would
   be too many tables otherwise.  This would be easier if the L bit
   was adjacent to the opcode.

   Unlike some other my66000_opc_info_t tables, we can order this one
   arbitrarily.  We (ab)use this by not filling it completely, and by
   rearraning the order when it suits us.  */

static const my66000_opc_info_t opc_mrr[] =
{
 { "ldub",  MAJOR(9) | MINOR( 0) | XOP1_L(0), MY66000_MRR, NULL, 0, 0},  // +0
 { "ldubl", MAJOR(9) | MINOR( 0) | XOP1_L(1), MY66000_MRR, NULL, 0, 0},
 { "lduh",  MAJOR(9) | MINOR( 1) | XOP1_L(0), MY66000_MRR, NULL, 0, 0},  // +2
 { "lduhl", MAJOR(9) | MINOR( 1) | XOP1_L(1), MY66000_MRR, NULL, 0, 0},
 { "lduw",  MAJOR(9) | MINOR( 2) | XOP1_L(0), MY66000_MRR, NULL, 0, 0},  // +4
 { "lduwl", MAJOR(9) | MINOR( 2) | XOP1_L(1), MY66000_MRR, NULL, 0, 0},
 { "ldd",   MAJOR(9) | MINOR( 3) | XOP1_L(0), MY66000_MRR, NULL, 0, 0},  // +6
 { "lddl" , MAJOR(9) | MINOR( 3) | XOP1_L(1), MY66000_MRR, NULL, 0, 0},
 { "ldsb",  MAJOR(9) | MINOR( 4) | XOP1_L(0), MY66000_MRR, NULL, 0, 0},  // +8
 { "ldsbl", MAJOR(9) | MINOR( 4) | XOP1_L(1), MY66000_MRR, NULL, 0, 0},
 { "ldsh",  MAJOR(9) | MINOR( 5) | XOP1_L(0), MY66000_MRR, NULL, 0, 0},  // +10
 { "ldshl", MAJOR(9) | MINOR( 5) | XOP1_L(1), MY66000_MRR, NULL, 0, 0},
 { "ldsw",  MAJOR(9) | MINOR( 6) | XOP1_L(0), MY66000_MRR, NULL, 0, 0},  // +12
 { "ldswl", MAJOR(9) | MINOR( 6) | XOP1_L(1), MY66000_MRR, NULL, 0, 0},
 { "la",    MAJOR(9) | MINOR( 7) | XOP1_L(0), MY66000_MRR, NULL, 0, 0}, // +14
 { "lal",   MAJOR(9) | MINOR( 7) | XOP1_L(1), MY66000_MRR, NULL, 0, 0},
 { "stb",   MAJOR(9) | MINOR( 8) | XOP1_L(0), MY66000_MRR, NULL, 0, 0}, // +16
 { "stbl" , MAJOR(9) | MINOR( 8) | XOP1_L(1), MY66000_MRR, NULL, 0, 0},
 { "sth",   MAJOR(9) | MINOR( 9) | XOP1_L(0), MY66000_MRR, NULL, 0, 0}, // +18
 { "sthl" , MAJOR(9) | MINOR( 9) | XOP1_L(1), MY66000_MRR, NULL, 0, 0},
 { "stw",   MAJOR(9) | MINOR(10) | XOP1_L(0), MY66000_MRR, NULL, 0, 0}, // +20
 { "stwl" , MAJOR(9) | MINOR(10) | XOP1_L(1), MY66000_MRR, NULL, 0, 0},
 { "std",   MAJOR(9) | MINOR(11) | XOP1_L(0), MY66000_MRR, NULL, 0, 0}, // +22
 { "stdl" , MAJOR(9) | MINOR(11) | XOP1_L(1), MY66000_MRR, NULL, 0, 0},

 /* We put the five-bit immediate stores before the others because we want
    to find them first.  */

 { "stb",   MAJOR(9) | MINOR(28) | XOP1_L(0), MY66000_SI5,  NULL, 0, 0}, // +24
 { "stbl",  MAJOR(9) | MINOR(28) | XOP1_L(1), MY66000_SI5,  NULL, 0, 0},
 { "sth",   MAJOR(9) | MINOR(29) | XOP1_L(0), MY66000_SI5,  NULL, 0, 0}, // +26
 { "sthl",  MAJOR(9) | MINOR(29) | XOP1_L(1), MY66000_SI5,  NULL, 0, 0},
 { "stw",   MAJOR(9) | MINOR(30) | XOP1_L(0), MY66000_SI5,  NULL, 0, 0}, // +28
 { "stwl",  MAJOR(9) | MINOR(30) | XOP1_L(1), MY66000_SI5,  NULL, 0, 0},
 { "std",   MAJOR(9) | MINOR(31) | XOP1_L(0), MY66000_SI5,  NULL, 0, 0}, // +30
 { "stdl",  MAJOR(9) | MINOR(31) | XOP1_L(1), MY66000_SI5,  NULL, 0, 0},

 { "stb",   MAJOR(9) | MINOR(24) | XOP1_L(0), MY66000_SI,  NULL, 0, 0}, // +32
 { "stbl",  MAJOR(9) | MINOR(24) | XOP1_L(1), MY66000_SI,  NULL, 0, 0},
 { "sth",   MAJOR(9) | MINOR(25) | XOP1_L(0), MY66000_SI,  NULL, 0, 0}, // +34
 { "sthl",  MAJOR(9) | MINOR(25) | XOP1_L(1), MY66000_SI,  NULL, 0, 0},
 { "stw",   MAJOR(9) | MINOR(26) | XOP1_L(0), MY66000_SI,  NULL, 0, 0}, // +36
 { "stwl",  MAJOR(9) | MINOR(26) | XOP1_L(1), MY66000_SI,  NULL, 0, 0},
 { "std",   MAJOR(9) | MINOR(27) | XOP1_L(0), MY66000_SI_STD, NULL, 0, 0}, // +38
 { "stdl",  MAJOR(9) | MINOR(27) | XOP1_L(1), MY66000_SI_STD, NULL, 0, 0},

 { NULL,    0, 0, NULL, 0, 0},
 { NULL,    0, 0, NULL, 0, 0},

 { NULL,   0,        MY66000_END, NULL, 0, 0}
};

static const my66000_opc_info_t opc_op1[] =
{
 { NULL,   MAJOR(9) | MINOR ( 0), MY66000_BAD, opc_mrr + 0, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR ( 1), MY66000_BAD, opc_mrr + 2, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR ( 2), MY66000_BAD, opc_mrr + 4, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR ( 3), MY66000_BAD, opc_mrr + 6, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR ( 4), MY66000_BAD, opc_mrr + 8, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR ( 5), MY66000_BAD, opc_mrr +10, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR ( 6), MY66000_BAD, opc_mrr +12, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR ( 7), MY66000_BAD, opc_mrr +14, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR ( 8), MY66000_BAD, opc_mrr +16, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR ( 9), MY66000_BAD, opc_mrr +18, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR (10), MY66000_BAD, opc_mrr +20, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR (11), MY66000_BAD, opc_mrr +22, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR (12), MY66000_BAD, NULL, 0, 0},
 { "pre",  MAJOR(9) | MINOR (13), MY66000_MRR, NULL, 0, 0},
 { "push", MAJOR(9) | MINOR (14), MY66000_MRR, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (15), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (16), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (17), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (18), MY66000_BAD, NULL, 0, 0},
 { "ldm",  MAJOR(9) | MINOR (19), MY66000_MM,  NULL, 0, 0},
 { "stm",  MAJOR(9) | MINOR (20), MY66000_MM,  NULL, 0, 0},
 { "mm",   MAJOR(9) | MINOR (21), MY66000_MM,  NULL, 0, 0},
 { "ms",   MAJOR(9) | MINOR (22), MY66000_MM,  NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (23), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (24), MY66000_BAD, opc_mrr + 32, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR (25), MY66000_BAD, opc_mrr + 34, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR (26), MY66000_BAD, opc_mrr + 36, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR (27), MY66000_BAD, opc_mrr + 38, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR (28), MY66000_BAD, opc_mrr + 24, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR (29), MY66000_BAD, opc_mrr + 26, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR (30), MY66000_BAD, opc_mrr + 28, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR (31), MY66000_BAD, opc_mrr + 30, XOP1_L_MASK, XOP1_L_SHFT},
 { NULL,   MAJOR(9) | MINOR (32), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (33), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (34), MY66000_BAD, NULL, 0, 0},
 { "calx", MAJOR(9) | MINOR (35), MY66000_CALX, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (36), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (37), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (38), MY66000_BAD, NULL, 0, 0},
 { "cala", MAJOR(9) | MINOR (39), MY66000_CALX, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (40), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (41), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (42), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (43), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (44), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (45), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (46), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (47), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (48), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (49), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (50), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (51), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (52), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (53), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (54), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (55), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (56), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (57), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (58), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (59), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (60), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (61), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (62), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (63), MY66000_BAD, NULL, 0, 0},
 { NULL,  0,              MY66000_END,   NULL, 0, 0}
};


/* This table is indexed with only the S bit, so we use offsets into
   the middle of the table.  */

static const my66000_opc_info_t opc_arith[] =
{
 {"add",  MAJOR(10) | MINOR(33) | SIGNED(0), MY66000_ARITH, NULL, 0, 0},  // + 0
 {"adds", MAJOR(10) | MINOR(33) | SIGNED(1), MY66000_ARITH, NULL, 0, 0},
 {"mul",  MAJOR(10) | MINOR(34) | SIGNED(0), MY66000_ARITH, NULL, 0, 0},  // + 2
 {"muls", MAJOR(10) | MINOR(34) | SIGNED(1), MY66000_ARITH, NULL, 0, 0},
 {"div",  MAJOR(10) | MINOR(35) | SIGNED(0), MY66000_ARITH, NULL, 0, 0},  // + 4
 {"divs", MAJOR(10) | MINOR(35) | SIGNED(1), MY66000_ARITH, NULL, 0, 0},
 {"cmp",  MAJOR(10) | MINOR(36) | SIGNED(0), MY66000_ARITH, NULL, 0, 0},  // + 6
 {"cmps", MAJOR(10) | MINOR(36) | SIGNED(1), MY66000_ARITH, NULL, 0, 0},

 {"max",  MAJOR(10) | MINOR(38) | SIGNED(0), MY66000_ARITH, NULL, 0, 0},  // + 8
 {"maxs", MAJOR(10) | MINOR(38) | SIGNED(1), MY66000_ARITH, NULL, 0, 0},
 {"min",  MAJOR(10) | MINOR(39) | SIGNED(0), MY66000_ARITH, NULL, 0, 0},  // + 10
 {"mins", MAJOR(10) | MINOR(39) | SIGNED(1), MY66000_ARITH, NULL, 0, 0},

 {"srl",  MAJOR(10) | MINOR(44) | SIGNED(0), MY66000_ARITH, NULL, 0, 0}, // + 12
 {"sra",  MAJOR(10) | MINOR(44) | SIGNED(1), MY66000_ARITH, NULL, 0, 0},
 {"sll",  MAJOR(10) | MINOR(45) | SIGNED(0), MY66000_ARITH, NULL, 0, 0}, // + 14
 {"sla",  MAJOR(10) | MINOR(45) | SIGNED(1), MY66000_ARITH, NULL, 0, 0},
 {"ror",  MAJOR(10) | MINOR(43) | SIGNED(0), MY66000_ARITH, NULL, 0, 0}, // +16
 {"rol",  MAJOR(10) | MINOR(43) | SIGNED(1), MY66000_ARITH, NULL, 0, 0},
 { NULL,   0,        MY66000_END, NULL, 0, 0}
};

/* Likewise for here.  */
static const my66000_opc_info_t opc_float[] =
{
 {"fadd",  MAJOR(10) | MINOR(49) | SIGNED(0), MY66000_FLOAT, NULL, 0, 0}, // + 0
 {"faddf", MAJOR(10) | MINOR(49) | SIGNED(1), MY66000_FLOAT, NULL, 0, 0},
 {"fmul",  MAJOR(10) | MINOR(50) | SIGNED(0), MY66000_FLOAT, NULL, 0, 0}, // + 2
 {"fmulf", MAJOR(10) | MINOR(50) | SIGNED(1), MY66000_FLOAT, NULL, 0, 0},
 {"fdiv",  MAJOR(10) | MINOR(51) | SIGNED(0), MY66000_FLOAT, NULL, 0, 0}, // + 4
 {"fdivf", MAJOR(10) | MINOR(51) | SIGNED(1), MY66000_FLOAT, NULL, 0, 0},
 {"fcmp",  MAJOR(10) | MINOR(52) | SIGNED(0), MY66000_FLOAT, NULL, 0, 0}, // + 6
 {"fcmpf", MAJOR(10) | MINOR(52) | SIGNED(1), MY66000_FLOAT, NULL, 0, 0},
 {"fmax",  MAJOR(10) | MINOR(54) | SIGNED(0), MY66000_FLOAT, NULL, 0, 0}, // + 8
 {"fmaxf", MAJOR(10) | MINOR(54) | SIGNED(1), MY66000_FLOAT, NULL, 0, 0},
 {"fmin",  MAJOR(10) | MINOR(55) | SIGNED(0), MY66000_FLOAT, NULL, 0, 0}, // + 10
 {"fminf", MAJOR(10) | MINOR(55) | SIGNED(1), MY66000_FLOAT, NULL, 0, 0},
 {"csgn",  MAJOR(10) | MINOR(57) | SIGNED(0), MY66000_FLOAT, NULL, 0, 0}, // + 12
 {"csgnf", MAJOR(10) | MINOR(57) | SIGNED(1), MY66000_FLOAT, NULL, 0, 0},
 {"eadd",  MAJOR(10) | MINOR(58) | SIGNED(0), MY66000_EADD,  NULL, 0, 0}, // + 14
 {"eaddf", MAJOR(10) | MINOR(58) | SIGNED(1), MY66000_EADD,  NULL, 0, 0},
 {"pow",   MAJOR(10) | MINOR(61) | SIGNED(0), MY66000_FLOAT, NULL, 0, 0}, // + 16
 {"powf" , MAJOR(10) | MINOR(61) | SIGNED(1), MY66000_FLOAT, NULL, 0, 0},
 {"atan2", MAJOR(10) | MINOR(62) | SIGNED(0), MY66000_FLOAT, NULL, 0, 0}, // + 18
 {"atan2f",MAJOR(10) | MINOR(62) | SIGNED(1), MY66000_FLOAT, NULL, 0, 0},
  { NULL,   0,        MY66000_END, NULL, 0, 0}
};

/* ... and here.  */
static const my66000_opc_info_t opc_cvt[] =
{
  { "cvtus", MAJOR(10) | MINOR( 1) | SIGNED(0), MY66000_CVTU, NULL, 0, 0},
  { "cvtsu", MAJOR(10) | MINOR( 1) | SIGNED(1), MY66000_CVTS, NULL, 0, 0}, // + 0
  { "cvtsd", MAJOR(10) | MINOR( 2) | SIGNED(0), MY66000_CVTS, NULL, 0, 0}, // + 2
  { "cvtsf", MAJOR(10) | MINOR( 2) | SIGNED(1), MY66000_CVTS, NULL, 0, 0},
  { "cvtud", MAJOR(10) | MINOR( 3) | SIGNED(0), MY66000_CVTU, NULL, 0, 0}, // + 4
  { "cvtuf", MAJOR(10) | MINOR( 3) | SIGNED(1), MY66000_CVTU, NULL, 0, 0},
  { "cvtdu", MAJOR(10) | MINOR(17) | SIGNED(0), MY66000_CVTU, NULL, 0, 0}, // + 6
  { "cvtds", MAJOR(10) | MINOR(17) | SIGNED(1), MY66000_CVTU, NULL, 0, 0},
  { "cvtfu", MAJOR(10) | MINOR(18) | SIGNED(0), MY66000_CVTU, NULL, 0, 0}, // + 8
  { "cvtfs", MAJOR(10) | MINOR(18) | SIGNED(1), MY66000_CVTU, NULL, 0, 0},
  { "cvtdf", MAJOR(10) | MINOR(19) | SIGNED(0), MY66000_CVTU, NULL, 0, 0}, // +10
  { "cvtfd", MAJOR(10) | MINOR(19) | SIGNED(1), MY66000_CVTU, NULL, 0, 0},
  { "rnd",   MAJOR(10) | MINOR(20) | SIGNED(0), MY66000_CVTU, NULL, 0, 0}, // +12
  { "rndf",  MAJOR(10) | MINOR(20) | SIGNED(1), MY66000_CVTU, NULL, 0, 0},
  { NULL,   0,        MY66000_END, NULL, 0, 0}
};

static const my66000_opc_info_t opc_op2[] =
{
 { NULL,  MAJOR(10) | MINOR ( 0), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR ( 1), MY66000_BAD,   opc_cvt + 0, SIGNED_MASK, SIGNED_SHFT},
 { NULL,  MAJOR(10) | MINOR ( 2), MY66000_BAD,   opc_cvt + 2, SIGNED_MASK, SIGNED_SHFT},
 { NULL,  MAJOR(10) | MINOR ( 3), MY66000_BAD,   opc_cvt + 4, SIGNED_MASK, SIGNED_SHFT},
 { NULL,  MAJOR(10) | MINOR ( 4), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR ( 5), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR ( 6), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR ( 7), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR ( 8), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR ( 9), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (10), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (11), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (12), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (13), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (14), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (15), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (16), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (17), MY66000_BAD,   opc_cvt + 6, SIGNED_MASK, SIGNED_SHFT},
 { NULL,  MAJOR(10) | MINOR (18), MY66000_BAD,   opc_cvt + 8, SIGNED_MASK, SIGNED_SHFT},
 { NULL,  MAJOR(10) | MINOR (19), MY66000_BAD,   opc_cvt +10, SIGNED_MASK, SIGNED_SHFT},
 { NULL,  MAJOR(10) | MINOR (20), MY66000_BAD,   opc_cvt +12, SIGNED_MASK, SIGNED_SHFT},
 { NULL,  MAJOR(10) | MINOR (21), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (22), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (23), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (24), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (25), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (26), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (27), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (28), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (29), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (30), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (31), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (32), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (33), MY66000_BAD,   opc_arith + 0, SIGNED_MASK, SIGNED_SHFT}, /* ADD */
 { NULL,  MAJOR(10) | MINOR (34), MY66000_BAD,   opc_arith + 2, SIGNED_MASK, SIGNED_SHFT}, /* MUL */
 { NULL,  MAJOR(10) | MINOR (35), MY66000_BAD,   opc_arith + 4, SIGNED_MASK, SIGNED_SHFT}, /* DIV */
 { NULL,  MAJOR(10) | MINOR (36), MY66000_BAD,   opc_arith + 6, SIGNED_MASK, SIGNED_SHFT}, /* CMP */
 { NULL,  MAJOR(10) | MINOR (37), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (38), MY66000_BAD,   opc_arith + 8, SIGNED_MASK, SIGNED_SHFT}, /* MAX */
 { NULL,  MAJOR(10) | MINOR (39), MY66000_BAD,   opc_arith +10, SIGNED_MASK, SIGNED_SHFT}, /* MIN */
 { "or",  MAJOR(10) | MINOR (40), MY66000_ARITH, NULL, 0, 0},
 { "xor", MAJOR(10) | MINOR (41), MY66000_ARITH, NULL, 0, 0},
 { "and", MAJOR(10) | MINOR (42), MY66000_ARITH, NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (43), MY66000_BAD,   opc_arith +16, SIGNED_MASK, SIGNED_SHFT},/* ROT */
 { NULL,  MAJOR(10) | MINOR (44), MY66000_BAD,   opc_arith +12, SIGNED_MASK, SIGNED_SHFT}, /* SR */
 { NULL,  MAJOR(10) | MINOR (45), MY66000_BAD,   opc_arith +14, SIGNED_MASK, SIGNED_SHFT}, /* SL */
 { "bitr",MAJOR(10) | MINOR (46), MY66000_ARITH, NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (47), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (48), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (49), MY66000_BAD,   opc_float + 0, SIGNED_MASK, SIGNED_SHFT}, /* FADD */
 { NULL,  MAJOR(10) | MINOR (50), MY66000_BAD,   opc_float + 2, SIGNED_MASK, SIGNED_SHFT}, /* FMUL */
 { NULL,  MAJOR(10) | MINOR (51), MY66000_BAD,   opc_float + 4, SIGNED_MASK, SIGNED_SHFT}, /* FDIV */
 { NULL,  MAJOR(10) | MINOR (52), MY66000_BAD,   opc_float + 6, SIGNED_MASK, SIGNED_SHFT}, /* FCMP */
 { NULL,  MAJOR(10) | MINOR (53), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (54), MY66000_BAD,   opc_float + 8, SIGNED_MASK, SIGNED_SHFT}, /* FMAX */
 { NULL,  MAJOR(10) | MINOR (55), MY66000_BAD,   opc_float +10, SIGNED_MASK, SIGNED_SHFT}, /* FMIN */
 { NULL,  MAJOR(10) | MINOR (56), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (57), MY66000_BAD,   opc_float +12, SIGNED_MASK, SIGNED_SHFT}, /* CSGN */
 { NULL,  MAJOR(10) | MINOR (58), MY66000_BAD,   opc_float +14, SIGNED_MASK, SIGNED_SHFT}, /* EADD */
 { NULL,  MAJOR(10) | MINOR (59), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (60), MY66000_BAD,   NULL, 0, 0},
 { NULL,  MAJOR(10) | MINOR (61), MY66000_BAD,   opc_float +16, SIGNED_MASK, SIGNED_SHFT}, /* POW */
 { NULL,  MAJOR(10) | MINOR (62), MY66000_BAD,   opc_float +18, SIGNED_MASK, SIGNED_SHFT}, /* ATAN2 */
 { NULL,  MAJOR(10) | MINOR (63), MY66000_BAD,   NULL, 0, 0},
 { NULL,    0,          MY66000_END,  NULL, 0, 0}
};

static const my66000_opc_info_t opc_mpx[] =
{
 { "mux",  MAJOR (12) | XOP4_MINOR(1) | XOP4_BITS (0,0,0), MY66000_MUX, NULL, 0, 0},
 { "cmov", MAJOR (12) | XOP4_MINOR(1) | XOP4_BITS (0,0,1), MY66000_MUX, NULL, 0, 0},
 { NULL,   MAJOR (12) | XOP4_MINOR(1) | XOP4_BITS (0,1,0), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR (12) | XOP4_MINOR(1) | XOP4_BITS (0,1,1), MY66000_MOV3, NULL, 0, 0},
 { "mux",  MAJOR (12) | XOP4_MINOR(1) | XOP4_BITS (1,0,0), MY66000_MUX32, NULL, 0, 0},
 { NULL,   MAJOR (12) | XOP4_MINOR(1) | XOP4_BITS (1,0,1), MY66000_MOV32, NULL, 0, 0},
 { "mux",  MAJOR (12) | XOP4_MINOR(1) | XOP4_BITS (1,1,0), MY66000_MUX64, NULL, 0, 0},
 { NULL,   MAJOR (12) | XOP4_MINOR(1) | XOP4_BITS (1,1,1), MY66000_MOV64, NULL, 0, 0},
 { NULL,   0,                                              MY66000_END, NULL, 0, 0},
};

static const my66000_opc_info_t opc_op4[] =
{
 { "fmac",   MAJOR (12) | XOP4_MINOR(0), MY66000_FMAC, NULL, 0, 0},
 { NULL,     MAJOR (12) | XOP4_MINOR(1), MY66000_BAD,  opc_mpx, XOP4_FMT_MASK, XOP4_FMT_SHFT },  /* MPX */
 { "ins",    MAJOR (12) | XOP4_MINOR(2), MY66000_INS,  NULL, 0, 0},  /* INS */
 { NULL,     MAJOR (12) | XOP4_MINOR(3), MY66000_BAD,  NULL, 0, 0},  /* empty */
 { "fmacf",  MAJOR (12) | XOP4_MINOR(4), MY66000_FMAC, NULL, 0, 0},
 { NULL,     MAJOR (12) | XOP4_MINOR(5), MY66000_BAD,  NULL, 0, 0},  /* empty */
 { NULL,     MAJOR (12) | XOP4_MINOR(6), MY66000_BAD,  NULL, 0, 0},  /* empty */
 { NULL,     MAJOR (12) | XOP4_MINOR(7), MY66000_FMAC, NULL, 0, 0},  /* empty */
 { NULL,   0,              MY66000_END,   NULL, 0, 0},
};

/* Dept. of dirty tricks: usually the mask is used to switch between
   different formats on disassembly. In opc_hr[], we set it to zero so
   we always look downwards, to find jmp or call; matching of the
   function code is then done via the format.  */

static const my66000_opc_info_t opc_jmp[] =
{
  {"jmp", MAJOR(13) | MINOR(0) | HR_RW(2) | HR_FCN_IP, MY66000_JMP, NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
};

static const my66000_opc_info_t opc_calli[] =
{
  {"calli", MAJOR(13) | MINOR(0) | HR_RW(3) | HR_FCN_IP, MY66000_JMP, NULL, 0, 0},
 { NULL,   0,              MY66000_END,   NULL, 0, 0},
};


static const my66000_opc_info_t opc_hr[] =
{
  {NULL,  MAJOR(13) | MINOR(0) | HR_RW(0), MY66000_BAD, NULL, 0, 0},
  {"hrr", MAJOR(13) | MINOR(0) | HR_RW(1), MY66000_HRR, NULL, 0, 0},
  {"hrw", MAJOR(13) | MINOR(0) | HR_RW(2), MY66000_HRW, opc_jmp, 0, 0},
  {"hrx", MAJOR(13) | MINOR(0) | HR_RW(3), MY66000_HRX, opc_calli, 0, 0},
 { NULL,   0,              MY66000_END,   NULL, 0, 0},
};

/* All the ABS versions.  */

/* Only two XOP5 functions are valid - 0 and 1.  We do the rest via
     the mask of the format.  The alternative would be a table with 64
     entries, only two of which would be valid.  */

static const my66000_opc_info_t opc_abs[] =
{
  {NULL, MAJOR(13) | MINOR(5) | SIGNED(0), MY66000_BAD, opc_abs + 2, XOP5_ABS_MASK, XOP5_OFFS}, // + 0
  {NULL, MAJOR(13) | MINOR(5) | SIGNED(1), MY66000_BAD, opc_abs + 4, XOP5_ABS_MASK, XOP5_OFFS}, // + 1

  /* S = 0.  */
  {"abs",   MAJOR(13) | MINOR(5) | SIGNED(0) | XOP5_FCN(0), MY66000_ABS, NULL, 0, 0}, // + 2
  {"fabs",  MAJOR(13) | MINOR(5) | SIGNED(0) | XOP5_FCN(1), MY66000_ABS, NULL, 0, 0},
  /* S = 1.  */
  {"mov",   MAJOR(13) | MINOR(5) | SIGNED(1) | XOP5_FCN(0), MY66000_ABS, NULL, 0, 0}, // + 4
  {"fabsf", MAJOR(13) | MINOR(5) | SIGNED(1) | XOP5_FCN(1), MY66000_ABS, NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
};

static const my66000_opc_info_t opc_single[] =
{
  {"expon",  MAJOR(13) | MINOR(6) | SIGNED(0), MY66000_OP5_D, NULL, 0, 0},  // + 0
  {"exponf", MAJOR(13) | MINOR(6) | SIGNED(1), MY66000_OP5_F, NULL, 0, 0},
  {"fract",  MAJOR(13) | MINOR(7) | SIGNED(0), MY66000_OP5_D, NULL, 0, 0}, // + 2
  {"fractf", MAJOR(13) | MINOR(7) | SIGNED(1), MY66000_OP5_F, NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
};

/* XXX  In the ISA, this reads clz and ctz, but the compiler currently emits
 ctlz and cttz.  Stick with the compiler (for  now).  */

static const my66000_opc_info_t opc_ff1[] =
{
  {"ctlz",   MAJOR(13) | MINOR(9) | FF1(0), MY66000_FF1, NULL, 0, 0},
  {"fl1",   MAJOR(13) | MINOR(9) | FF1(1), MY66000_FF1, NULL, 0, 0},
  {"ft1",   MAJOR(13) | MINOR(9) | FF1(2), MY66000_FF1, NULL, 0, 0},
  {"cttz",   MAJOR(13) | MINOR(9) | FF1(3), MY66000_FF1, NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
};

static const my66000_opc_info_t opc_trans[] =
{
  { NULL, MAJOR(13) | MINOR(24) | SIGNED(0), MY66000_BAD, opc_transd, XOP5_FCN_MASK, XOP5_OFFS},
  { NULL, MAJOR(13) | MINOR(24) | SIGNED(1), MY66000_BAD, opc_transf, XOP5_FCN_MASK, XOP5_OFFS},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
};

static const my66000_opc_info_t opc_transd[] =
{
  { "fln2",    MAJOR(13) | MINOR(24) | XOP5_FCN( 0)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { "fln",     MAJOR(13) | MINOR(24) | XOP5_FCN( 1)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { "flog",    MAJOR(13) | MINOR(24) | XOP5_FCN( 2)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { NULL,      MAJOR(13) | MINOR(24) | XOP5_FCN( 3)| SIGNED(0), MY66000_BAD,   NULL, 0, 0},
  { "fexp2",   MAJOR(13) | MINOR(24) | XOP5_FCN( 4)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { "fexp",    MAJOR(13) | MINOR(24) | XOP5_FCN( 5)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { "fexp10",  MAJOR(13) | MINOR(24) | XOP5_FCN( 6)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { NULL,      MAJOR(13) | MINOR(24) | XOP5_FCN( 7)| SIGNED(0), MY66000_BAD,   NULL, 0, 0},
  { "frcp",    MAJOR(13) | MINOR(24) | XOP5_FCN( 8)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { NULL,      MAJOR(13) | MINOR(24) | XOP5_FCN( 9)| SIGNED(0), MY66000_BAD,   NULL, 0, 0},
  { NULL,      MAJOR(13) | MINOR(24) | XOP5_FCN(10)| SIGNED(0), MY66000_BAD,   NULL, 0, 0},
  { NULL,      MAJOR(13) | MINOR(24) | XOP5_FCN(11)| SIGNED(0), MY66000_BAD,   NULL, 0, 0},
  { "frsqrt",  MAJOR(13) | MINOR(24) | XOP5_FCN(12)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { "fsqrt",   MAJOR(13) | MINOR(24) | XOP5_FCN(13)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { NULL,      MAJOR(13) | MINOR(24) | XOP5_FCN(14)| SIGNED(0), MY66000_BAD,   NULL, 0, 0},
  { NULL,      MAJOR(13) | MINOR(24) | XOP5_FCN(15)| SIGNED(0), MY66000_BAD,   NULL, 0, 0},
  { "fln2p1",  MAJOR(13) | MINOR(24) | XOP5_FCN(16)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { "flnp1",   MAJOR(13) | MINOR(24) | XOP5_FCN(17)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { "flogp1",  MAJOR(13) | MINOR(24) | XOP5_FCN(18)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { NULL,      MAJOR(13) | MINOR(24) | XOP5_FCN(19)| SIGNED(0), MY66000_BAD,   NULL, 0, 0},
  { "fexp2m1", MAJOR(13) | MINOR(24) | XOP5_FCN(20)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { "fexpm1",  MAJOR(13) | MINOR(24) | XOP5_FCN(21)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { "fexp10m1",MAJOR(13) | MINOR(24) | XOP5_FCN(22)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { NULL,      MAJOR(13) | MINOR(24) | XOP5_FCN(23)| SIGNED(0), MY66000_BAD,   NULL, 0, 0},
  { "fsin",    MAJOR(13) | MINOR(24) | XOP5_FCN(24)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { "fcos",    MAJOR(13) | MINOR(24) | XOP5_FCN(25)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { "ftan",    MAJOR(13) | MINOR(24) | XOP5_FCN(26)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { NULL,      MAJOR(13) | MINOR(24) | XOP5_FCN(27)| SIGNED(0), MY66000_BAD,   NULL, 0, 0},
  { "fasin",   MAJOR(13) | MINOR(24) | XOP5_FCN(28)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { "facos",   MAJOR(13) | MINOR(24) | XOP5_FCN(29)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { "fatan",   MAJOR(13) | MINOR(24) | XOP5_FCN(30)| SIGNED(0), MY66000_TRANS, NULL, 0, 0},
  { NULL,      MAJOR(13) | MINOR(24) | XOP5_FCN(31)| SIGNED(0), MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
};

static const my66000_opc_info_t opc_transf[] =
{
  { "fln2f",    MAJOR(13) | MINOR(24) | XOP5_FCN( 0)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { "flnf",     MAJOR(13) | MINOR(24) | XOP5_FCN( 1)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { "flogf",    MAJOR(13) | MINOR(24) | XOP5_FCN( 2)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { NULL,       MAJOR(13) | MINOR(24) | XOP5_FCN( 3)| SIGNED(1), MY66000_BAD,   NULL, 0, 0},
  { "fexp2f",   MAJOR(13) | MINOR(24) | XOP5_FCN( 4)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { "fexpf",    MAJOR(13) | MINOR(24) | XOP5_FCN( 5)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { "fexp10f",  MAJOR(13) | MINOR(24) | XOP5_FCN( 6)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { NULL,       MAJOR(13) | MINOR(24) | XOP5_FCN( 7)| SIGNED(1), MY66000_BAD,   NULL, 0, 0},
  { "frcpf",    MAJOR(13) | MINOR(24) | XOP5_FCN( 8)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { NULL,       MAJOR(13) | MINOR(24) | XOP5_FCN( 9)| SIGNED(1), MY66000_BAD,   NULL, 0, 0},
  { NULL,       MAJOR(13) | MINOR(24) | XOP5_FCN(10)| SIGNED(1), MY66000_BAD,   NULL, 0, 0},
  { NULL,       MAJOR(13) | MINOR(24) | XOP5_FCN(11)| SIGNED(1), MY66000_BAD,   NULL, 0, 0},
  { "frsqrtf",  MAJOR(13) | MINOR(24) | XOP5_FCN(12)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { "fsqrtf",   MAJOR(13) | MINOR(24) | XOP5_FCN(13)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { NULL,       MAJOR(13) | MINOR(24) | XOP5_FCN(14)| SIGNED(1), MY66000_BAD,   NULL, 0, 0},
  { NULL,       MAJOR(13) | MINOR(24) | XOP5_FCN(15)| SIGNED(1), MY66000_BAD,   NULL, 0, 0},
  { "fln2p1f",  MAJOR(13) | MINOR(24) | XOP5_FCN(16)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { "flnp1f",   MAJOR(13) | MINOR(24) | XOP5_FCN(17)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { "flogp1f",  MAJOR(13) | MINOR(24) | XOP5_FCN(18)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { NULL,       MAJOR(13) | MINOR(24) | XOP5_FCN(19)| SIGNED(1), MY66000_BAD,   NULL, 0, 0},
  { "fexp2m1f", MAJOR(13) | MINOR(24) | XOP5_FCN(20)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { "fexpm1f",  MAJOR(13) | MINOR(24) | XOP5_FCN(21)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { "fexp10m1f",MAJOR(13) | MINOR(24) | XOP5_FCN(22)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { NULL,       MAJOR(13) | MINOR(24) | XOP5_FCN(23)| SIGNED(1), MY66000_BAD,   NULL, 0, 0},
  { "fsinf",    MAJOR(13) | MINOR(24) | XOP5_FCN(24)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { "fcosf",    MAJOR(13) | MINOR(24) | XOP5_FCN(25)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { "ftanf",    MAJOR(13) | MINOR(24) | XOP5_FCN(26)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { NULL,       MAJOR(13) | MINOR(24) | XOP5_FCN(27)| SIGNED(1), MY66000_BAD,   NULL, 0, 0},
  { "fasinf",   MAJOR(13) | MINOR(24) | XOP5_FCN(28)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { "facosf",   MAJOR(13) | MINOR(24) | XOP5_FCN(29)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { "fatanf",   MAJOR(13) | MINOR(24) | XOP5_FCN(30)| SIGNED(1), MY66000_TRANS, NULL, 0, 0},
  { NULL,       MAJOR(13) | MINOR(24) | XOP5_FCN(31)| SIGNED(1), MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_END,   NULL, 0, 0},
};

static const my66000_opc_info_t opc_op5[] =
{
 { NULL,  MAJOR(13) | MINOR( 0), MY66000_BAD, opc_hr, HR_RW_MASK, HR_RW_OFFS},
 { NULL,  MAJOR(13) | MINOR( 1), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR( 2), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR( 3), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR( 4), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR( 5), MY66000_BAD, opc_abs + 0,    SIGNED_MASK, SIGNED_SHFT},
 { NULL,  MAJOR(13) | MINOR( 6), MY66000_BAD, opc_single + 0, SIGNED_MASK, SIGNED_SHFT},  /* EXPON */
 { NULL,  MAJOR(13) | MINOR( 7), MY66000_BAD, opc_single + 2, SIGNED_MASK, SIGNED_SHFT},  /* FRACT */
 { "pop", MAJOR(13) | MINOR( 8), MY66000_POP, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR( 9), MY66000_BAD, opc_ff1, FF1_MASK, FCN_SHIFT},
 { NULL,  MAJOR(13) | MINOR(10), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(11), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(12), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(13), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(14), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(15), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(16), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(17), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(18), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(19), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(20), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(21), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(22), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(23), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(24), MY66000_BAD, opc_trans, SIGNED_MASK, SIGNED_SHFT},
 { NULL,  MAJOR(13) | MINOR(25), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(26), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(27), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(28), MY66000_BAD, NULL, 0, 0},
 { "vec", MAJOR(13) | MINOR(29), MY66000_VEC32, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(30), MY66000_BAD, NULL, 0, 0},
 { NULL,  MAJOR(13) | MINOR(31), MY66000_BAD, NULL, 0, 0},
 { NULL,   0,                  MY66000_END,   NULL, 0, 0}
};

/* Branch on condition instructions, which specify a register. Keep in
   sync with the opc_pcnd table below.  */

static const my66000_opc_info_t opc_bcnd[] =
{
 { "beq0", MAJOR(26) | CND_MINOR ( 0), MY66000_BC,  NULL, 0, 0},
 { "bne0", MAJOR(26) | CND_MINOR ( 1), MY66000_BC,  NULL, 0, 0},
 { "bge0", MAJOR(26) | CND_MINOR ( 2), MY66000_BC,  NULL, 0, 0},
 { "blt0", MAJOR(26) | CND_MINOR ( 3), MY66000_BC,  NULL, 0, 0},
 { "bgt0", MAJOR(26) | CND_MINOR ( 4), MY66000_BC,  NULL, 0, 0},
 { "ble0", MAJOR(26) | CND_MINOR ( 5), MY66000_BC,  NULL, 0, 0},
 { NULL,    0,                         MY66000_BAD, NULL, 0, 0},
 { NULL,    0,                         MY66000_BAD, NULL, 0, 0},
 { "bdeq", MAJOR(26) | CND_MINOR ( 8), MY66000_BC,  NULL, 0, 0},
 { "bdne", MAJOR(26) | CND_MINOR ( 9), MY66000_BC,  NULL, 0, 0},
 { "bdge", MAJOR(26) | CND_MINOR (10), MY66000_BC,  NULL, 0, 0},
 { "bdlt", MAJOR(26) | CND_MINOR (11), MY66000_BC,  NULL, 0, 0},
 { "bdgt", MAJOR(26) | CND_MINOR (12), MY66000_BC,  NULL, 0, 0},
 { "bdle", MAJOR(26) | CND_MINOR (13), MY66000_BC,  NULL, 0, 0},
 { "bdor", MAJOR(26) | CND_MINOR (14), MY66000_BC,  NULL, 0, 0},
 { "bdun", MAJOR(26) | CND_MINOR (15), MY66000_BC,  NULL, 0, 0},
 { "bfeq", MAJOR(26) | CND_MINOR (16), MY66000_BC,  NULL, 0, 0},
 { "bfne", MAJOR(26) | CND_MINOR (17), MY66000_BC,  NULL, 0, 0},
 { "bfge", MAJOR(26) | CND_MINOR (18), MY66000_BC,  NULL, 0, 0},
 { "bflt", MAJOR(26) | CND_MINOR (19), MY66000_BC,  NULL, 0, 0},
 { "bfgt", MAJOR(26) | CND_MINOR (20), MY66000_BC,  NULL, 0, 0},
 { "bfle", MAJOR(26) | CND_MINOR (21), MY66000_BC,  NULL, 0, 0},
 { "bfor", MAJOR(26) | CND_MINOR (22), MY66000_BC,  NULL, 0, 0},
 { "bfun", MAJOR(26) | CND_MINOR (23), MY66000_BC,  NULL, 0, 0},
 { "bin",  MAJOR(26) | CND_MINOR (24), MY66000_BC,  NULL, 0, 0},
 { NULL,    0,                         MY66000_BAD, NULL, 0, 0},
 { NULL,    0,                         MY66000_BAD, NULL, 0, 0},
 { NULL,    0,                         MY66000_BAD, NULL, 0, 0},
 { NULL,    0,                         MY66000_BAD, NULL, 0, 0},
 { "svr",  MAJOR(26) | CND_MINOR (29), MY66000_SVC, NULL, 0, 0},
 { "svc",  MAJOR(26) | CND_MINOR (30), MY66000_SVC, NULL, 0, 0},
 { "ret",  MAJOR(26) | CND_MINOR (31), MY66000_EMPTY, NULL, 0, 0},
 { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

static const my66000_opc_info_t opc_pcnd[] =
{
 { "peq0", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR ( 0), MY66000_PC,  NULL, 0, 0},
 { "pne0", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR ( 1), MY66000_PC,  NULL, 0, 0},
 { "pge0", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR ( 2), MY66000_PC,  NULL, 0, 0},
 { "plt0", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR ( 3), MY66000_PC,  NULL, 0, 0},
 { "pgt0", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR ( 4), MY66000_PC,  NULL, 0, 0},
 { "ple0", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR ( 5), MY66000_PC,  NULL, 0, 0},
 { NULL,    0,                         MY66000_BAD, NULL, 0, 0},
 { NULL,    0,                         MY66000_BAD, NULL, 0, 0},
 { "pdeq", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR ( 8), MY66000_PC,  NULL, 0, 0},
 { "pdne", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR ( 9), MY66000_PC,  NULL, 0, 0},
 { "pdge", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR (10), MY66000_PC,  NULL, 0, 0},
 { "pdlt", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR (11), MY66000_PC,  NULL, 0, 0},
 { "pdgt", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR (12), MY66000_PC,  NULL, 0, 0},
 { "pdle", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR (13), MY66000_PC,  NULL, 0, 0},
 { "pdor", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR (14), MY66000_PC,  NULL, 0, 0},
 { "pdun", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR (15), MY66000_PC,  NULL, 0, 0},
 { "pfeq", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR (16), MY66000_PC,  NULL, 0, 0},
 { "pfne", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR (17), MY66000_PC,  NULL, 0, 0},
 { "pfge", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR (18), MY66000_PC,  NULL, 0, 0},
 { "pflt", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR (19), MY66000_PC,  NULL, 0, 0},
 { "pfgt", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR (20), MY66000_PC,  NULL, 0, 0},
 { "pfle", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR (21), MY66000_PC,  NULL, 0, 0},
 { "pfor", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR (22), MY66000_PC,  NULL, 0, 0},
 { "pfun", MAJOR(6) | SHFT_MINOR(1) | CND_MINOR (23), MY66000_PC,  NULL, 0, 0},
 { "pin",  MAJOR(6) | SHFT_MINOR(1) | CND_MINOR (24), MY66000_PC,  NULL, 0, 0},
 { NULL,    0,                         MY66000_BAD, NULL, 0, 0},
 { NULL,    0,                         MY66000_BAD, NULL, 0, 0},
 { NULL,    0,                         MY66000_BAD, NULL, 0, 0},
 { NULL,    0,                         MY66000_BAD, NULL, 0, 0},
 { NULL,    0,                         MY66000_BAD, NULL, 0, 0},
 { NULL,    0,                         MY66000_BAD, NULL, 0, 0},
 { NULL,    0,                         MY66000_BAD, NULL, 0, 0},
 { NULL,    0,                         MY66000_BAD, NULL, 0, 0},
 { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

static const my66000_opc_info_t opc_tt[] = {
  { "jtt",  MAJOR(27) | TT_MINOR(0), MY66000_TT, opc_jtt, TT_SIZE_MASK, TT_SIZE_OFFS},
  { "ctt",  MAJOR(27) | TT_MINOR(1), MY66000_TT, opc_jtt, TT_SIZE_MASK, TT_SIZE_OFFS},
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

static const my66000_opc_info_t opc_jtt[] =
{
 {"jttb", MAJOR(27) | TT_MINOR(0) | TT_SIZE(0), MY66000_TT,  NULL, 0, 0},
 {"jtth", MAJOR(27) | TT_MINOR(0) | TT_SIZE(1), MY66000_TT,  NULL, 0, 0},
 {"jttw", MAJOR(27) | TT_MINOR(0) | TT_SIZE(2), MY66000_TT,  NULL, 0, 0},
 {"jttd", MAJOR(27) | TT_MINOR(0) | TT_SIZE(3), MY66000_TT,  NULL, 0, 0},
 { NULL,  0,                                    MY66000_END, NULL, 0, 0}
 };

/* The two halves of the branch on bit instructions.  Keep in sync with
 the predicate on condition codes below.  */

static const my66000_opc_info_t opc_bb1a[] =
{
 { "beq",  MAJOR(24) | BB1_MINOR( 0), MY66000_BC,  NULL, 0, 0},
 { "bneq", MAJOR(24) | BB1_MINOR( 1), MY66000_BC,  NULL, 0, 0},
 { "bne",  MAJOR(24) | BB1_MINOR( 2), MY66000_BC,  NULL, 0, 0},
 { "bnne", MAJOR(24) | BB1_MINOR( 3), MY66000_BC,  NULL, 0, 0},
 { "bge",  MAJOR(24) | BB1_MINOR( 4), MY66000_BC,  NULL, 0, 0},
 { "bnge", MAJOR(24) | BB1_MINOR( 5), MY66000_BC,  NULL, 0, 0},
 { "blt",  MAJOR(24) | BB1_MINOR( 6), MY66000_BC,  NULL, 0, 0},
 { "bnlt", MAJOR(24) | BB1_MINOR( 7), MY66000_BC,  NULL, 0, 0},
 { "bgt",  MAJOR(24) | BB1_MINOR( 8), MY66000_BC,  NULL, 0, 0},
 { "bngt", MAJOR(24) | BB1_MINOR( 9), MY66000_BC,  NULL, 0, 0},
 { "ble",  MAJOR(24) | BB1_MINOR(10), MY66000_BC,  NULL, 0, 0},
 { "bnle", MAJOR(24) | BB1_MINOR(11), MY66000_BC,  NULL, 0, 0},
 { "bhs",  MAJOR(24) | BB1_MINOR(12), MY66000_BC,  NULL, 0, 0},
 { "blo",  MAJOR(24) | BB1_MINOR(13), MY66000_BC,  NULL, 0, 0},
 { "bhi",  MAJOR(24) | BB1_MINOR(14), MY66000_BC,  NULL, 0, 0},
 { "bls",  MAJOR(24) | BB1_MINOR(15), MY66000_BC,  NULL, 0, 0},
 { "bor",  MAJOR(24) | BB1_MINOR(16), MY66000_BC,  NULL, 0, 0},
 { "bnor", MAJOR(24) | BB1_MINOR(17), MY66000_BC,  NULL, 0, 0},
 { "bto",  MAJOR(24) | BB1_MINOR(18), MY66000_BC,  NULL, 0, 0},
 { "bnto", MAJOR(24) | BB1_MINOR(19), MY66000_BC,  NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(20), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(21), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(23), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(24), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(25), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(26), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(27), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(28), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(29), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(30), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(31), MY66000_BAD, NULL, 0, 0},
 { NULL,   0,                         MY66000_END , NULL, 0, 0}
};

static const my66000_opc_info_t opc_bb1b[] =
{
 { "bsnan", MAJOR(25) | BB1_MINOR( 0), MY66000_BC, NULL, 0, 0},
 { "bqnan", MAJOR(25) | BB1_MINOR( 1), MY66000_BC, NULL, 0, 0},
 { "bminf", MAJOR(25) | BB1_MINOR( 2), MY66000_BC, NULL, 0, 0},
 { "bmnor", MAJOR(25) | BB1_MINOR( 3), MY66000_BC, NULL, 0, 0},
 { "bmde",  MAJOR(25) | BB1_MINOR( 4), MY66000_BC, NULL, 0, 0},
 { "bmze",  MAJOR(25) | BB1_MINOR( 5), MY66000_BC, NULL, 0, 0},
 { "bpze",  MAJOR(25) | BB1_MINOR( 6), MY66000_BC, NULL, 0, 0},
 { "bpde",  MAJOR(25) | BB1_MINOR( 7), MY66000_BC, NULL, 0, 0},
 { "bpnor", MAJOR(25) | BB1_MINOR( 8), MY66000_BC, NULL, 0, 0},
 { "bpinf", MAJOR(25) | BB1_MINOR( 9), MY66000_BC, NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_END , NULL, 0, 0},
} ;

/* The two halves of the predicate on bit instructions.  The
   names are the same as branch, except that the first letter
   is 'p' instead of 'b'.  Mind this if condition codes
   change.  */

static const my66000_opc_info_t opc_pb1a[] =
{
 { "peq",  MAJOR(6) | BB1_MINOR( 0), MY66000_PC, NULL, 0, 0},
 { "pneq", MAJOR(6) | BB1_MINOR( 1), MY66000_PC, NULL, 0, 0},
 { "pne",  MAJOR(6) | BB1_MINOR( 2), MY66000_PC, NULL, 0, 0},
 { "pnne", MAJOR(6) | BB1_MINOR( 3), MY66000_PC, NULL, 0, 0},
 { "pge",  MAJOR(6) | BB1_MINOR( 4), MY66000_PC, NULL, 0, 0},
 { "pnge", MAJOR(6) | BB1_MINOR( 5), MY66000_PC, NULL, 0, 0},
 { "plt",  MAJOR(6) | BB1_MINOR( 6), MY66000_PC, NULL, 0, 0},
 { "pnlt", MAJOR(6) | BB1_MINOR( 7), MY66000_PC, NULL, 0, 0},
 { "pgt",  MAJOR(6) | BB1_MINOR( 8), MY66000_PC, NULL, 0, 0},
 { "pngt", MAJOR(6) | BB1_MINOR( 9), MY66000_PC, NULL, 0, 0},
 { "ple",  MAJOR(6) | BB1_MINOR(10), MY66000_PC, NULL, 0, 0},
 { "pnle", MAJOR(6) | BB1_MINOR(11), MY66000_PC, NULL, 0, 0},
 { "phs",  MAJOR(6) | BB1_MINOR(12), MY66000_PC, NULL, 0, 0},
 { "plo",  MAJOR(6) | BB1_MINOR(13), MY66000_PC, NULL, 0, 0},
 { "phi",  MAJOR(6) | BB1_MINOR(14), MY66000_PC, NULL, 0, 0},
 { "pls",  MAJOR(6) | BB1_MINOR(15), MY66000_PC, NULL, 0, 0},
 { "por",  MAJOR(6) | BB1_MINOR(16), MY66000_PC, NULL, 0, 0},
 { "pnor", MAJOR(6) | BB1_MINOR(17), MY66000_PC, NULL, 0, 0},
 { "pto",  MAJOR(6) | BB1_MINOR(18), MY66000_PC, NULL, 0, 0},
 { "pnto", MAJOR(6) | BB1_MINOR(19), MY66000_PC, NULL, 0, 0},
 { NULL,   MAJOR(6) | BB1_MINOR(20), MY66000_PC, NULL, 0, 0},
 { NULL,   MAJOR(6) | BB1_MINOR(21), MY66000_PC, NULL, 0, 0},
 { NULL,   MAJOR(6) | BB1_MINOR(23), MY66000_PC, NULL, 0, 0},
 { NULL,   MAJOR(6) | BB1_MINOR(24), MY66000_PC, NULL, 0, 0},
 { NULL,   MAJOR(6) | BB1_MINOR(25), MY66000_PC, NULL, 0, 0},
 { NULL,   MAJOR(6) | BB1_MINOR(26), MY66000_PC, NULL, 0, 0},
 { NULL,   MAJOR(6) | BB1_MINOR(27), MY66000_PC, NULL, 0, 0},
 { NULL,   MAJOR(6) | BB1_MINOR(28), MY66000_PC, NULL, 0, 0},
 { NULL,   MAJOR(6) | BB1_MINOR(29), MY66000_PC, NULL, 0, 0},
 { NULL,   MAJOR(6) | BB1_MINOR(30), MY66000_PC, NULL, 0, 0},
 { NULL,   MAJOR(6) | BB1_MINOR(31), MY66000_PC, NULL, 0, 0},
 { NULL,   0,                        MY66000_END , NULL, 0, 0}
};

static const my66000_opc_info_t opc_pb1b[] =
{
 { "psnan", MAJOR(7) | BB1_MINOR( 0), MY66000_PC, NULL, 0, 0},
 { "pqnan", MAJOR(7) | BB1_MINOR( 1), MY66000_PC, NULL, 0, 0},
 { "pminf", MAJOR(7) | BB1_MINOR( 2), MY66000_PC, NULL, 0, 0},
 { "pmnor", MAJOR(7) | BB1_MINOR( 3), MY66000_PC, NULL, 0, 0},
 { "pmde",  MAJOR(7) | BB1_MINOR( 4), MY66000_PC, NULL, 0, 0},
 { "pmze",  MAJOR(7) | BB1_MINOR( 5), MY66000_PC, NULL, 0, 0},
 { "ppze",  MAJOR(7) | BB1_MINOR( 6), MY66000_PC, NULL, 0, 0},
 { "ppde",  MAJOR(7) | BB1_MINOR( 7), MY66000_PC, NULL, 0, 0},
 { "ppnor", MAJOR(7) | BB1_MINOR( 8), MY66000_PC, NULL, 0, 0},
 { "ppinf", MAJOR(7) | BB1_MINOR( 9), MY66000_PC, NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_BAD,   NULL, 0, 0},
  { NULL,   0,              MY66000_END , NULL, 0, 0},
} ;

static const my66000_opc_info_t opc_mod[] =
{
 { NULL,    0,                        MY66000_BAD, NULL, 0, 0},
 { "carry", MAJOR(60) | MOD_MINOR(1), MY66000_CARRY, NULL, 0, 0},
 { NULL,    0,                        MY66000_BAD, NULL, 0, 0},
 { "thru",  MAJOR(60) | MOD_MINOR(3), MY66000_CARRY, NULL, 0, 0},
 { NULL,    0,                        MY66000_BAD, NULL, 0, 0},
 { NULL,   0,                         MY66000_END , NULL, 0, 0},

};

static const my66000_opc_info_t opc_loop[] =
{
  { NULL,   0, MAJOR(29) | SIGNED(0), opc_loopu, LOOP_MASK, LOOP_OFFS },
  { NULL,   0, MAJOR(29) | SIGNED(1), opc_loops, LOOP_MASK, LOOP_OFFS },
  { NULL,   0,                                       MY66000_END ,  NULL, 0, 0},
};

static const my66000_opc_info_t opc_loopu[] =
{
  { NULL,     0,                                     MY66000_BAD,   NULL, 0, 0},
  { "loop1",  MAJOR(29) | LOOP_MINOR(1) | SIGNED(0), MY66000_LOOPU, NULL, 0, 0},
  { "loop2",  MAJOR(29) | LOOP_MINOR(2) | SIGNED(0), MY66000_LOOPU, NULL, 0, 0},
  { "loop3",  MAJOR(29) | LOOP_MINOR(3) | SIGNED(0), MY66000_LOOPU, NULL, 0, 0},
  { NULL,   0,                                       MY66000_END ,  NULL, 0, 0},
};

static const my66000_opc_info_t opc_loops[] =
{
  { NULL,     0,                                     MY66000_BAD,   NULL, 0, 0},
  { "loop1",  MAJOR(29) | LOOP_MINOR(1) | SIGNED(1), MY66000_LOOPS, NULL, 0, 0},
  { "loop2",  MAJOR(29) | LOOP_MINOR(2) | SIGNED(1), MY66000_LOOPS, NULL, 0, 0},
  { "loop3",  MAJOR(29) | LOOP_MINOR(3) | SIGNED(1), MY66000_LOOPS, NULL, 0, 0},
  { NULL,   0,                                       MY66000_END ,  NULL, 0, 0},
};

const char *my66000_rname[32] =
  {
    "r0",  "r1",  "r2",	 "r3",	"r4",  "r5",  "r6",  "r7",
    "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15",
    "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23",
    "r24", "r25", "r26", "r27", "r28", "r29", "r30", "sp"
  };

const char *my66000_rbase[32] =
  {
    "ip",  "r1",  "r2",	 "r3",	"r4",  "r5",  "r6",  "r7",
    "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15",
    "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23",
    "r24", "r25", "r26", "r27", "r28", "r29", "r30", "sp"
  };

const char *my66000_rind[32] =
  {
    "",    "r1",  "r2",	 "r3",	"r4",  "r5",  "r6",  "r7",
    "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15",
    "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23",
    "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31"
  };

/* HR functions, treated like registers syntactically.  */

const char *my66000_hr_fcn[32] =
  {
    /*        000     001     010     011     100     101   110  111 */
    /* 00 */  "cs",  "pri",  "int", "intp", "intt", "dev", NULL, NULL,
    /* 01 */  "ht",  "ght",  "got",  "apt",   NULL,  NULL, NULL, NULL,
    /* 10 */ "dis",  "rp",   "csp",  "why", "asid",  "en", NULL, NULL,
    /* 11 */   "c",   "s",   "vec",   "rm",   NULL,  "ra", "ip", NULL,
  };

/* Some alias names for assembly.  For disassembly, the ones
   above will be used.  */

my66000_reg_alias_t my66000_reg_alias[] =
{
 {"r31", 31},
 {"fp",  30},
 {NULL,   0},
};

const char my66000_numtab[32] =
{
 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
};

/* Registers which are legal in a VEC instruction, bitmap style.  */

const char *my66000_vec_reg[MY66000_VEC_BITS] =
{
  "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7",  "r8",  "r9",  "r10",
 "r11", "r12", "r13", "r14", "r15", "r25", "r26", "r27", "r28",  "r29",
 "r30"
};

const char *my66000_loop_u[MY66000_LOOP_CND] =
{
  "eq", "ne", "hs", "lo", "hi", "ls", "a", "n"
};

const char *my66000_loop_s[MY66000_LOOP_CND] =
{
  "eq", "ne", "ge", "lt", "gt", "le", "a", "n"
};


#define OPERAND_MASK(width,shift) ((1u << (width)) - 1) << (shift)
#define OPERAND_ENTRY(width,shift) OPERAND_MASK(width,shift), shift, 0, 0

/* Dept. of dirty tricks: Operands are encoded in the format string
   by letters starting with A.  Lookup is then via
   my66000_operand_table[c - 'A'].  An alternative would be to have
   strings using more descriptive names, like "dst" and put them in a
   hash, but that would create unnecessary overhead during parsing.

   The ordering is not otherwise significant, it just happens to be the
   way it came up during development.

   Immediates have a size and a sequence number.  Sequence number 1 is
   put into the instruction stream before sequence number 2, it is an
   internal error to have two operands with the same sequence number.
*/

const my66000_operand_info_t my66000_operand_table[] =
{
 {MY66000_OPS_DST,    OPERAND_ENTRY ( 5, 21), "Destination register",     'A' },
 {MY66000_OPS_SRC1,   OPERAND_ENTRY ( 5, 16), "Source register 1",        'B' },
 {MY66000_OPS_SRC2,   OPERAND_ENTRY ( 5,  0), "Source register 2",        'C' },
 {MY66000_OPS_RINDEX, OPERAND_ENTRY ( 5,  0), "Index register",           'D' },
 {MY66000_OPS_IMM16,  OPERAND_ENTRY (16,  0), "16-bit signed immediate",  'E' },
 {MY66000_OPS_I1,     OPERAND_ENTRY ( 5, 16), "5-bit immediate SRC1",     'F' },
 {MY66000_OPS_I2,     OPERAND_ENTRY ( 5,  0), "5-bit immediate SRC2",     'G' },
 {MY66000_OPS_BB1,    OPERAND_ENTRY ( 6, 21), "Bit number",		  'H' },
 {MY66000_OPS_B16,    OPERAND_ENTRY (16,  0), "16-bit branch target",	  'I' },
 {MY66000_OPS_B26,    OPERAND_ENTRY (26,  0), "26-bit branch target",	  'J' },
 {MY66000_OPS_RBASE,  OPERAND_ENTRY ( 5, 16), "Base register",            'K' },
 {MY66000_OPS_I32_1,     0, 0, 4, 1,          "32-bit immediate SRC1",    'L' },
 {MY66000_OPS_I32_PCREL, 0, 0, 4, 1,          "32-bit immediate ip-rel",  'M' },
 {MY66000_OPS_SRC3,   OPERAND_ENTRY ( 5,  5), "Source register 3",        'N' },
 {MY66000_OPS_I32_HEX,   0, 0, 4, 1,          "32-bit float immediate",   'O' },
 {MY66000_OPS_I64_1,     0, 0, 8, 1,          "64-bit immediate SRC1",    'P' },
 {MY66000_OPS_I64_PCREL, 0, 0, 8, 1,          "64-bit immediate ip-rel",  'Q' },
 {MY66000_OPS_I64_HEX,   0, 0, 8, 1,          "64-bit float immediate",   'R' },
 {MY66000_OPS_IMM13,  OPERAND_ENTRY (16, 0),  "13-bit aligned immediate", 'S' },
 {MY66000_OPS_I32_ST,    0, 0, 4, 2,          "32-bit immediate stw",     'T' },
 {MY66000_OPS_I64_ST,    0, 0, 8, 2,          "64-bit immediate store",   'U' },
 {MY66000_OPS_WIDTH,   OPERAND_ENTRY ( 6, 6), "6-bit width",		  'V' },
 {MY66000_OPS_OFFSET,  OPERAND_ENTRY ( 6, 0), "6-bit offset",		  'W' },
 {MY66000_OPS_W_BITR,  OPERAND_ENTRY ( 6, 6), "6-bit power of two",       'X' },
 {MY66000_OPS_FL_ENTER, OPERAND_ENTRY (2, 0), "Enter flags",		  'Y' },
 {MY66000_OPS_UNUSED,  0, 0, 0, 0,            "unused",                   'Z' },
 {MY66000_OPS_INVALID, 0, 0, 0, 0,            "non-letter placeholder",   '[' },
 {MY66000_OPS_INVALID, 0, 0, 0, 0,            "non-letter placeholder",   '\\' },
 {MY66000_OPS_INVALID, 0, 0, 0, 0,            "non-letter placeholder",   ']' },
 {MY66000_OPS_INVALID, 0, 0, 0, 0,            "non-letter placeholder",   '^' },
 {MY66000_OPS_INVALID, 0, 0, 0, 0,            "non-letter placeholder",   '_' },
 {MY66000_OPS_INVALID, 0, 0, 0, 0,            "non-letter placeholder",   '\'' },
 {MY66000_OPS_UNUSED , 0, 0, 0, 0,            "unused",                   'a' },
 {MY66000_OPS_UNUSED , 0, 0, 0, 0,            "unused",                   'b' },
 {MY66000_OPS_CARRY,   OPERAND_ENTRY (16, 0), "carry list",               'c' },
 {MY66000_OPS_TF,      OPERAND_ENTRY (12, 0), "true-false predicate list",'d' },
 {MY66000_OPS_HRFCN,   OPERAND_ENTRY ( 5, 0), "HR function",              'e' },
 {MY66000_OPS_UNUSED,  OPERAND_ENTRY ( 4, 0), "also unused",              'f' },
 {MY66000_OPS_INS,     0, 0, 4, 1,            "INS specifier",            'g' },
 {MY66000_OPS_VEC,     OPERAND_ENTRY (21, 0), "Vector bitfield",          'h' },
 {MY66000_OPS_UIMM16,  OPERAND_ENTRY (16, 0), "16-bit unsigned immediate",'i' },
 {MY66000_OPS_SI5,     OPERAND_ENTRY ( 5,21), "5-bit immediate store",    'j' },
 {MY66000_OPS_MSCALE,  OPERAND_ENTRY ( 2,13), "Scale for indexed ld/st",  'k' },
 {MY66000_OPS_VEC32,   0, 0, 4, 1,            "32-bit vector bitfield",   'l' },
 {MY66000_OPS_LOOP_U,  OPERAND_ENTRY ( 3,21), "LOOP condition, unsigned", 'm' },
 {MY66000_OPS_LOOP_S,  OPERAND_ENTRY ( 3,21), "LOOP condition, signed",   'n' },
 {MY66000_OPS_I3,      OPERAND_ENTRY ( 5, 5), "5-bit immediate SRC3",     'o' },
};

/* My 66000 has instructions for which modifiers depend on the
   operands.  This section deals with encoding those.  */

/* Some patterns have a requirement of zeros where otherwise
   registers would be.  Compare to my66000_operand_table above.  */

#define SRC1_MASK OPERAND_MASK(5,16)
#define SRC2_MASK OPERAND_MASK(5, 0)
#define SRC3_MASK OPERAND_MASK(5, 5)

#define SRC2_NUM(a) (a)

static const my66000_fmt_spec_t opimm_fmt_list[] =
{
 { "A,B,#E", 0, 0},
 { NULL,     0, 0},

};

static const my66000_fmt_spec_t mvimm_fmt_list[] =
{
 { "A,#E", 0, 0},
 { NULL,   0, 0}
};

static const my66000_fmt_spec_t mem_fmt_list[] =
{
 { "A,[K]",   0, 0xffff},
 { "A,[K,E]", 0, 0,    },
 { NULL,      0, 0,    },
};

/* This is table 15: 2-Operand Specification.  */

static const my66000_fmt_spec_t arith_fmt_list [] =
{

 { "A,B,C",    XOP2_BITS (0,0,0,0), XOP2_MASK},
 { "A,B,-C",   XOP2_BITS (0,0,0,1), XOP2_MASK},
 { "A,-B,C",   XOP2_BITS (0,0,1,0) ,XOP2_MASK},
 { "A,-B,-C",  XOP2_BITS (0,0,1,1), XOP2_MASK},

 { "A,B,#G",   XOP2_BITS (0,1,0,0), XOP2_MASK},
 { "A,#F,C",   XOP2_BITS (0,1,0,1), XOP2_MASK},
 { "A,B,#-G",  XOP2_BITS (0,1,1,0), XOP2_MASK},
 { "A,#-F,C",  XOP2_BITS (0,1,1,1), XOP2_MASK},

 { "A,B,#L",   XOP2_BITS (1,0,0,0), XOP2_MASK | SRC2_MASK},
 { "A,#L,C",   XOP2_BITS (1,0,0,1), XOP2_MASK | SRC1_MASK},
 { "A,-B,#L",  XOP2_BITS (1,0,1,0), XOP2_MASK | SRC2_MASK},
 { "A,#L,-C",  XOP2_BITS (1,0,1,1), XOP2_MASK | SRC1_MASK},

 { "A,B,#P",   XOP2_BITS (1,1,0,0), XOP2_MASK | SRC2_MASK},
 { "A,#P,C",   XOP2_BITS (1,1,0,1), XOP2_MASK | SRC1_MASK},
 { "A,-B,#P",  XOP2_BITS (1,1,1,0), XOP2_MASK | SRC2_MASK},
 { "A,#P,-C",  XOP2_BITS (1,1,1,1), XOP2_MASK | SRC1_MASK},
 { NULL,      0, 0},
};

/* Much like the above, but for floating point.  */

static const my66000_fmt_spec_t float_fmt_list [] =
{

 { "A,B,C",    XOP2_BITS (0,0,0,0), XOP2_MASK},
 { "A,B,-C",   XOP2_BITS (0,0,0,1), XOP2_MASK},
 { "A,-B,C",   XOP2_BITS (0,0,1,0) ,XOP2_MASK},
 { "A,-B,-C",  XOP2_BITS (0,0,1,1), XOP2_MASK},

 /* Put the negative number first because -0 is significant for
    floating point.  */

 { "A,B,#-G",  XOP2_BITS (0,1,1,0), XOP2_MASK},
 { "A,B,#G",   XOP2_BITS (0,1,0,0), XOP2_MASK},
 { "A,#-F,C",  XOP2_BITS (0,1,1,1), XOP2_MASK},
 { "A,#F,C",   XOP2_BITS (0,1,0,1), XOP2_MASK},

 { "A,B,#O",   XOP2_BITS (1,0,0,0), XOP2_MASK | SRC2_MASK},
 { "A,#O,C",   XOP2_BITS (1,0,0,1), XOP2_MASK | SRC1_MASK},
 { "A,-B,#O",  XOP2_BITS (1,0,1,0), XOP2_MASK | SRC2_MASK},
 { "A,#O,-C",  XOP2_BITS (1,0,1,1), XOP2_MASK | SRC1_MASK},

 { "A,B,#R",   XOP2_BITS (1,1,0,0), XOP2_MASK | SRC2_MASK},
 { "A,#R,C",   XOP2_BITS (1,1,0,1), XOP2_MASK | SRC1_MASK},
 { "A,-B,#R",  XOP2_BITS (1,1,1,0), XOP2_MASK | SRC2_MASK},
 { "A,#R,-C",  XOP2_BITS (1,1,1,1), XOP2_MASK | SRC1_MASK},
 { NULL,      0, 0},
};

/* Conversion. Here, we have the speciality that omitting the second
   source argument is encoded as "#8" as SRC2.

   FIXME: This encoding is NOT CANONCIAL and subject to revision,
   since the encoding has not yet been sanctioned.  */

/* The default encoding for CVT which means "just use the current
   default setting" is 8.  */

#define CVT_DEFAULT SRC2_NUM(8)

static const my66000_fmt_spec_t cvts_fmt_list [] =
{
  {"A,B,C",	 XOP2_BITS (0,0,0,0), XOP2_MASK},

  /* Minus a rounding mode makes limited sense, but for the sake of
     generality...  */
  {"A,B,-C",	 XOP2_BITS (0,0,0,1), XOP2_MASK},
  {"A,-B,C",     XOP2_BITS (0,0,1,0), XOP2_MASK},
  {"A,B,-C",	 XOP2_BITS (0,0,1,1), XOP2_MASK},

  /* 8 is the default constant, other modes are 1-7.  We accept other
     values, because of why not.  */
  {"A,B",        XOP2_BITS (0,1,0,0) | CVT_DEFAULT, XOP2_MASK | SRC2_MASK},
  {"A,B,#G",     XOP2_BITS (0,1,0,0), XOP2_MASK},

  /* Rounding mode is immaterial for five-bit constants, we just set SRC2 to R0.  */
  {"A,#F",	 XOP2_BITS (0,1,0,1), XOP2_MASK | SRC2_MASK},
  /* This one will never be useful.  */

  {"A,B,#-G",  XOP2_BITS (0,1,1,0), XOP2_MASK},
  {"A,B,#L",   XOP2_BITS (1,0,0,0), XOP2_MASK | SRC2_MASK},
  {"A,#L",     XOP2_BITS (1,0,1,1), XOP2_MASK | SRC1_MASK | SRC2_MASK},
  {"A,#L,C",   XOP2_BITS (1,0,0,1), XOP2_MASK | SRC1_MASK},
  {"A,-B,#L",  XOP2_BITS (1,0,1,0), XOP2_MASK | SRC2_MASK},
  {"A,#L,-C",  XOP2_BITS (1,0,1,1), XOP2_MASK | SRC1_MASK},

  {"A,B,#P",   XOP2_BITS (1,1,0,0), XOP2_MASK | SRC2_MASK},
  {"A,#P",     XOP2_BITS (1,1,0,1), XOP2_MASK | SRC1_MASK | SRC2_MASK},
  {"A,#P,C",   XOP2_BITS (1,1,0,1), XOP2_MASK | SRC1_MASK},
  {"A,-B,#P",  XOP2_BITS (1,1,1,0), XOP2_MASK | SRC2_MASK},
  {"A,#P,-C",  XOP2_BITS (1,1,1,1), XOP2_MASK | SRC1_MASK},

  { NULL,      0, 0},

};

static const my66000_fmt_spec_t cvtu_fmt_list [] =
{
  {"A,B,C",	 XOP2_BITS (0,0,0,0), XOP2_MASK},

  /* Minus a rounding mode makes limited sense, but for the sake of
     generality...  */
  {"A,B,-C",	 XOP2_BITS (0,0,0,1), XOP2_MASK},
  {"A,-B,C",     XOP2_BITS (0,0,1,0), XOP2_MASK},
  {"A,B,-C",	 XOP2_BITS (0,0,1,1), XOP2_MASK},

  /* 8 is the default constant, other modes are 1-7.  We accept other
     values, because of why not.  */
  {"A,B",        XOP2_BITS (0,1,0,0) | CVT_DEFAULT, XOP2_MASK | SRC2_MASK},
  {"A,B,#G",     XOP2_BITS (0,1,0,0), XOP2_MASK},

  /* Rounding mode is immaterial for five-bit constants, we just set SRC2 to R0.  */
  {"A,#F",	 XOP2_BITS (0,1,0,1), XOP2_MASK | SRC2_MASK},
  /* This one will never be legal.  */

  {"A,B,#-G",	 XOP2_BITS (0,1,1,0), XOP2_MASK},
  { "A,B,#L",   XOP2_BITS (1,0,0,0), XOP2_MASK | SRC2_MASK},
  { "A,#O",     XOP2_BITS (1,0,0,1), XOP2_MASK | SRC1_MASK | SRC2_MASK},
  { "A,#O,C",   XOP2_BITS (1,0,0,1), XOP2_MASK | SRC1_MASK},
  { "A,-B,#L",  XOP2_BITS (1,0,1,0), XOP2_MASK | SRC2_MASK},
  { "A,#O,-C",  XOP2_BITS (1,0,1,1), XOP2_MASK | SRC1_MASK},

  { "A,B",      XOP2_BITS (1,1,0,0), XOP2_MASK | SRC1_MASK | SRC2_MASK},
  { "A,B,#P",   XOP2_BITS (1,1,0,0), XOP2_MASK | SRC2_MASK},
  { "A,#P,C",   XOP2_BITS (1,1,0,1), XOP2_MASK | SRC1_MASK},
  { "A,-B,#P",  XOP2_BITS (1,1,1,0), XOP2_MASK | SRC2_MASK},
  { "A,#P,-C",  XOP2_BITS (1,1,1,1), XOP2_MASK | SRC1_MASK},

  { NULL,      0, 0},

};

/* EADD is special because it has one integer operand.  */

static const my66000_fmt_spec_t eadd_fmt_list [] =
{

 { "A,B,C",    XOP2_BITS (0,0,0,0), XOP2_MASK},
 { "A,B,-C",   XOP2_BITS (0,0,0,1), XOP2_MASK},
 { "A,-B,C",   XOP2_BITS (0,0,1,0) ,XOP2_MASK},
 { "A,-B,-C",  XOP2_BITS (0,0,1,1), XOP2_MASK},

 { "A,B,#G",   XOP2_BITS (0,1,0,0), XOP2_MASK},
 { "A,#F,C",   XOP2_BITS (0,1,0,1), XOP2_MASK},
 { "A,B,#-G",  XOP2_BITS (0,1,1,0), XOP2_MASK},
 { "A,#-F,C",  XOP2_BITS (0,1,1,1), XOP2_MASK},

 { "A,B,#L",   XOP2_BITS (1,0,0,0), XOP2_MASK | SRC2_MASK},
 { "A,#O,C",   XOP2_BITS (1,0,0,1), XOP2_MASK | SRC1_MASK},
 { "A,-B,#L",  XOP2_BITS (1,0,1,0), XOP2_MASK | SRC2_MASK},
 { "A,#O,-C",  XOP2_BITS (1,0,1,1), XOP2_MASK | SRC1_MASK},

 { "A,B,#P",   XOP2_BITS (1,1,0,0), XOP2_MASK | SRC2_MASK},
 { "A,#R,C",   XOP2_BITS (1,1,0,1), XOP2_MASK | SRC1_MASK},
 { "A,-B,#Ü",  XOP2_BITS (1,1,1,0), XOP2_MASK | SRC2_MASK},
 { "A,#R,-C",  XOP2_BITS (1,1,1,1), XOP2_MASK | SRC1_MASK},
 { NULL,      0, 0},
};

static const my66000_fmt_spec_t bb1_fmt_list [] =
{
 { "H,B,I", 0, 0},
 { NULL,    0, 0},
};

/* Format for unconditional branches.  */

static const my66000_fmt_spec_t br_fmt_list [] =
{
 { "J",    0,  0},
 { NULL,   0,  0},
};

/* Formats for conditonal branches.  */
static const my66000_fmt_spec_t bc_fmt_list [] =
{
 {"B,I", 0, 0},
 { NULL,    0, 0},
};

/* TT instruction.  */
static const my66000_fmt_spec_t tt_fmt_list [] =
{
  {"B,#i",  0, 0},
  { NULL,  0, 0},
};

/* Table for load/store with two registers.  */

static const my66000_fmt_spec_t mrr_fmt_list [] =
{
 /* This is for disassembly only, we prefer offset 0 for
    just indirect load.  */
 { "A,[K]",        XOP1_BITS(0,0), MRR_FMT_MASK | XOP1_SCALE_MASK | RIND_ZERO_MASK},

 /* Different syntax variants of scaled indexing without offset.  */
 { "A,[K,D,0]",    XOP1_BITS(0,0), MRR_FMT_MASK | XOP1_SCALE_MASK},
 { "A,[K,D<<k]",   XOP1_BITS(0,0), MRR_FMT_MASK},
 { "A,[K,D<<k,0]", XOP1_BITS(0,0), MRR_FMT_MASK},

 /* FIXME:  Relocations for symbols which are not offset
    with respect to the IP are poorly defined at the moment.  */

 /* IP-relative, without register offset, and an IP-relative 32-bit
    and 64-bit relocations, respectively.  */
 { "A,[K,M]",      XOP1_BITS(1,0), MRR_FMT_MASK | XOP1_SCALE_MASK | RIND_ZERO_MASK},
 { "A,[K,Q]",      XOP1_BITS(1,1), MRR_FMT_MASK | XOP1_SCALE_MASK | RIND_ZERO_MASK},

 /* IP-relative, with scaled index register and offset.  If the index register is zero, this
    gets picked up earlier.*/

 { "A,[K,D,M]",    XOP1_BITS(1,0), MRR_FMT_MASK | XOP1_SCALE_MASK},
 { "A,[K,D,Q]",    XOP1_BITS(1,1), MRR_FMT_MASK | XOP1_SCALE_MASK},
 { "A,[K,D<<k,M]", XOP1_BITS(1,0), MRR_FMT_MASK},
 { "A,[K,D<<k,Q]", XOP1_BITS(1,1), MRR_FMT_MASK},
 { NULL, 0, 0},
};

/* CALX and CALLA.  */

static const my66000_fmt_spec_t calx_fmt_list [] =
{
 { "[K]",        XOP1_BITS(0,0), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK | RIND_ZERO_MASK},
 { "[K,D,0]",    XOP1_BITS(0,0), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK},
 { "[K,D<<k]",   XOP1_BITS(0,0), DST_MASK | MRR_FMT_MASK},
 { "[K,D<<k,0]", XOP1_BITS(0,0), DST_MASK | MRR_FMT_MASK},
 { "[K,M]",      XOP1_BITS(1,0), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK | RIND_ZERO_MASK},
 { "[K,Q]",      XOP1_BITS(1,1), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK | RIND_ZERO_MASK},
 { "[K,D,M]",    XOP1_BITS(1,0), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK},
 { "[K,D,Q]",    XOP1_BITS(1,1), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK},
 { "[K,D<<k,M]", XOP1_BITS(1,0), DST_MASK | MRR_FMT_MASK},
 { "[K,D<<k,Q]", XOP1_BITS(1,1), DST_MASK | MRR_FMT_MASK},
 { NULL, 0, 0},
};

static const my66000_fmt_spec_t si_fmt_list [] =
{
 { "#T,[K]",        XOP1_BITS(0,0), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK | RIND_ZERO_MASK},
 { "#T,[K,D,0]",    XOP1_BITS(0,0), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK},
 { "#T,[K,D<<k]",   XOP1_BITS(0,0), DST_MASK | MRR_FMT_MASK},
 { "#T,[K,D<<k,0]", XOP1_BITS(0,0), DST_MASK | MRR_FMT_MASK},

 { "#T,[K,M]",      XOP1_BITS(1,0), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK | RIND_ZERO_MASK},
 { "#T,[K,Q]",      XOP1_BITS(1,1), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK | RIND_ZERO_MASK},

 { "#T,[K,D,M]",    XOP1_BITS(1,0), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK},
 { "#T,[K,D,Q]",    XOP1_BITS(1,1), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK},
 { "#T,[K,D<<k,M]", XOP1_BITS(1,0), DST_MASK | MRR_FMT_MASK},
 { "#T,[K,D<<k,Q]", XOP1_BITS(1,1), DST_MASK | MRR_FMT_MASK},

 { NULL, 0, 0},
};

static const my66000_fmt_spec_t si_ldd_fmt_list [] =
{
 { "#U,[K]",        XOP1_BITS(0,0), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK | RIND_ZERO_MASK},
 { "#U,[K,D,0]",    XOP1_BITS(0,0), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK},
 { "#U,[K,D<<k]",   XOP1_BITS(0,0), DST_MASK | MRR_FMT_MASK},
 { "#U,[K,D<<k,0]", XOP1_BITS(0,0), DST_MASK | MRR_FMT_MASK},

 { "#U,[K,M]",      XOP1_BITS(1,0), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK | RIND_ZERO_MASK},
 { "#U,[K,Q]",      XOP1_BITS(1,1), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK | RIND_ZERO_MASK},

 { "#U,[K,D,M]",    XOP1_BITS(1,0), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK},
 { "#U,[K,D,Q]",    XOP1_BITS(1,1), DST_MASK | MRR_FMT_MASK | XOP1_SCALE_MASK},
 { "#U,[K,D<<k,M]", XOP1_BITS(1,0), DST_MASK | MRR_FMT_MASK},
 { "#U,[K,D<<k,Q]", XOP1_BITS(1,1), DST_MASK | MRR_FMT_MASK},
 { NULL,       0, 0},
};

static const my66000_fmt_spec_t si5_fmt_list [] =
{
 { "#j,[K]",        XOP1_BITS(0,0), MRR_FMT_MASK | XOP1_SCALE_MASK | RIND_ZERO_MASK},
 { "#j,[K,D,0]",    XOP1_BITS(0,0), MRR_FMT_MASK | XOP1_SCALE_MASK},
 { "#j,[K,D<<k]",   XOP1_BITS(0,0), MRR_FMT_MASK},
 { "#j,[K,D<<k,0]", XOP1_BITS(0,0), MRR_FMT_MASK},

 { "#j,[K,M]",      XOP1_BITS(0,1), MRR_FMT_MASK | XOP1_SCALE_MASK | RIND_ZERO_MASK},
 { "#j,[K,Q]",      XOP1_BITS(1,1), MRR_FMT_MASK | XOP1_SCALE_MASK | RIND_ZERO_MASK},

 { "#j,[K,D,M]",    XOP1_BITS(0,1), MRR_FMT_MASK | XOP1_SCALE_MASK},
 { "#j,[K,D,Q]",    XOP1_BITS(1,1), MRR_FMT_MASK | XOP1_SCALE_MASK},
 { "#j,[K,D<<k,M]", XOP1_BITS(0,1), MRR_FMT_MASK},
 { "#j,[K,D<<k,Q]", XOP1_BITS(1,1), MRR_FMT_MASK},
 { NULL, 0, 0},
};

/* Table 14: 3-Operand Constant Specification.  */

static const my66000_fmt_spec_t fmac_fmt_list [] =
{
 { "A,B,C,N",   XOP4_BITS (0,0,0), XOP4_FMT_MASK},
 { "A,B,C,-N",  XOP4_BITS (0,0,1), XOP4_FMT_MASK},
 { "A,B,-C,N",  XOP4_BITS (0,1,0), XOP4_FMT_MASK},
 { "A,B,-C,-N", XOP4_BITS (0,1,1), XOP4_FMT_MASK},
 { "A,B,C,#O",  XOP4_BITS (1,0,0), XOP4_FMT_MASK},
 { "A,B,#O,N",  XOP4_BITS (1,0,1), XOP4_FMT_MASK},
 { "A,B,C,#R",  XOP4_BITS (1,1,0), XOP4_FMT_MASK},
 { "A,B,#R,N",  XOP4_BITS (1,1,1), XOP4_FMT_MASK},
 { NULL, 0, 0}
};

static const my66000_fmt_spec_t ins_fmt_list [] =
{
 { "A,B,C,N",   XOP4_BITS (0,0,0), XOP4_FMT_MASK},
 { "A,B,C,-N",  XOP4_BITS (0,0,1), XOP4_FMT_MASK},
 { "A,B,-C,N",  XOP4_BITS (0,1,0), XOP4_FMT_MASK},
 { "A,B,-C,-N", XOP4_BITS (0,1,1), XOP4_FMT_MASK},
 { "A,B,C,#<g>",  XOP4_BITS (1,0,0), XOP4_FMT_MASK},
 { "A,B,#O,N",  XOP4_BITS (1,0,1), XOP4_FMT_MASK},
 { "A,B,C,#R",  XOP4_BITS (1,1,0), XOP4_FMT_MASK},
 { "A,B,#R,N",  XOP4_BITS (1,1,1), XOP4_FMT_MASK},
 { NULL,       0, 0},
};

static const my66000_fmt_spec_t mux_fmt_list[] =
{
 { "A,B,C,N",  0, 0},
 { NULL,       0, 0},
};

static const my66000_fmt_spec_t mov2_fmt_list[] =
{
 { "A,C",      0, 0},
 { NULL,       0, 0},
};

static const my66000_fmt_spec_t mov3_fmt_list[] =
{
 { "A,N",      0, 0},
 { NULL,       0, 0},
};

static const my66000_fmt_spec_t mux32_fmt_list[] =
{
 { "A,B,C,#L", 0, 0},
 { NULL,       0, 0},
};

static const my66000_fmt_spec_t mov32_fmt_list[] =
{
 { "A,#L",     0, 0},
 { NULL,       0, 0},
};

static const my66000_fmt_spec_t mux64_fmt_list[] =
{
 { "A,B,C,#P", 0, 0},
 { NULL,       0, 0},
};

static const my66000_fmt_spec_t mov64_fmt_list[] =
{
 { "A,#P",     0, 0},
 { NULL,       0, 0},
};

static const my66000_fmt_spec_t shift_fmt_list[] =
{
 { "A,B,<V:W>", 0, 0},
 { NULL,        0, 0}
};

/* Prediate on bit set.  */
static const my66000_fmt_spec_t pb1_fmt_list[] =
{
  { "H,B,d", 0, THEN_FMT_MASK_7 | ELSE_FMT_MASK_7},
  { "H,B,d", 0, THEN_FMT_MASK_7 | ELSE_FMT_MASK_8},
  { "H,B,d", 0, THEN_FMT_MASK_8 | ELSE_FMT_MASK_7},
  { "H,B,d", 0, THEN_FMT_MASK_8 | ELSE_FMT_MASK_8},
 { NULL,        0, 0},
};

/* Predicate on condition.  */

static const my66000_fmt_spec_t pcnd_fmt_list[] =
{
  { "B,d", 0, THEN_FMT_MASK_7 | ELSE_FMT_MASK_7},
  { "B,d", 0, THEN_FMT_MASK_8 | ELSE_FMT_MASK_7},
  { "B,d", 0, THEN_FMT_MASK_7 | ELSE_FMT_MASK_8},
  { "B,d", 0, THEN_FMT_MASK_8 | ELSE_FMT_MASK_8},
 { NULL,     0, 0},
};

static const my66000_fmt_spec_t bcnd_fmt_list[] =
{
 { "B,E",    0, 0},
 { NULL,     0, 0},
};

/* Predicate on bit set, based on comparison.   */

static const my66000_fmt_spec_t pc_fmt_list[] =
{
 { "B,d",       0, 0},
 { NULL,        0, 0},
};

static const my66000_fmt_spec_t enter_fmt_list[] =
{
 { "A,B,S",   0, ENTER_MASK},
 { "A,B,S,Y", 0, 0},
 { "A,B,Y,S", 0, 0},  /* FIXME: This needs to be removed.  */
 { NULL,      0, 0},
};

static const my66000_fmt_spec_t carry_fmt_list[] =
{
 { "A,{c}",  0, 0},
 { NULL,     0, 0},
};

static const my66000_fmt_spec_t hrr_fmt_list[] =
{
 { "A,e",  HR_BITS(0,0), HR_FMT_MASK | SRC1_MASK},
 { NULL,     0, 0},
};

static const my66000_fmt_spec_t hrw_fmt_list[] =
{
 { "e,B",   HR_BITS(0,0), HR_FMT_MASK | DST_MASK},
 { "e,#F",  HR_BITS(0,1), HR_FMT_MASK | DST_MASK},
 { "e,#L",  HR_BITS(1,0), HR_FMT_MASK | DST_MASK | SRC1_MASK},
 { "e,#P",  HR_BITS(1,1), HR_FMT_MASK | DST_MASK | SRC1_MASK},
 { NULL,     0, 0},
};

static const my66000_fmt_spec_t hrx_fmt_list[] =
{
  {"A,e,B", HR_BITS(0,0), HR_FMT_MASK},
 { "A,e,#F",  HR_BITS(0,1), HR_FMT_MASK},
 { "A,e,#L",  HR_BITS(1,0), HR_FMT_MASK | SRC1_MASK},
 { "A,e,#P",  HR_BITS(1,1), HR_FMT_MASK | SRC1_MASK},
 { NULL,     0, 0},
};

/* JMP is HRW IP,SRC1, CALLI is HR R0,IP,SRC1.  Since DST is zero in both
   cases, it is enough to use a single format table.  */
static const my66000_fmt_spec_t jmp_fmt_list[] =
{
  {"B",  HR_BITS(0,0) | HR_FCN_IP, HR_FMT_MASK | DST_MASK | XOP5_FCN_MASK},
  {"#F", HR_BITS(0,1) | HR_FCN_IP, HR_FMT_MASK | DST_MASK | XOP5_FCN_MASK},
  {"#L", HR_BITS(1,0) | HR_FCN_IP, HR_FMT_MASK | DST_MASK | SRC1_MASK | XOP5_FCN_MASK},
  {"#P", HR_BITS(1,1) | HR_FCN_IP, HR_FMT_MASK | DST_MASK | SRC1_MASK | XOP5_FCN_MASK},
  { NULL, 0, 0},
};


static const my66000_fmt_spec_t calli_fmt_list[] =
{
 { NULL, 0, 0},
};

static const my66000_fmt_spec_t vec_fmt_list[] =
{
 {"A,{h}", 0, 0},
 { NULL, 0, 0},
};

static const my66000_fmt_spec_t empty_fmt_list[] =
{
 {"",    0, 0},
 { NULL, 0, 0},
};

static const my66000_fmt_spec_t mm_fmt_list [] =
{
 { "A,B,C", 0, 0},
 { NULL, 0, 0},
};

static const my66000_fmt_spec_t op5_d_fmt_list [] =
{
  { "A,B",     XOP5_BITS(0,0,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "+A,B",    XOP5_BITS(0,0,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "A,+B",    XOP5_BITS(0,0,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "+A,+B",   XOP5_BITS(0,0,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "A,-B",    XOP5_BITS(0,0,0,1), XOP5_FMT_MASK | SRC2_MASK},
  { "+A,-B",   XOP5_BITS(0,0,0,1), XOP5_FMT_MASK | SRC2_MASK},
  { "-A,B",    XOP5_BITS(0,0,1,0), XOP5_FMT_MASK | SRC2_MASK},
  { "-A,+B",   XOP5_BITS(0,0,1,0), XOP5_FMT_MASK | SRC2_MASK},
  { "-A,-B",   XOP5_BITS(0,0,1,1), XOP5_FMT_MASK | SRC2_MASK},
  { "A,#F",    XOP5_BITS(0,1,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "+A,#F",   XOP5_BITS(0,1,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "A,#-F",   XOP5_BITS(0,1,0,1), XOP5_FMT_MASK | SRC2_MASK},
  { "+A,#-F",  XOP5_BITS(0,1,0,1), XOP5_FMT_MASK | SRC2_MASK},
  { "-A,#F",   XOP5_BITS(0,1,1,0), XOP5_FMT_MASK | SRC2_MASK},
  { "-A,#-F",  XOP5_BITS(0,1,1,1), XOP5_FMT_MASK | SRC2_MASK},
  { "A,#O",    XOP5_BITS(1,0,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "+A,#O",   XOP5_BITS(1,0,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "A,#-O",   XOP5_BITS(1,0,0,1), XOP5_FMT_MASK | SRC2_MASK},
  { "+A,#-O",  XOP5_BITS(1,0,0,1), XOP5_FMT_MASK | SRC2_MASK},
  { "-A,#O",   XOP5_BITS(1,0,1,0), XOP5_FMT_MASK | SRC2_MASK},
  { "-A,#-O",  XOP5_BITS(1,0,1,1), XOP5_FMT_MASK | SRC2_MASK},
  { "A,#P",    XOP5_BITS(1,1,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "+A,#P",   XOP5_BITS(1,1,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "A,#-P",   XOP5_BITS(1,1,0,1), XOP5_FMT_MASK | SRC2_MASK},
  { "+A,#-P",  XOP5_BITS(1,1,0,1), XOP5_FMT_MASK | SRC2_MASK},
  { "-A,#P",   XOP5_BITS(1,1,1,0), XOP5_FMT_MASK | SRC2_MASK},
  { "-A,#-P",  XOP5_BITS(1,1,1,1), XOP5_FMT_MASK | SRC2_MASK},

 { NULL, 0, 0},
};

static const my66000_fmt_spec_t op5_f_fmt_list [] =
{
  { "A,B",     XOP5_BITS(0,0,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "+A,B",    XOP5_BITS(0,0,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "A,+B",    XOP5_BITS(0,0,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "+A,+B",   XOP5_BITS(0,0,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "A,-B",    XOP5_BITS(0,0,0,1), XOP5_FMT_MASK | SRC2_MASK},
  { "+A,-B",   XOP5_BITS(0,0,0,1), XOP5_FMT_MASK | SRC2_MASK},
  { "-A,B",    XOP5_BITS(0,0,1,0), XOP5_FMT_MASK | SRC2_MASK},
  { "-A,+B",   XOP5_BITS(0,0,1,0), XOP5_FMT_MASK | SRC2_MASK},
  { "-A,-B",   XOP5_BITS(0,0,1,1), XOP5_FMT_MASK | SRC2_MASK},
  { "A,#F",    XOP5_BITS(0,1,0,0), XOP5_FMT_MASK},
  { "+A,#F",   XOP5_BITS(0,1,0,0), XOP5_FMT_MASK},
  { "A,#-F",   XOP5_BITS(0,1,0,1), XOP5_FMT_MASK},
  { "+A,#-F",  XOP5_BITS(0,1,0,1), XOP5_FMT_MASK},
  { "-A,#F",   XOP5_BITS(0,1,1,0), XOP5_FMT_MASK},
  { "-A,#-F",  XOP5_BITS(0,1,1,1), XOP5_FMT_MASK},
  { "A,#O",    XOP5_BITS(1,0,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "+A,#O",   XOP5_BITS(1,0,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "A,#-O",   XOP5_BITS(1,0,0,1), XOP5_FMT_MASK | SRC2_MASK},
  { "+A,#-O",  XOP5_BITS(1,0,0,1), XOP5_FMT_MASK | SRC2_MASK},
  { "-A,#O",   XOP5_BITS(1,0,1,0), XOP5_FMT_MASK | SRC2_MASK},
  { "-A,#-O",  XOP5_BITS(1,0,1,1), XOP5_FMT_MASK | SRC2_MASK},
#if 0
  { "A,#P",    XOP5_BITS(1,1,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "+A,#P",   XOP5_BITS(1,1,0,0), XOP5_FMT_MASK | SRC2_MASK},
  { "A,#-P",   XOP5_BITS(1,1,0,1), XOP5_FMT_MASK | SRC2_MASK},
  { "+A,#-P",  XOP5_BITS(1,1,0,1), XOP5_FMT_MASK | SRC2_MASK},
  { "-A,#P",   XOP5_BITS(1,1,1,0), XOP5_FMT_MASK | SRC2_MASK},
  { "-A,#-P",  XOP5_BITS(1,1,1,1), XOP5_FMT_MASK | SRC2_MASK},
#endif
 { NULL, 0, 0},
};

static const my66000_fmt_spec_t pop_fmt_list [] =
{
  {"A,B",  XOP5_S1(0), XOP5_I_MASK | XOP5_SD_MASK | XOP5_S1_MASK | SRC2_MASK},
  {"A,-B", XOP5_S1(1), XOP5_I_MASK | XOP5_SD_MASK | XOP5_S1_MASK | SRC2_MASK},
  { NULL, 0, 0},
};

static const my66000_fmt_spec_t abs_fmt_list[] =
{
  {"A,B",   XOP5_BITS(0,0,0,0), XOP5_FMT_MASK | XOP5_FMT_ABS_MASK},
  {"+A,B",  XOP5_BITS(0,0,0,0), XOP5_FMT_MASK | XOP5_FMT_ABS_MASK},
  {"A,+B",  XOP5_BITS(0,0,0,0), XOP5_FMT_MASK | XOP5_FMT_ABS_MASK},
  {"+A,+B", XOP5_BITS(0,0,0,0), XOP5_FMT_MASK | XOP5_FMT_ABS_MASK},
  {"-A,B",  XOP5_BITS(0,0,1,0), XOP5_FMT_MASK | XOP5_FMT_ABS_MASK},
  {"-A,+B", XOP5_BITS(0,0,1,0), XOP5_FMT_MASK | XOP5_FMT_ABS_MASK},
  {"A,#F",  XOP5_BITS(0,1,0,0), XOP5_FMT_MASK | XOP5_FMT_ABS_MASK},
  {"+A,#F", XOP5_BITS(0,1,0,0), XOP5_FMT_MASK | XOP5_FMT_ABS_MASK},
  {"-A,#F", XOP5_BITS(0,1,1,0), XOP5_FMT_MASK | XOP5_FMT_ABS_MASK},
  {"A,#L",  XOP5_BITS(1,0,0,0), XOP5_FMT_MASK | XOP5_FMT_ABS_MASK},
  {"+A,#L", XOP5_BITS(1,0,0,0), XOP5_FMT_MASK | XOP5_FMT_ABS_MASK},
  {"-A,#L", XOP5_BITS(1,0,1,0), XOP5_FMT_MASK | XOP5_FMT_ABS_MASK},
  {"A,#P",  XOP5_BITS(1,1,0,0), XOP5_FMT_MASK | XOP5_FMT_ABS_MASK},
  {"+A,#P", XOP5_BITS(1,1,0,0), XOP5_FMT_MASK | XOP5_FMT_ABS_MASK},
  {"-A,#P", XOP5_BITS(1,1,1,0), XOP5_FMT_MASK | XOP5_FMT_ABS_MASK},
  { NULL, 0, 0},
};

static const my66000_fmt_spec_t trans_fmt_list[] =
{
  {"A,B",   XOP5_I(0) | XOP5_S1(0) | XOP5_SD(0), XOP5_FMT_MASK},
  {"+A,B",  XOP5_I(0) | XOP5_S1(0) | XOP5_SD(0), XOP5_FMT_MASK},
  {"A,+B",  XOP5_I(0) | XOP5_S1(0) | XOP5_SD(0), XOP5_FMT_MASK},
  {"+A,+B", XOP5_I(0) | XOP5_S1(0) | XOP5_SD(0), XOP5_FMT_MASK},
  {"-A,B",  XOP5_I(0) | XOP5_S1(0) | XOP5_SD(1), XOP5_FMT_MASK},
  {"-A,+B", XOP5_I(0) | XOP5_S1(0) | XOP5_SD(1), XOP5_FMT_MASK},
  {"A,-B",  XOP5_I(0) | XOP5_S1(1) | XOP5_SD(0), XOP5_FMT_MASK},
  {"+A,-B", XOP5_I(0) | XOP5_S1(1) | XOP5_SD(0), XOP5_FMT_MASK},
  {"-A,-B", XOP5_I(0) | XOP5_S1(1) | XOP5_SD(1), XOP5_FMT_MASK},
  { NULL, 0, 0},
};

static const my66000_fmt_spec_t ff1_fmt_list[] =
{
  { "A,B",    XOP5_BITS(0,0,0,0), XOP5_FMT_MASK | FF1_FMT_MASK},
  { "+A,B",   XOP5_BITS(0,0,0,0), XOP5_FMT_MASK | FF1_FMT_MASK},
  { "A,-B",   XOP5_BITS(0,0,0,1), XOP5_FMT_MASK | FF1_FMT_MASK},
  { "+A,-B",  XOP5_BITS(0,0,0,1), XOP5_FMT_MASK | FF1_FMT_MASK},
  { "A,#F",   XOP5_BITS(0,1,0,0), XOP5_FMT_MASK | FF1_FMT_MASK},
  { "+A,#F",  XOP5_BITS(0,1,0,0), XOP5_FMT_MASK | FF1_FMT_MASK},
  { "A,#-F",  XOP5_BITS(0,1,0,1), XOP5_FMT_MASK | FF1_FMT_MASK},
  { "+A,#-F", XOP5_BITS(0,1,0,1), XOP5_FMT_MASK | FF1_FMT_MASK},
  { "A,#O",   XOP5_BITS(1,0,0,0), XOP5_FMT_MASK | FF1_FMT_MASK},
  { "+A,#O",  XOP5_BITS(1,0,0,0), XOP5_FMT_MASK | FF1_FMT_MASK},
  { "A,#-O",  XOP5_BITS(1,0,0,1), XOP5_FMT_MASK | FF1_FMT_MASK},
  { "+A,#-O", XOP5_BITS(1,0,0,1), XOP5_FMT_MASK | FF1_FMT_MASK},
  { "A,#-P",  XOP5_BITS(1,1,0,1), XOP5_FMT_MASK | FF1_FMT_MASK},
  { "+A,#-P", XOP5_BITS(1,1,0,1), XOP5_FMT_MASK | FF1_FMT_MASK},
  { NULL, 0, 0}
};

static const my66000_fmt_spec_t vec32_fmt_list[] =
{
  {"A,{l}",  XOP5_I(1), SRC1_MASK | SRC2_MASK | XOP5_FMT_MASK},
  { NULL, 0, 0},
};

/* LOOP uses the same bits as XOP4.  */

static const my66000_fmt_spec_t loopu_fmt_list[] =
{
  { "m,B,N,C",   XOP4_BITS (0, 0, 0), XOP4_FMT_MASK},
  { "m,B,N,#G",  XOP4_BITS (0, 0, 1), XOP4_FMT_MASK},
  { "m,B,#o,C",  XOP4_BITS (0, 1, 0), XOP4_FMT_MASK},
  { "m,B,#o,#G", XOP4_BITS (0, 1, 1), XOP4_FMT_MASK},
  { "m,B,N,#L",  XOP4_BITS (1, 0, 0), XOP4_FMT_MASK | SRC2_MASK},
  { "m,B,#L,C",  XOP4_BITS (1, 0, 1), XOP4_FMT_MASK | SRC3_MASK},
  { "m,B,#T,#L", XOP4_BITS (1, 1, 0), XOP4_FMT_MASK | SRC2_MASK | SRC3_MASK},
  { "m,B,#1,#P", XOP4_BITS (1, 1, 0), XOP4_FMT_MASK | SRC2_MASK},
  { NULL, 0, 0},
};

static const my66000_fmt_spec_t loops_fmt_list[] =
{
  { "n,B,N,C",  XOP4_BITS (0, 0, 0), XOP4_FMT_MASK},
  { "n,B,N,#G", XOP4_BITS (0, 0, 1), XOP4_FMT_MASK},
  { "n,B,#o,C",  XOP4_BITS (0, 1, 0), XOP4_FMT_MASK},
  { "n,B,#o,#G", XOP4_BITS (0, 1, 1), XOP4_FMT_MASK},
  { "n,B,N,#L",  XOP4_BITS (1, 0, 0), XOP4_FMT_MASK | SRC2_MASK},
  { "n,B,#L,C",  XOP4_BITS (1, 0, 1), XOP4_FMT_MASK | SRC3_MASK},
  { "n,B,#T,#L", XOP4_BITS (1, 1, 0), XOP4_FMT_MASK | SRC2_MASK | SRC3_MASK},
  { "n,B,#1,#P", XOP4_BITS (1, 1, 0), XOP4_FMT_MASK | SRC2_MASK},
  { NULL, 0, 0},
};

static const my66000_fmt_spec_t svc_fmt_list[] =
{
  { "#i", 0, SRC1_MASK},
  { NULL, 0, 0},
};

/* NOP is all zeros except for the major opcode.  */
#define NOP_FMT_MASK (((uint32_t) -1) >> 6)

static const my66000_fmt_spec_t nop_fmt_list[] =
{
  {"", 0, NOP_FMT_MASK},
  { NULL, 0, 0},
};

/* Where to look up the operand list for a certain instruction
   format.  Warning: Keep this table in the same order as enum
   my66000_encoding in include/opcode/my66000.h, this will be checked
   on startup of gas.  */

const my66000_opcode_fmt_t my66000_opcode_fmt[] =
  {
   { NULL,              MY66000_BAD   },
   { empty_fmt_list,    MY66000_ILL   },
   { opimm_fmt_list,    MY66000_OPIMM },
   { mvimm_fmt_list,	MY66000_MVIMM },
   { mem_fmt_list,      MY66000_MEM   },
   { arith_fmt_list,    MY66000_ARITH },
   { bb1_fmt_list,	MY66000_BB1   },
   { br_fmt_list,       MY66000_BR    },
   { mrr_fmt_list,      MY66000_MRR   },
   { fmac_fmt_list,	MY66000_FMAC  },
   { mux_fmt_list,      MY66000_MUX   },
   { mov2_fmt_list,     MY66000_MOV2  },
   { mov3_fmt_list,     MY66000_MOV3  },
   { mux32_fmt_list,    MY66000_MUX32 },
   { mov32_fmt_list,    MY66000_MOV32 },
   { mux64_fmt_list,    MY66000_MUX64 },
   { mov64_fmt_list,    MY66000_MOV64 },
   { shift_fmt_list,    MY66000_SHIFT },
   { empty_fmt_list,    MY66000_EMPTY },
   { enter_fmt_list,    MY66000_ENTER },
   { bc_fmt_list,       MY66000_BC    },
   { carry_fmt_list,    MY66000_CARRY },
   { pb1_fmt_list,      MY66000_PB1   },
   { pc_fmt_list,       MY66000_PC    },
   { pcnd_fmt_list,     MY66000_PCND  },
   { bcnd_fmt_list,     MY66000_BCND  },
   { hrr_fmt_list,      MY66000_HRR   },
   { hrw_fmt_list,      MY66000_HRW   },
   { hrx_fmt_list,	MY66000_HRX   },
   { jmp_fmt_list,      MY66000_JMP   },
   { calli_fmt_list,    MY66000_CALLI },
   { ins_fmt_list,      MY66000_INS   },
   { vec_fmt_list,      MY66000_VEC   },
   { tt_fmt_list,       MY66000_TT    },
   { si5_fmt_list,      MY66000_SI5   },
   { si_fmt_list,       MY66000_SI    },
   { si_ldd_fmt_list,   MY66000_SI_STD},
   { mm_fmt_list,       MY66000_MM    },
   { float_fmt_list,    MY66000_FLOAT },
   { eadd_fmt_list,     MY66000_EADD  },
   { calx_fmt_list,     MY66000_CALX  },
   { op5_d_fmt_list,    MY66000_OP5_D },
   { abs_fmt_list,      MY66000_ABS   },
   { op5_f_fmt_list,    MY66000_OP5_F },
   { pop_fmt_list,      MY66000_POP   },
   { vec32_fmt_list,    MY66000_VEC32 },
   { cvtu_fmt_list,     MY66000_CVTU  },
   { cvts_fmt_list,     MY66000_CVTS  },
   { loopu_fmt_list,    MY66000_LOOPU },
   { loops_fmt_list,    MY66000_LOOPS },
   { svc_fmt_list,      MY66000_SVC   },
   { trans_fmt_list,    MY66000_TRANS },
   { ff1_fmt_list,      MY66000_FF1   },
   { nop_fmt_list,      MY66000_NOP   },
   { NULL,	        MY66000_END   },
  };

/* Some helper functions for relaxation in the assembler.  In theory,
   this is redundant with the tables above. In practice, getting the
   info from the table is simply too much hassle.  */

/* Inquire the size of the immediate, if any.  */

int
my66000_imm_size (uint32_t iword)
{
  uint32_t major = (iword & MY66000_MAJOR_MASK);
  if (major != MAJOR(9) && major != MAJOR (10))
    return 0;

  if ((iword & XOP1_d_MASK) == 0)
    return 0;

  return iword & XOP1_D_MASK ? 8 : 4;
}

/* Set the size of the immediate (with sanity check).  */

uint32_t
my66000_set_imm_size (uint32_t iword, uint32_t size)
{
  uint32_t major = (iword & MY66000_MAJOR_MASK);
  uint32_t rmask = -1 ^ (XOP1_D_MASK | XOP1_d_MASK);
  uint32_t ret;

  if (major != MAJOR(9) && major != MAJOR (10))
    {
      fprintf (stderr, "Internal error: set_size on illecal opcode %u\n",
	       major >> MY66000_MAJOR_SHIFT);
      exit(EXIT_FAILURE);
    }
  ret = iword & rmask;
  switch (size)
    {
    case 4:
      ret |= XOP1_D_MASK;
      break;
    case 8:
      ret |= XOP1_D_MASK | XOP1_d_MASK;
      break;
    default:
      fprintf (stderr, "Internal error: set_size with illegal size %u\n",
	       size);
      exit(EXIT_FAILURE);
    }
  return ret;
}

/* Check if an instruction is a TT instruction, for use in the assembler.  */

bool
my66000_is_tt (uint32_t iword)
{
  return ((iword ^ MAJOR(27)) & MAJOR_MASK) == 0;
}

/* Return size the of the TT .jt "arguments" from the
   instruction.  */

uint32_t
my66000_get_tt_size (uint32_t iword)
{
  uint32_t fld;
  assert (my66000_is_tt (iword));
  fld = (iword & TT_SIZE_MASK) >> TT_SIZE_OFFS;
  return 1 << fld;
}

/* Set the size of the TT .jt "arguments" from the
   instruction.  */
uint32_t
my66000_set_tt_size (uint32_t iword, uint32_t size)
{
  uint32_t rmask = -1 ^ TT_SIZE_MASK;
  uint32_t ret;

  assert (my66000_is_tt (iword));

  ret = iword & rmask;
  switch (size)
    {
    case 1:
      break;
    case 2:
      ret |= TT_SIZE(1);
      break;
    case 4:
      ret |= TT_SIZE(2);
      break;
    case 8:
      ret |= TT_SIZE(3);
      break;
    default:
      fprintf (stderr,"Internal error: my66000_set_tt_size\n");
      exit (EXIT_FAILURE);
    }
  return ret;
}

bool
my66000_is_mem (uint32_t iword)
{
  return (iword >> MY66000_MAJOR_SHIFT) == 9;
}

uint32_t
my66000_set_mem_size (uint32_t iword, uint32_t size)
{
  uint32_t mask = -1 ^ XOP1_d_MASK;
  uint32_t bit, res;
  assert (my66000_is_mem (iword));
  assert (XOP1_D(iword));
  switch (size)
    {
    case 4:
      bit = 0;
      break;
    case 8:
      bit = XOP1_d(1);
      break;
    default:
      abort();
    }
  res = (iword & mask) | bit;
  return res;
}

/* Return true if this is a "store immediate" instruction which
   requires a 32-bit or 64-bit immediate.  */

bool
my66000_is_imm_st (uint32_t iword)
{
  uint32_t major = (iword & MAJOR_MASK) >> MY66000_MAJOR_SHIFT;
  uint32_t minor = (iword & MINOR_MASK) >> MINOR_OFFS;
  return major == 9 &&  minor >= 24 && minor <= 27;
}

bool
my66000_is_loop (uint32_t iword)
{
  uint32_t major = (iword & MAJOR_MASK) >> MY66000_MAJOR_SHIFT;
  return major == 29;
}

/* Return the size field hidden in the opcode for the store immedaite
   instruction.  */

uint32_t
my66000_get_imm_sz (uint32_t iword)
{
  assert (my66000_is_imm_st (iword) || my66000_is_loop (iword));
  return (iword >> MINOR_OFFS) & 3;
}

/* Checks if an instruction is something that can be relaxed to or from
   CALL.  */

bool
my66000_is_call (uint32_t iword)
{
  uint32_t major = (iword & MAJOR_MASK) >> MY66000_MAJOR_SHIFT;
  uint32_t minor = (iword & MINOR_MASK) >> MINOR_OFFS;
  return major == 31 || (major == 9 && minor == 39);
}

/* Return the iword for a CALL offset or CALA [ip,offset].  */

uint32_t
my66000_get_call (int size)
{
  uint32_t ret;

  switch(size)
    {
    case 0:
      ret = MAJOR(31);
      break;
    case 4:
      ret = MAJOR(9) | MINOR(39) | XOP1_D(1) | XOP1_d(0);
      break;
    case 8:
      ret = MAJOR(9) | MINOR(39) | XOP1_D(1) | XOP1_D(0);
      break;
    default:
      abort();
    }
  return ret;
}
