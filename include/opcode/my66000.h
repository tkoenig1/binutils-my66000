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

#define MY66000_MAJOR_SHIFT 26
#define MY66000_MAJOR_MASK (63 << MY66000_MAJOR_SHIFT)

/* This is for looking up the opcode formats in my66000_opcode_fmt
   in my66000_opc.c . If you add an encoding here, you also have to
   add it there at the identical place.  */

typedef enum my66000_encoding
{
 MY66000_END = -1,
 MY66000_BAD = 0,  /* Empty.  */
 MY66000_ILL,      /* Reserved as non-opcodes.  */
 MY66000_OPIMM,    /* Arithmetic with a 16-bit immediate.  */
 MY66000_MVIMM,    /* Move 16-bit immediate.  */
 MY66000_MEM,      /* Load/store with a 16-bit offset.  */
 MY66000_ARITH,    /* Arithmetic operations.  */
 MY66000_BB1,      /* Branch on bit set, explicit bit.  */
 MY66000_BR,       /* Branch, call etc.  */
 MY66000_MRR,	   /* Indexed memory operation, [Ra,Rb]  */
 MY66000_FMAC,     /* FMAC instruction.  */
 MY66000_MUX,      /* MUX and CMOV.  */
 MY66000_MOV2,     /* Move from SRC2.  */
 MY66000_MOV3,     /* Move from SRC3.  This is redundant, but we suport it. */
 MY66000_MUX32,    /* MUX with a 32-bit immediate selector.  */
 MY66000_MOV32,    /* MOV with a 32-bit immediate.  */
 MY66000_MUX64,    /* MUX with a 64-bit immediate selector.  */
 MY66000_MOV64,    /* MOV with a 64-bit immediate.  */
 MY66000_SHIFT,    /* Shift formats with 6-bit immediates.  */
 MY66000_EMPTY,    /* No argument list.  */
 MY66000_ENTER,    /* Enter instruction.  */
 MY66000_BC,       /* Conditional branch on bit set, named constant */
 MY66000_CARRY,    /* Carry modifier.  */
 MY66000_PB1,      /* Predicate on bit set, explicit bit  */
 MY66000_PC,       /* Predicate on bit set, from comparison  */
 MY66000_PCND,     /* Predicate on condition.  */
 MY66000_BCND,     /* Branch on condition.  */
 MY66000_HR_RO,    /* Transfer of header registers, read-only.  */
 MY66000_HR_RW,    /* Transfer of header registers, read-write.  */
 MY66000_JMP,      /* JMP instruction, special case of HR.  */
 MY66000_CALLI,    /* CALLI instruction, special case of HR.  */
 MY66000_INS,      /* INS instruction.  */
 MY66000_VEC,      /* Vector instruction, with immediate.  */
 MY66000_TT,
 MY66000_SI5,      /* Store immediate with five-bit operands.  */
 MY66000_SI,	   /* Store immediate.  */
 MY66000_SI_STD,   /* Store immediate doubleword.  */
 MY66000_MM,       /* Load/store multiple.  */
 MY66000_FLOAT,    /* FADD and friends.  */
 MY66000_EADD,     /* EADD, all by itself.  */
 MY66000_CALX,     /* CALX and CALA  */
 MY66000_LOOP,     /* Loop instruction.  */
 MY66000_EXIT,
} my66000_encoding;

/* This is the main data structure for instructions. The table
   contains the opcode pattern, a constant giving the operand
   encoding, a subtable of instructions and the mask and shift of the
   instructions of the subtable, so they can be looked up.  */

typedef struct my66000_opc_info_t
{
  const char *name;			/* Name.  */
  uint32_t patt_opc;			/* Opcode bit pattern.  */
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
extern const char *my66000_hr_rw[16];
extern const char *my66000_hr_ro[16];

#define MY66000_VEC_BITS 21

extern const char *my66000_vec_reg[MY66000_VEC_BITS];

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
 MY66000_OPS_DST,
 MY66000_OPS_SRC1,
 MY66000_OPS_SRC2,
 MY66000_OPS_SRC3,
 MY66000_OPS_IMM16,
 MY66000_OPS_I1,  /* Five-bit immediates.  */
 MY66000_OPS_I2,
 MY66000_OPS_BB1,
 MY66000_OPS_B16, /* 16-bit branch target.  */
 MY66000_OPS_B26, /* 26-bit branch target.  */
 MY66000_OPS_RINDEX,
 MY66000_OPS_RBASE,
 MY66000_OPS_I32_1,
 MY66000_OPS_I32_PCREL,
 MY66000_OPS_I32_HEX,  /* Hex is only a hint for the disassembler.  */
 MY66000_OPS_I64_1,
 MY66000_OPS_I64_PCREL,
 MY66000_OPS_I64_HEX,
 MY66000_OPS_S32,
 MY66000_OPS_S64,
 MY66000_OPS_I32_ST,
 MY66000_OPS_I64_ST,
 MY66000_OPS_WIDTH,
 MY66000_OPS_OFFSET,
 MY66000_OPS_W_BITR,
 MY66000_OPS_IMM13,    /* 13-bit immediate for enter.  */
 MY66000_OPS_FL_ENTER, /* Flag value for enter.  */
 MY66000_OPS_FLT32,    /* 32-bit floating point.  */
 MY66000_OPS_INVALID,  /* Invalid, used for non-letters.  */
 MY66000_OPS_CARRY,    /* Instruction modification list.  */
 MY66000_OPS_TF,       /* True/false list for predicates.  */
 MY66000_OPS_PRTHEN,   /* "Then" part of predicate.  */
 MY66000_OPS_PRELSE,   /* "Else" part of predicate.  */
 MY66000_OPS_HRRO,     /* A read-only HR register.  */
 MY66000_OPS_HRRW,     /* A read-write HR register.  */
 MY66000_OPS_INS,      /* A 32-bit INS specifier.  */
 MY66000_OPS_VEC,      /* A vector bitmap.  */
 MY66000_OPS_UIMM16,   /* A 16-bit unsigned immediate.  */
 MY66000_OPS_SI5,      /* A 5-bit immediate store constant.  */
 MY66000_OPS_MSCALE,   /* Scale for load and store.  */
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
  uint32_t patt;	/* Bit pattern that has to match... */
  uint32_t mask;	/* ...under this mask.  */
  _Bool relax_next;	/* Offset to the next format taking part in
			   relaxation, if any.  */
} my66000_fmt_spec_t;

typedef struct my66000_opcode_fmt_t
{
  const my66000_fmt_spec_t *spec;
  my66000_encoding enc;
  uint32_t frag_mask;
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
extern uint32_t my66000_set_tt_size (uint32_t,  uint32_t);

extern bool my66000_is_imm_st (uint32_t);
extern uint32_t my66000_get_imm_sz (uint32_t);

extern bool my66000_is_mem (uint32_t);
extern uint32_t my66000_set_mem_size (uint32_t, uint32_t);

extern bool my66000_is_call (uint32_t);
extern uint32_t my66000_get_call (int);

#endif
