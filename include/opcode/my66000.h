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
#define MY66000_MAJOR(c) ((c) << MY66000_MAJOR_SHIFT)
#define MY66000_MAJOR_MASK (63 << MY66000_MAJOR_SHIFT)

typedef enum my66000_encoding
{
 MY66000_END = -1,
 MY66000_BAD = 0,  /* Empty.  */
 MY66000_ILL,      /* Reserved as non-opcodes.  */
 MY66000_OPIMM,
 MY66000_MEM,
 MY66000_OP2,
 MY66000_PB1,
 MY66000_BB1A,
 MY66000_BB1B,
 MY66000_PCND,
 MY66000_PB1A,
 MY66000_PB1B,
 MY66000_BCND,
 MY66000_JT,
 MY66000_BR,
 MY66000_CALL,
 MY66000_SHIFT,
 MY66000_EXIT,
 MY66000_MEMM,
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
extern const char *my66000_rnum[32];
extern const char my66000_numtab[32];

/* Names of all the different operands the architecture has.  */
typedef enum my66000_operands
{
 MY66000_OPS_DST,
 MY66000_OPS_SRC1,
 MY66000_OPS_SRC2,
 MY66000_OPS_IMMED16,
 MY66000_OPS_IMM5_S1,
 MY66000_OPS_IMM5_S2,
 MY66000_OPS_RINDEX,
 MY66000_OPS_END
} my66000_operands;

/* Specify the operand properties for the format strings and for
   decoding/encoding.  */

typedef struct my66000_operand_info_t
{
  my66000_operands oper;
  uint32_t mask;
  uint32_t shift;
} my66000_operand_info_t;

extern const my66000_operand_info_t my66000_operand_table[];

typedef struct my66000_fmt_spec_t
{
  char *fmt;
  uint32_t frag;
  uint32_t mask;
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

#endif
