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

/* Instructios and instruction classes.	 */

#ifndef MY66000_H
#define MY66000_H

#define MY66000_MAJOR_SHIFT 26
#define MY66000_MAJOR_MASK (63 << MY66000_MAJOR_SHIFT)

/* This is for looking up the opcode formats in my66000_opcode_fmt
   in my66000_opc.c . If you add an encoding here, you also have to
   add it there at the identical place.	 */

typedef enum my66000_encoding
{
 MY66000_END = -1,
 MY66000_BAD = 0, /* Empty.  */
 MY66000_ILL,	  /* Reserved as non-opcodes.	*/
 MY66000_OPIMM,	  /* Arithmetic with a 16-bit immediate.  */
 MY66000_MVIMM,	  /* Move 16-bit immediate.  */
 MY66000_MEM,	  /* Load/store with a 16-bit offset.	*/
 MY66000_ARITH,	  /* Arithmetic operations.  */
 MY66000_ARITHS0, /* Same, with the S bit set to zero.	 */
 MY66000_BB1A,	  /* Branch on bit set, explicit bit 0-31.  */
 MY66000_BB1B,	  /* Branch on bit set, explicit bit 32-63.  */
 MY66000_BR,	  /* Branch, call etc.	 */
 MY66000_MRR,	  /* Indexed memory operation, [Ra,Rb]	 */
 MY66000_MRRL0,	  /* Indexed memory operation, [Ra,Rb], with the L bit forced to 0.  */
 MY66000_FMAC,	  /* FMAC instruction.	 */
 MY66000_MUX,	  /* MUX and CMOV.  */
 MY66000_MOV2,	  /* Move from SRC2.  */
 MY66000_MOV3,	  /* Move from SRC3.  This is redundant, but we suport it. */
 MY66000_MUX32,	  /* MUX with a 32-bit immediate selector.  */
 MY66000_MOV32,	  /* MOV with a 32-bit immediate.  */
 MY66000_MUX64,	  /* MUX with a 64-bit immediate selector.  */
 MY66000_MOV64,	  /* MOV with a 64-bit immediate.  */
 MY66000_SHIFT,	  /* Shift formats with 6-bit immediates.  */
 MY66000_EMPTY,	  /* No argument list.	 */
 MY66000_ENTER,	  /* Enter instruction.	 */
 MY66000_EXIT,	  /* Exit instruction.	 */
 MY66000_BC,	  /* Conditional branch on bit set, named constant */
 MY66000_CARRY,	  /* Carry modifier.  */
 MY66000_PB1A,	  /* Predicate on bit set, explicit bit, bit 0-31  */
 MY66000_PB1B,	  /* Predicate on bit set, explicit bit, bit 31-64  */
 MY66000_PC,	  /* Predicate on bit set, from comparison  */
 MY66000_PCND,	  /* Predicate on condition.  */
 MY66000_BCND,	  /* Branch on condition.  */
 MY66000_HRR,	  /* HR, read special register.	 */
 MY66000_HRW,	  /* HR, write special register.  */
 MY66000_HRX,	  /* HR, exchange.  */
 MY66000_JMP,	  /* JMP instruction, special case of HR.  */
 MY66000_CALLI,	  /* CALLI instruction, special case of HR.  */
 MY66000_INS,	  /* INS instruction.	*/
 MY66000_VEC,	  /* Vector instruction, with immediate.  */
 MY66000_TT,	  /* Table transfer.  */
 MY66000_SI5,	  /* Store immediate with five-bit operands.  */
 MY66000_SI,	  /* Store immediate.	*/
 MY66000_SI_STD,  /* Store immediate doubleword.  */
 MY66000_MM,	  /* Load/store multiple.  */
 MY66000_FLOAT,	  /* FADD and friends.	 */
 MY66000_EADD,	  /* EADD, all by itself.  */
 MY66000_CALX,	  /* CALX and CALA  */
 MY66000_OP5_D,	  /* Double-precision single operand, expon and fract	*/
 MY66000_ABS,	  /* ABS and related.	*/
 MY66000_OP5_F,	  /* Single-precision single operand, expon and fract	*/
 MY66000_POP,	  /* POP and FF1.  */
 MY66000_VEC32,	  /* Vec with 32-bit immediate.	 */
 MY66000_CVTU,	  /* CVT and RND, unsigned immediates.	 */
 MY66000_CVTS,	  /* CVT and RND, signed immediates.  */
 MY66000_LOOPU,	  /* Loop instruction, unsigned.  */
 MY66000_LOOPS,	  /* Loop instruction, signed.	 */
 MY66000_SVC,	  /* SVC and SVR instructions.	 */
 MY66000_TRANS,	  /* Transcendental instructions.  */
 MY66000_FF1,	  /* FF1 etc.	*/
 MY66000_NOP,	  /* NOP instrucition - a special OR.	*/
 MY66000_LDM,	  /* Load and store multiple.	*/
 MY66000_XOP0,	  /* XOP0 group of shifts.  */
 MY66000_MS_55,	  /* MS for minor opcode 55.  */
 MY66000_MS_56,	  /* MS for minor opcode 56.  */
 MY66000_MS_60,	  /* MS for minor opcode 60.  */
 MY66000_BR_FAR4, /* Far branches, fourxo bytes offset.  */
 MY66000_BR_FAR8, /* Far branches, eight bytes offset.  */
 MY66000_SVR,     /* SVR instruction.  */
} my66000_encoding;

