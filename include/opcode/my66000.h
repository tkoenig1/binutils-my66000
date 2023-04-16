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

typedef enum my66000_encoding
{
 MY66000_END = -1,
 MY66000_BAD = 0,  /* Empty.  */
 MY66000_ILL,      /* Reserved as non-opcodes.  */
 MY66000_OPIMM,    /* Arithmetic with a 16-bit immediate.  */
 MY66000_MVIMM,    /* Move 16-bit immediate.  */
 MY66000_MEM,      /* Load/store with a 16-bit offset.  */
 MY66000_ARITH,    /* Arithmetic operations.  */
 MY66000_BB1A,     /* First half of the Branch on Bit operations.  */
 MY66000_BB1B,     /* Second half of the abore.  */
 MY66000_BR,       /* Branch, call etc.  */
 MY66000_MRR,	   /* Indexed memory operation, [Ra,Rb]  */
 MY66000_FMAC,     /* FMAC instruction.  */
 MY66000_PB1A,
 MY66000_PB1B,
 MY66000_PCND,
 MY66000_BCND,
 MY66000_JT,
 MY66000_SHIFT,
 MY66000_EXIT,
 MY66000_MM,     /* Load/store multiple.  */
 MY66000_SI,	 /* Store immediate.  */
 MY66000_SI5,	 /* Store 5-bit immediate.  */
 MY66000_CARRY,
 MY66000_VEC,
} my66000_encoding;

/* This is the main data structure for instructions. The table
   contains the fragment opcode, a constant giving the operand
   encoding, a subtable of instructions and the mask and shift of the
   instructions of the subtable, so they can be looked up.  */

typedef struct my66000_opc_info_t
{
  const char *name;
  uint32_t frag_opc;			/* Opcode fragment  */
  enum my66000_encoding enc;		/* Encoding.  */
  struct my66000_opc_info_t const *sub; /* Subtable, if applicable.  */
  uint32_t frag_mask;			/* Mask for the subtable or format opcode.  */
  uint32_t shift;			/* Shift for the subtable.  */
} my66000_opc_info_t;

extern const my66000_opc_info_t my66000_opc_info[];

/* List for gathering all the names for the hash table.  */

extern const char *my66000_rname[32];
extern const char *my66000_rbase[32];
extern const char *my66000_rind[32];
extern const char my66000_numtab[32];

typedef struct
{
  char *name;
  int num;
} my66000_reg_alias_t;

extern my66000_reg_alias_t my66000_reg_alias[];

/* Names of all the different operands the architecture has.  */
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
 MY66000_OPS_I64_1,
 MY66000_OPS_I64_PCREL,
 MY66000_OPS_S32,
 MY66000_OPS_S64,
 MY66000_OPS_I32_ST,
 MY66000_OPS_I64_ST,
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

/* Helper functions for relax in the assembler.  */

extern uint32_t my66000_set_imm_size (uint32_t, uint32_t);
extern int my66000_imm_size (uint32_t);

#endif
