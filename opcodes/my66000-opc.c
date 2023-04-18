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
#include "disassemble.h"
#include "opcode/my66000.h"

/* Macro so we can fill up the frag_opc, frag_mask and shift
   members more easily.  */

#define MAJOR(c) ((c) << MY66000_MAJOR_SHIFT)
#define MAJOR_MASK (63 << MAJOR_OFFS)

#define SHFT_OFFS 12
#define SHFT_MASK 15 << SHFT_OFFS

#define MINOR_OFFS 5
#define MINOR(c) ((c) << MINOR_OFFS)
#define MINOR_MASK MINOR(63)

#define XOP4_OFFS 10
#define XOP4_MINOR(c) ((c) << XOP4_OFFS)
#define XOP4_MASK XOP4_MINOR(7)

#define BB1_OFFS 21
#define BB1_MINOR(c) ((c) << BB1_OFFS)
#define BB1_MASK 31 << BB1_OFFS


#define BCND_OFFS 21
#define BCND_MASK 31 << BCND_OFFS

#define TT_OFFS 23
#define TT_MASK 7 << TT_OFFS

#define SHFT_MINOR(c) (c) << SHFT_OFFS

static const my66000_opc_info_t opc_om6[] =
{
 { "pb1",  MAJOR(6) | SHFT_MINOR( 0), MY66000_PB1A,  NULL, 0, 0},
 { "pcnd", MAJOR(6) | SHFT_MINOR( 1), MY66000_PCND,  NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR( 2), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR( 3), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR( 4), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR( 5), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR( 6), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR( 7), MY66000_BAD,   NULL, 0, 0},
 { "srl",  MAJOR(6) | SHFT_MINOR( 8), MY66000_SHIFT, NULL, 0, 0},
 { "sra",  MAJOR(6) | SHFT_MINOR( 9), MY66000_SHIFT, NULL, 0, 0},
 { "sll",  MAJOR(6) | SHFT_MINOR(10), MY66000_SHIFT, NULL, 0, 0},
 { "sla",  MAJOR(6) | SHFT_MINOR(11), MY66000_SHIFT, NULL, 0, 0},
 { "bitr", MAJOR(6) | SHFT_MINOR(12), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR(13), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR(14), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(6) | SHFT_MINOR(15), MY66000_BAD,   NULL, 0, 0},
 { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

static const my66000_opc_info_t opc_om7[] =
{
 { "pb1",  MAJOR(7) | SHFT_MINOR( 0), MY66000_PB1B,  NULL, 0, 0},
 { "pcnd", MAJOR(7) | SHFT_MINOR( 1), MY66000_PCND,  NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR( 2), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR( 3), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR( 4), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR( 5), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR( 6), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR( 7), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR( 8), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR( 9), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR(10), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR(11), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR(12), MY66000_SHIFT, NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR(13), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR(14), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(7) | SHFT_MINOR(15), MY66000_BAD,   NULL, 0, 0},
 { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

#define SIGNED_SHFT 12
#define SIGNED(c) ((c) << SIGNED_SHFT)
#define SIGNED_MASK SIGNED (1)

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

#define MRR_FMT_MASK (XOP1_D_MASK | XOP1_SCALE_MASK | XOP1_d_MASK)

/* Mask for rindex=0 */

#define RIND_ZERO_MASK 31

/* Mask for instruction pointer as SRC1.  */

#define IP_MASK (31 << 16)

#define XOP1_BITS(D,scale,d) (XOP1_D(D) | XOP1_SCALE(scale) | XOP1_d(d))

/* Definition for the various modifier bits in the XOP2 group.  */

#define XOP2_I_SHFT 15
#define XOP2_I(c) ((c) << XOP2_I_SHFT)
#define XOP2_I_MASK XOP2_I(1)

#define XOP2_S1_SHFT 14
#define XOP2_S1(c) ((c) << XOP2_S1_SHFT)
#define XOP2_S1_MASK XOP2_S1(1)

#define XOP2_S2_SHFT 13
#define XOP2_S2(c) ((c) << XOP2_S2_SHFT)
#define XOP2_S2_MASK XOP2_S2(1)

/* The S bit, at position 12, is already used in the opcode tables
   above.  */

#define XOP2_d_SHFT 11
#define XOP2_d(c) ((c) << XOP2_d_SHFT)
#define XOP2_d_MASK XOP2_d(1)

#define XOP2_MASK (XOP2_I(1) | XOP2_S1(1) | XOP2_S2(1) | XOP2_d(1))

/* Write the above more compactly.  */

#define XOP2_BITS(I,d,S1,S2) (XOP2_I(I) | XOP2_d(d) | XOP2_S1(S1) | XOP2_S2(S2))

/* FMAC uses some of same bits as XOP2.  */

#define XOP4_BITS(I,S1,S2) (XOP2_I(I) | XOP2_S1(S1) | XOP2_S2(S2))
#define XOP4_FMT_MASK XOP4_BITS (1,1,1)
#define XOP4_FMT_SHFT 13


static const my66000_opc_info_t opc_mrr[] =
{
 { "ldub",  MAJOR(9) | MINOR( 0) | XOP1_L(0), MY66000_MRR, NULL, 0, 0},
 { "ldubl", MAJOR(9) | MINOR( 0) | XOP1_L(1), MY66000_MRR, NULL, 0, 0},
 { NULL,   0,        MY66000_END, NULL, 0, 0}
};

static const my66000_opc_info_t opc_op1[] =
{
 { NULL,   MAJOR(9) | MINOR ( 0), MY66000_BAD, opc_mrr, XOP1_L_MASK, XOP1_L_SHFT},
 { "lduh", MAJOR(9) | MINOR ( 1), MY66000_MRR, NULL, 0, 0},
 { "lduw", MAJOR(9) | MINOR ( 2), MY66000_MRR, NULL, 0, 0},
 { "ldd",  MAJOR(9) | MINOR ( 3), MY66000_MRR, NULL, 0, 0},
 { "ldsb", MAJOR(9) | MINOR ( 4), MY66000_MRR, NULL, 0, 0},
 { "ldsh", MAJOR(9) | MINOR ( 5), MY66000_MRR, NULL, 0, 0},
 { "ldsw", MAJOR(9) | MINOR ( 6), MY66000_MRR, NULL, 0, 0},
 { "la",   MAJOR(9) | MINOR ( 7), MY66000_MRR, NULL, 0, 0},
 { "stb",  MAJOR(9) | MINOR ( 8), MY66000_MRR, NULL, 0, 0},
 { "sth",  MAJOR(9) | MINOR ( 9), MY66000_MRR, NULL, 0, 0},
 { "stw",  MAJOR(9) | MINOR (10), MY66000_MRR, NULL, 0, 0},
 { "std",  MAJOR(9) | MINOR (11), MY66000_MRR, NULL, 0, 0},
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
 { "stb",  MAJOR(9) | MINOR (24), MY66000_BAD, NULL, 0, 0}, /* !!! */
 { NULL,   MAJOR(9) | MINOR (25), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (26), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (27), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (28), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (29), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (30), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (31), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (32), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (33), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (34), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (35), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (36), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (37), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (38), MY66000_BAD, NULL, 0, 0},
 { NULL,   MAJOR(9) | MINOR (39), MY66000_BAD, NULL, 0, 0},
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


static const my66000_opc_info_t opc_arith[] =
{
 {"add",  MAJOR(10) | MINOR(33) | SIGNED(0), MY66000_ARITH, NULL, 0, 0},
 {"adds", MAJOR(10) | MINOR(33) | SIGNED(1), MY66000_ARITH, NULL, 0, 0},
 { NULL,   0,        MY66000_END, NULL, 0, 0}
};

static const my66000_opc_info_t opc_op2[] =
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
 { NULL,    MINOR (33), MY66000_BAD,  opc_arith, SIGNED_MASK, SIGNED_SHFT},
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

static const my66000_opc_info_t opc_mpx[] =
{
 { "mux",  MAJOR (12) | XOP4_MINOR(1) | XOP4_BITS (0,0,0), MY66000_MUX, NULL, 0, 0},
 { "cmov", MAJOR (12) | XOP4_MINOR(1) | XOP4_BITS (0,0,1), MY66000_MUX, NULL, 0, 0},
 { "mov",  MAJOR (12) | XOP4_MINOR(1) | XOP4_BITS (0,1,0), MY66000_MOV2, NULL, 0, 0},
 { "mov",  MAJOR (12) | XOP4_MINOR(1) | XOP4_BITS (0,1,1), MY66000_MOV3, NULL, 0, 0},
 { "mux",  MAJOR (12) | XOP4_MINOR(1) | XOP4_BITS (1,0,0), MY66000_MUX32, NULL, 0, 0},
 { "mov",  MAJOR (12) | XOP4_MINOR(1) | XOP4_BITS (1,0,1), MY66000_MOV32, NULL, 0, 0},
 { "mux",  MAJOR (12) | XOP4_MINOR(1) | XOP4_BITS (1,1,0), MY66000_MUX64, NULL, 0, 0},
 { "mov",  MAJOR (12) | XOP4_MINOR(1) | XOP4_BITS (1,1,1), MY66000_MOV64, NULL, 0, 0},
 { NULL,   0,                                              MY66000_END, NULL, 0, 0},
};

static const my66000_opc_info_t opc_op4[] =
{
 { "fmac",   MAJOR (12) | XOP4_MINOR(0), MY66000_FMAC, NULL, 0, 0},
 { NULL,     MAJOR (12) | XOP4_MINOR(1), MY66000_BAD,  opc_mpx, XOP4_FMT_MASK,
   XOP4_FMT_SHFT },  /* MPX */
 { NULL,     MAJOR (12) | XOP4_MINOR(2), MY66000_BAD,  NULL, 0, 0},  /* INS */
 { NULL,     MAJOR (12) | XOP4_MINOR(3), MY66000_BAD,  NULL, 0, 0},  /* empty */
 { "fmacs",  MAJOR (12) | XOP4_MINOR(4), MY66000_FMAC, NULL, 0, 0},
 { NULL,     MAJOR (12) | XOP4_MINOR(5), MY66000_BAD,  NULL, 0, 0},  /* empty */
 { NULL,     MAJOR (12) | XOP4_MINOR(6), MY66000_BAD,  NULL, 0, 0},  /* empty */
 { NULL,     MAJOR (12) | XOP4_MINOR(7), MY66000_FMAC, NULL, 0, 0},  /* Loop */
 { NULL,   0,              MY66000_END,   NULL, 0, 0},
};

static const my66000_opc_info_t opc_op5[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

static const my66000_opc_info_t opc_bcnd[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

static const my66000_opc_info_t opc_jt[] =
{
  { NULL,   0,              MY66000_END,   NULL, 0, 0}
};

/* FIXME: Disassembling is ambigous.  */

/* The two halves of the branch on bit instructions.  */
static const my66000_opc_info_t opc_bb1a[] =
{
 { "bne",  MAJOR(24) | BB1_MINOR( 0), MY66000_BB1A, NULL, 0, 0},
 { "beq",  MAJOR(24) | BB1_MINOR( 1), MY66000_BB1A, NULL, 0, 0},
 { "bgt",  MAJOR(24) | BB1_MINOR( 2), MY66000_BB1A, NULL, 0, 0},
 { "bge",  MAJOR(24) | BB1_MINOR( 3), MY66000_BB1A, NULL, 0, 0},
 { "blt",  MAJOR(24) | BB1_MINOR( 4), MY66000_BB1A, NULL, 0, 0},
 { "ble",  MAJOR(24) | BB1_MINOR( 5), MY66000_BB1A, NULL, 0, 0},
 { "bor",  MAJOR(24) | BB1_MINOR( 6), MY66000_BB1A, NULL, 0, 0},
 { "bun",  MAJOR(24) | BB1_MINOR( 7), MY66000_BB1A, NULL, 0, 0},
 { "bnne", MAJOR(24) | BB1_MINOR( 8), MY66000_BB1A, NULL, 0, 0},
 { "bneq", MAJOR(24) | BB1_MINOR( 9), MY66000_BB1A, NULL, 0, 0},
 { "bhi",  MAJOR(24) | BB1_MINOR(10), MY66000_BB1A, NULL, 0, 0},
 { "bhs",  MAJOR(24) | BB1_MINOR(11), MY66000_BB1A, NULL, 0, 0},
 { "blo",  MAJOR(24) | BB1_MINOR(12), MY66000_BB1A, NULL, 0, 0},
 { "bls",  MAJOR(24) | BB1_MINOR(13), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(14), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(15), MY66000_BB1A, NULL, 0, 0},
 { "sin",  MAJOR(24) | BB1_MINOR(16), MY66000_BB1A, NULL, 0, 0},
 { "fin",  MAJOR(24) | BB1_MINOR(17), MY66000_BB1A, NULL, 0, 0},
 { "cin",  MAJOR(24) | BB1_MINOR(18), MY66000_BB1A, NULL, 0, 0},
 { "cin",  MAJOR(24) | BB1_MINOR(19), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(20), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(21), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(23), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(24), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(25), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(26), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(27), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(28), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(29), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(30), MY66000_BB1A, NULL, 0, 0},
 { NULL,   MAJOR(24) | BB1_MINOR(31), MY66000_BB1A, NULL, 0, 0},
 { NULL,   0,                                 MY66000_END , NULL, 0, 0}
};

static const my66000_opc_info_t opc_bb1b[] =
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
 { "bb1",  MAJOR(24), MY66000_BB1A,  opc_bb1a, BB1_MASK,  BB1_OFFS},
 { "bb1",  MAJOR(25), MY66000_BB1B,  opc_bb1b, BB1_MASK,  BB1_OFFS},
 { "bcnd", MAJOR(26), MY66000_BCND,  opc_bcnd, BCND_MASK, BCND_OFFS},
 { NULL,   MAJOR(27), MY66000_BAD,   opc_jt,   TT_MASK,   TT_OFFS},
 { NULL,   MAJOR(28), MY66000_BAD,   NULL, 0, 0},
 { NULL,   MAJOR(29), MY66000_BAD,   NULL, 0, 0},
 { "br",   MAJOR(30), MY66000_BR,    NULL, 0, 0},
 { "call", MAJOR(31), MY66000_BR,    NULL, 0, 0},
 { "ldub", MAJOR(32), MY66000_MEM,   NULL, 0, 0},
 { "lduh", MAJOR(33), MY66000_MEM,   NULL, 0, 0},
 { "lduw", MAJOR(34), MY66000_MEM,   NULL, 0, 0},
 { "ldd",  MAJOR(35), MY66000_MEM,   NULL, 0, 0},
 { "ldsb", MAJOR(36), MY66000_MEM,   NULL, 0, 0},
 { "ldsh", MAJOR(37), MY66000_MEM,   NULL, 0, 0},
 { "ldsw", MAJOR(38), MY66000_MEM,   NULL, 0, 0},
 { "exit", MAJOR(39), MY66000_EXIT,  NULL, 0, 0},
 { "stb",  MAJOR(40), MY66000_MEM,   NULL, 0, 0},
 { "sth",  MAJOR(41), MY66000_MEM,   NULL, 0, 0},
 { "stw",  MAJOR(42), MY66000_MEM,   NULL, 0, 0},
 { "std",  MAJOR(43), MY66000_MEM,   NULL, 0, 0},
 { "enter", MAJOR(44), MY66000_EXIT, NULL, 0, 0},
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
 { "or",   MAJOR(56), MY66000_OPIMM, NULL, 0, 0},
 { "xor",  MAJOR(57), MY66000_OPIMM, NULL, 0, 0},
 { "and",  MAJOR(58), MY66000_OPIMM, NULL, 0, 0},
 { "mov",  MAJOR(59), MY66000_MVIMM, NULL, 0, 0},
 { "carry", MAJOR(60), MY66000_CARRY, NULL, 0, 0},
 { "vec",  MAJOR(61), MY66000_VEC,   NULL, 0, 0},
 { NULL,   MAJOR(62), MY66000_BAD,   NULL, 0, 0},
 { "ill5", MAJOR(63), MY66000_BAD,   NULL, 0, 0},
 { NULL,   0,         MY66000_END,   NULL, 0, 0},
};

/* List of all the tables containing opcodes, for initializing the
   hashtabs for gas.  Keep up to date if you add anything above,
   otherwise the instructions will not be found.  */

const my66000_opc_info_t *my66000_opc_info_list[] =
{
 my66000_opc_info, opc_om6, opc_om7, opc_arith, opc_op1, opc_op2, opc_op4,
 opc_op5, opc_bcnd, opc_jt, opc_bb1a, opc_bb1b, opc_mrr, opc_mpx, NULL
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
    "r24", "r25", "r26", "r27", "r28", "r29", "r30", "sp"
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

#define OPERAND_MASK(width,shift) ((1u << (width)) - 1) << (shift)
#define OPERAND_ENTRY(width,shift) OPERAND_MASK(width,shift), shift, 0, 0

/* Dept. of dirty tricks: Operands are encoded in the format string
   by capital letters starting with A.  Lookup is then via
   my66000_operand_table[c - 'A'].  An alternative would be to have
   strings using more descriptive names, like "dst" and put them in a
   hash, but that would create unnecessary overhead during
   parsing.

   The ordering is not otherwise significant, it just happens to be the
   way it came up during development.

   Immediates have a size and a sequence (important for output of the
   disassembler).

*/

const my66000_operand_info_t my66000_operand_table[] =
{
 {MY66000_OPS_DST,    OPERAND_ENTRY ( 5, 21), "Destination register",    'A' },
 {MY66000_OPS_SRC1,   OPERAND_ENTRY ( 5, 16), "Source register 1",       'B' },
 {MY66000_OPS_SRC2,   OPERAND_ENTRY ( 5,  0), "Source register 2",       'C' },
 {MY66000_OPS_RINDEX, OPERAND_ENTRY ( 5,  0), "Index register",          'D' },
 {MY66000_OPS_IMM16,  OPERAND_ENTRY (16,  0), "16-bit signed immediate", 'E' },
 {MY66000_OPS_I1,     OPERAND_ENTRY ( 5, 16), "5-bit immediate SRC1",    'F' },
 {MY66000_OPS_I2,     OPERAND_ENTRY ( 5,  0), "5-bit immediate SRC2",    'G' },
 {MY66000_OPS_BB1,    OPERAND_ENTRY ( 6, 21), "Bit number",		 'H' },
 {MY66000_OPS_B16,    OPERAND_ENTRY (16,  0), "16-bit branch target",	 'I' },
 {MY66000_OPS_B26,    OPERAND_ENTRY (26,  0), "26-bit branch target",	 'J' },
 {MY66000_OPS_RBASE,  OPERAND_ENTRY ( 5, 16), "Base register",           'K' },
 {MY66000_OPS_I32_1,     0, 0, 4, 1,          "32-bit immediate SRC1",   'L' },
 {MY66000_OPS_I32_PCREL, 0, 0, 4, 1,          "32-bit immediate ip-rel", 'M' },
 {MY66000_OPS_SRC3,   OPERAND_ENTRY (5,   5), "Source register 3",       'N' },
 {MY66000_OPS_I32_HEX,   0, 0, 4, 1,          "32-bit hex immediate",    'O' },
 {MY66000_OPS_I64_1,     0, 0, 8, 1,          "64-bit immediate SRC1",   'P' },
 {MY66000_OPS_I64_PCREL, 0, 0, 8, 1,          "64-bit immediate ip-rel", 'Q' },
 {MY66000_OPS_I64_HEX,   0, 0, 8, 1,          "64-bit hex immediate",    'R' },
 {0,                     0, 0, 0, 0,          "unused",                  'S' },

 {MY66000_OPS_I32_ST,    0, 0, 4, 2,          "32-bit immediate store",  'T' },
 {MY66000_OPS_I64_ST,    0, 0, 8, 2,          "64-bit immediate store",  'U' },
};

/* My 66000 has instructions for which modifiers depend on the
   operands.  This section deals with encoding those.  */

/* Some patterns have a requirement of zeros where otherwise
   registers would be.  Compare to my66000_operand_table above.  */

#define SRC1_MASK OPERAND_MASK(5,16)
#define SRC2_MASK OPERAND_MASK(5, 0)

static const my66000_fmt_spec_t opimm_fmt_list[] =
{
 { "A,B,#E", 0, 0, 0},
 { NULL,     0, 0, 0},

};

static const my66000_fmt_spec_t mvimm_fmt_list[] =
{
 { "A,#E", 0, 0, 0},
 { NULL,   0, 0, 0}
};

static const my66000_fmt_spec_t mem_fmt_list[] =
{
 { "A,[B]",   0, 0xffff, 0},
 { "A,[B,E]", 0, 0,      0},
 { NULL,      0, 0,      0},
};

/* This is table 13-2, 2-Operand Specification.  */

static const my66000_fmt_spec_t arith_fmt_list [] =
{
 { "A,B,C",    XOP2_BITS (0,0,0,0), XOP2_MASK, 0},
 { "A,B,-C",   XOP2_BITS (0,0,0,1), XOP2_MASK, 0},
 { "A,-B,C",   XOP2_BITS (0,0,1,0) ,XOP2_MASK, 0},
 { "A,-B,-C",  XOP2_BITS (0,0,1,1), XOP2_MASK, 0},

 { "A,B,#F",   XOP2_BITS (0,1,0,0), XOP2_MASK, 0},
 { "A,#G,C",   XOP2_BITS (0,1,0,1), XOP2_MASK, 0},
 { "A,B,#-F",  XOP2_BITS (0,1,1,0), XOP2_MASK, 0},
 { "A,#-G,C",  XOP2_BITS (0,1,1,1), XOP2_MASK, 0},

 { "A,B,#L",   XOP2_BITS (1,0,0,0), XOP2_MASK, 0},
 { "A,#L,C",   XOP2_BITS (1,0,0,1), XOP2_MASK, 0},
 { "A,-B,#L",  XOP2_BITS (1,0,1,0), XOP2_MASK, 0},
 { "A,#L,-C",  XOP2_BITS (1,0,1,1), XOP2_MASK, 0},

 { "A,B,#P",   XOP2_BITS (1,1,0,0), XOP2_MASK, 0},
 { "A,#P,C",   XOP2_BITS (1,1,0,1), XOP2_MASK, 0},
 { "A,-B,#P",  XOP2_BITS (1,1,1,0), XOP2_MASK, 0},
 { "A,#P,-C",  XOP2_BITS (1,1,1,1), XOP2_MASK, 0},
 { NULL,      0, 0, 0},
};

static const my66000_fmt_spec_t bb1_fmt_list [] =
{
 { "H,B,I", 0, 0, 0},
 { NULL,    0, 0, 0},
};

static const my66000_fmt_spec_t br_fmt_list [] =
{
 { "J",    0,  0, 0},
 { NULL,   0,  0, 0},
};

/* Table for load/store with two registers.  */

static const my66000_fmt_spec_t mrr_fmt_list [] =
{
 /* Different synatax variants for scaled.  */
 { "A,[K,D,0]",    XOP1_BITS(0,0,0), MRR_FMT_MASK, 0},
 { "A,[K,D<<0]",   XOP1_BITS(0,0,0), MRR_FMT_MASK, 0},
 { "A,[K,D<<0,0]", XOP1_BITS(0,0,0), MRR_FMT_MASK, 0},
 { "A,[K,D<<1]",   XOP1_BITS(0,1,0), MRR_FMT_MASK, 0},
 { "A,[K,D<<1,0]", XOP1_BITS(0,1,0), MRR_FMT_MASK, 0},
 { "A,[K,D<<2]",   XOP1_BITS(0,2,0), MRR_FMT_MASK, 0},
 { "A,[K,D<<2,0]", XOP1_BITS(0,2,0), MRR_FMT_MASK, 0},
 { "A,[K,D<<3]",   XOP1_BITS(0,3,0), MRR_FMT_MASK, 0},
 { "A,[K,D<<3,0]", XOP1_BITS(0,3,0), MRR_FMT_MASK, 0},

 /* FIXME:  Relocations for symbols which are not offset
    with respect to the IP are poorly defined at the moment.  */

 /* IP-relative, without register offset, and an IP-relative 32-bit
    and 64-bit relocations, respectively.  */
 { "A,[K,M]",      XOP1_BITS(0,0,1), MRR_FMT_MASK | RIND_ZERO_MASK | IP_MASK, 1},
 { "A,[K,Q]",      XOP1_BITS(1,0,1), MRR_FMT_MASK | RIND_ZERO_MASK | IP_MASK, 0},
 { NULL, 0, 0, 0},
};

/* Table 14: 3-Operand Constant Specification.  */

static const my66000_fmt_spec_t fmac_fmt_list [] =
{
 { "A,B,C,N",   XOP4_BITS (0,0,0), XOP4_FMT_MASK, 0},
 { "A,B,C,-N",  XOP4_BITS (0,0,1), XOP4_FMT_MASK, 0},
 { "A,B,-C,N",  XOP4_BITS (0,1,0), XOP4_FMT_MASK, 0},
 { "A,B,-C,-N", XOP4_BITS (0,1,1), XOP4_FMT_MASK, 0},
 { "A,B,C,#O",  XOP4_BITS (1,0,0), XOP4_FMT_MASK, 0},
 { "A,B,#O,N",  XOP4_BITS (1,0,1), XOP4_FMT_MASK, 0},
 { "A,B,C,#R",  XOP4_BITS (1,1,0), XOP4_FMT_MASK, 0},
 { "A,B,#R,N",  XOP4_BITS (1,1,1), XOP4_FMT_MASK, 0},
 { NULL, 0, 0, 0}
};

static const my66000_fmt_spec_t mux_fmt_list[] =
{
 { "A,B,C",    0, 0, 0},
 { NULL,       0, 0, 0},
};

static const my66000_fmt_spec_t mov2_fmt_list[] =
{
 { "A,C",      0, 0, 0},
 { NULL,       0, 0, 0},
};

static const my66000_fmt_spec_t mov3_fmt_list[] =
{
 { "A,N",      0, 0, 0},
 { NULL,       0, 0, 0},
};

static const my66000_fmt_spec_t mux32_fmt_list[] =
{
 { "A,B,#L",   0, 0, 0},
 { NULL,       0, 0, 0},
};

static const my66000_fmt_spec_t mov32_fmt_list[] =
{
 { "A,#L",     0, 0, 0},
 { NULL,       0, 0, 0},
};

static const my66000_fmt_spec_t mux64_fmt_list[] =
{
 { "A,B,#P",   0, 0, 0},
 { NULL,       0, 0, 0},
};

static const my66000_fmt_spec_t mov64_fmt_list[] =
{
 { "A,#P",     0, 0, 0},
 { NULL,       0, 0, 0},
};

/* Where to look up the operand list for a certain instruction format.
   Warning: Keep this table in the same order as my66000_encoding in
   include/opcode/my66000.h, this will be checked on startup of gas.  */

const my66000_opcode_fmt_t my66000_opcode_fmt[] =
  {
   { NULL,              MY66000_BAD,    0},
   { NULL,              MY66000_ILL,    0},
   { opimm_fmt_list,    MY66000_OPIMM,  0},
   { mvimm_fmt_list,	MY66000_MVIMM,  0},
   { mem_fmt_list,      MY66000_MEM,    0},
   { arith_fmt_list,    MY66000_ARITH,  0},
   { bb1_fmt_list,	MY66000_BB1A,	0},
   { bb1_fmt_list,	MY66000_BB1B,	0},
   { br_fmt_list,       MY66000_BR,     0},
   { mrr_fmt_list,      MY66000_MRR,    0},
   { fmac_fmt_list,	MY66000_FMAC,   0},
   { mux_fmt_list,      MY66000_MUX,    0},
   { mov2_fmt_list,     MY66000_MOV2,   0},
   { mov3_fmt_list,     MY66000_MOV3,   0},
   { mux32_fmt_list,    MY66000_MUX32,  0},
   { mov32_fmt_list,    MY66000_MOV32,  0},
   { mux64_fmt_list,    MY66000_MUX64,  0},
   { mov64_fmt_list,    MY66000_MOV64,  0},
   { NULL,	        MY66000_END,    0},
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
      fprintf (stderr, "Internal error: set_size on illecal opcode %u",
	       major >> MY66000_MAJOR_SHIFT);
      exit(EXIT_FAILURE);
    }
  ret = iword & rmask;
  switch (size)
    {
    case 4:
      ret |= XOP1_d_MASK;
      break;
    case 8:
      ret |= XOP1_d_MASK | XOP1_D_MASK;
      break;
    default:
      fprintf (stderr, "Internal error: set_size with illegal size %u",
	       size);
      exit(EXIT_FAILURE);
    }
  return ret;
}