/* This is the main data structure for instructions. The table
   contains the opcode pattern, a constant giving the operand
   encoding, a subtable of instructions and the mask and shift of the
   instructions of the subtable, so they can be looked up.  */

typedef struct my66000_opc_info_t
{
  const char *name;			/* Name.  */
  uint32_t patt_opc;			/* Opcode bit pattern.	*/
  enum my66000_encoding enc;		/* Encoding.  */
  struct my66000_opc_info_t const *sub; /* Subtable, if applicable.  */
  uint32_t patt_mask;			/* Mask for the subtable or format opcode.  */
  uint32_t shift;			/* Shift for the subtable.  */
} my66000_opc_info_t;

extern const my66000_opc_info_t my66000_opc_info[];

/* Lists for gathering all the names for the various hash tables.  */

extern const char *my66000_rname[32];
extern const char *my66000_rbase[32];
extern const char *my66000_rind[32];
extern const char my66000_numtab[32];
extern const char *my66000_hr_fcn[32];

#define MY66000_VEC_BITS 21

extern const char *my66000_vec_reg[MY66000_VEC_BITS];

#define MY66000_LOOP_CND 8

extern const char *my66000_loop_u[MY66000_LOOP_CND];
extern const char *my66000_loop_s[MY66000_LOOP_CND];


typedef struct
{
  char *name;
  int num;
} my66000_reg_alias_t;

extern my66000_reg_alias_t my66000_reg_alias[];

/* Names of all the different operands the architecture has.  There
   are quite a few...  */

typedef enum my66000_operands
{
  MY66000_OPS_DST,	 /* Destination register.  */
  MY66000_OPS_SRC1,	 /* SRC1 register.  */
  MY66000_OPS_SRC2,	 /* SRC2 register. */
  MY66000_OPS_SRC3,	 /* SRC3 regsiter.  */
  MY66000_OPS_IMM16,	 /* 16-bit immediate.  */
  MY66000_OPS_I1,	 /* Five-bit immediates, value in SRC1.	 */
  MY66000_OPS_I2,	 /* Five-bit immediate, value in SRC2.	*/
  MY66000_OPS_BB1A,	 /* Bit number, 0-31.  */
  MY66000_OPS_BB1B,	 /* Bit number, 32-63. */
  MY66000_OPS_B16,	 /* 16-bit branch target.  */
  MY66000_OPS_B26,	 /* 26-bit branch target.  */
  MY66000_OPS_RINDEX,	 /* Index register.  */
  MY66000_OPS_RBASE,	 /* Base register.  */
  MY66000_OPS_I32_1,	 /* 32-bit immediate SRC1, position 1.	*/
  MY66000_OPS_I32_PCREL, /* 32-bit pc-relative immediate, position 1.  */
  MY66000_OPS_I32_HEX,	 /* 32-bit floating point (aka hex) immediate.	*/
  MY66000_OPS_I64_1,	 /* 64-bit immediate, position 1.  */
  MY66000_OPS_I64_PCREL, /* 64-bit pc-relative immediate, position 1,  */
  MY66000_OPS_I64_HEX,	 /* 64-bit floating point (aka hex) immediate, position 1.  */
  MY66000_OPS_I32_ST,	 /* 32-bit immediate, position 2.  */
  MY66000_OPS_I64_ST,	 /* 64-bit immediate, position 2.  */
  MY66000_OPS_WIDTH,	 /* 6-bit width.  */
  MY66000_OPS_OFFSET,	 /* 6-bit offset.  */
  MY66000_OPS_W_BITR,	 /* 6 bit, power of two only.  */
  MY66000_OPS_IMM13,	 /* 13-bit immediate for enter.	 */
  MY66000_OPS_FL_ENTER,	 /* Flag value for enter.  */
  MY66000_OPS_FL_EXIT,	 /* Flag value for exit.  */
  MY66000_OPS_UNUSED,	 /* Unused.  */
  MY66000_OPS_INVALID,	 /* Invalid, used for non-letters.  */
  MY66000_OPS_CARRY,	 /* Instruction modification list.  */
  MY66000_OPS_TF,	 /* True/false list for predicates.  */
  MY66000_OPS_HRFCN,	 /* HR function.  */
  MY66000_OPS_INS,	 /* A 32-bit INS specifier.  */
  MY66000_OPS_VEC,	 /* A vector bitmap.  */
  MY66000_OPS_IMM16JT,	 /* A 16-bit displacement.  */
  MY66000_OPS_SI5,	 /* 5-bit immediate, DST.  */
  MY66000_OPS_MSCALE,	 /* Scale for load and store.  */
  MY66000_OPS_VEC32,	 /* A 32-bit immediate for big VEC.  */
  MY66000_OPS_LOOP_U,	 /* LOOP condition, unsigned.  */
  MY66000_OPS_LOOP_S,	 /* LOOP condition, signed.  */
  MY66000_OPS_I3,	 /* 5-bit immediate SRC3.  */
  MY66000_OPS_I8_MS,	 /* 8-bit MS data in a 32-bit container.  */
  MY66000_OPS_I5_MS,	 /* 5-bit MS data as immediate.  */
  MY66000_OPS_I32_2,     /* 32-bit immediate at position two.  */
  MY66000_OPS_I16_LO,    /* Low 16 bit of 32-bit immediate.  */
  MY66000_OPS_I16_HI,    /* High 16-bit of 32-bit immediate. */
  MY66000_OPS_SVC16,     /* SVC immediate 16.  */
  MY66000_OPS_END
} my66000_operands;

/* Specify the operand properties for the format strings and for
   decoding/encoding.  */

typedef struct my66000_operand_info_t
{
  my66000_operands oper;
  uint32_t mask;
  uint32_t shift;
  uint32_t size;
  uint32_t seq;
  const char * desc;
  char letter;
} my66000_operand_info_t;

extern const my66000_operand_info_t my66000_operand_table[];

typedef struct my66000_fmt_spec_t
{
  char *fmt;		/* Format.  */
  uint32_t patt;	/* Bit pattern that has to match...  */
  uint32_t mask;	/* ...under this mask.  */
} my66000_fmt_spec_t;

typedef struct my66000_opcode_fmt_t
{
  const my66000_fmt_spec_t *spec;
  my66000_encoding enc;
} my66000_opcode_fmt_t;

extern const my66000_opcode_fmt_t my66000_opcode_fmt[];
extern const my66000_opc_info_t *my66000_opc_info_list[];
extern const my66000_opc_info_t my66000_opc_info[];
extern const my66000_opc_info_t my66000_opc_info_special[];

/* Helper functions for relax in the assembler.  */

extern uint32_t my66000_set_imm_size (uint32_t, uint32_t);
extern int my66000_imm_size (uint32_t);

extern bool my66000_is_tt (uint32_t);
extern uint32_t my66000_get_tt_size (uint32_t);
extern uint32_t my66000_set_tt_size (uint32_t,	uint32_t);

extern bool my66000_is_imm_st (uint32_t);
extern bool my66000_is_loop (uint32_t);
extern uint32_t my66000_get_imm_sz (uint32_t);

extern bool my66000_is_mem (uint32_t);
extern uint32_t my66000_set_mem_size (uint32_t, uint32_t);

extern bool my66000_is_call (uint32_t);
extern uint32_t my66000_get_call (int);
extern bool my66000_is_branch (uint32_t);
extern uint32_t my66000_get_branch (int);

void my66000_opc_sanity_check(void);

#endif
