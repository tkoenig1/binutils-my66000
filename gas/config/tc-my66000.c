/* tc-my66000.h -- Assembler for My 66000.
   Copyright (C) 2023 Free Software Foundation, Inc.
   Contributed by Thomas König (tkoenig@gcc.gnu.org).
   Based on the moxie and mrisc32 targets.

   This file is part of GAS.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING3. If not,
   see <http://www.gnu.org/licenses/>.  */

#include "as.h"
#include "safe-ctype.h"
#include "opcode/my66000.h"
#include <assert.h>

const char comment_chars[]		= ";";
const char line_separator_chars[]	= "";
const char line_comment_chars[]		= "#;";

/* Characters which are used to indicate an exponent in a floating
   point number.  */
const char EXP_CHARS[] = "eEdD";

/* Characters which mean that a number is a floating point constant,
   as in 0d1.0.  */
const char FLT_CHARS[] = "dDeExXpP";

// static int pending_reloc;

const pseudo_typeS md_pseudo_table[] =
{
  {0, 0, 0}
};

/* Target specific command line options.  */
enum options
{
  OPTION_RELAX = OPTION_MD_BASE,
  OPTION_NO_RELAX
};

struct option md_longopts[] =
{
  {NULL, no_argument, NULL, 0}
};

size_t md_longopts_size = sizeof (md_longopts);

const char *md_shortopts = "";

/* It appears that GAS will call this function for any expression
   that can not be recognized.  When the function is called,
   `input_line_pointer' will point to the start of the
   expression.  */

void
md_operand (expressionS *op __attribute__((unused)))
{
  /* Empty for now. */
}

const char *
md_atof (int type, char *litP, int *sizeP)
{
  return ieee_md_atof (type, litP, sizeP, FALSE);
}


int
md_parse_option (int c ATTRIBUTE_UNUSED, const char *arg ATTRIBUTE_UNUSED)
{
  return 0;
}

void
md_show_usage (FILE *stream ATTRIBUTE_UNUSED)
{
  return;
}

/* Maximum instruction length, which is enough in any case  */
#define MAX_OP_STR_LEN 15

/* Duplicate names for instructions occur at most twice.  */

static htab_t s_opc_map_1, s_opc_map_2;

static void
build_opc_hashes (const my66000_opc_info_t * table)
{
  for (int i=0; table[i].enc != MY66000_END; i++)
    {
      void ** slot;
      if (table[i].enc == MY66000_BAD)
	continue;

      if (table[i].name != NULL)
	{
	  slot = str_hash_insert (s_opc_map_1, table[i].name, (void *) &table[i], 1);
	  if (slot != NULL)
	    {
	      slot = str_hash_insert (s_opc_map_2, table[i].name,
				      (void *) &table[i], 1);
	      if (slot != NULL) {
		as_bad (_ ("Internal error: more than two equal opcodes"));
		exit(1);
	      }
	    }
	}
    }
  
}

static htab_t rname_map;
static htab_t operand_map;

void
md_begin (void)
{
  /* Build hashes for looking up the instructions.  */
  s_opc_map_1 = str_htab_create ();
  s_opc_map_2 = str_htab_create ();
  build_opc_hashes (my66000_opc_info);
  build_opc_hashes (my66000_opc_om6);
  build_opc_hashes (my66000_opc_om7);
  build_opc_hashes (my66000_opc_op1);
  build_opc_hashes (my66000_opc_op2);
  build_opc_hashes (my66000_opc_op4);
  build_opc_hashes (my66000_opc_op5);
  build_opc_hashes (my66000_opc_bb1a);
  build_opc_hashes (my66000_opc_bb1b);
  build_opc_hashes (my66000_opc_bcnd);
  build_opc_hashes (my66000_opc_jt);

  /* Build hash for the register names.  */
  rname_map = str_htab_create ();
  for (int i=0; i<32; i++)
    {
      str_hash_insert (rname_map, my66000_rname[i],
		       (void *) &my66000_numtab[i], 0);
      
      str_hash_insert (rname_map, my66000_rnum[i],
		       (void *) &my66000_numtab[i], 0);
    }
  /* Finally, the hashes for operand formats.  */
 operand_map = str_htab_create ();
 for (const my66000_operand_info_t * p = my66000_operand_table; p->name != NULL; p++)
    str_hash_insert (operand_map, p->name, (void *) p, 0);
}

#define MAX_REG_STR_LEN 3
#if 0
/* Match a register name from map and return its number, or, on
   error, return -1 and set errmsg to something useful.  */

static int
match_register (char **ptr, char **errmsg, htab_t map)
{
  int reg;
  char *s = *ptr;
  char tmp;
  char *rp;
  int i;
  char buffer[MAX_REG_STR_LEN + 1];

  /* Search for the end of the potential register name.  */
  for (i=0; !(ISALNUM(s[i])); i++)
    buffer[i] = TOLOWER(s[i]);

  buffer[i] = '\0';

  /* Look for the register from map.  */
  rp = (char *) str_hash_find (map, buf);
  if (rp == NULL)
    {
      char buf[100];
      snprintf (buf, sizeof(buffer), "%s: %s", (_("bad register name")), s);
      *errmsg = buf;
      reg = -1;
    }
  else
    {
      *ptr = s + (i-1);
      reg = *rp;
    }
  s[i] = tmp;
  return reg;
}

/* Match a character required by the syntax.  Issue error or advance
   ptr.  */

static void
match_character (char c, char **errmsg, char **ptr)
{
  int i;
  char *s = *ptr;

  for (i=0; !(ISALNUM(s[i] & 0xff)); i++)
    ;

  if (*s != c)
    {
      char buf[100];
      snprintf (buf, sizeof(buffer), "%s: %c", (_("unexpected character")), c);
      *errmsg = buf;
    }
  else
    *ptr = &s[i];
}

/* Match a 16-bit constant, including sign.  */

static int
match_16bit (char **ptr, char **errmsg)
{
  int i;
  char *s = *ptr;
  bool neg;
  uint64_t val;
  int16_t ret;

  if (*s == '-')
    {
      neg = true;
      s++;
    }
  else
    neg = false;

  val = 0;
  for (i=0; ISNUM(s[i]); i++)
    val = 10*val + (s[i] - '0');
}

static uint32_t
set_dst (uint32_t iword, int regno)
{
  return iword | (regno << 21);
}

#endif
static void
encode_instr (const my66000_opc_info_t *opc, char *str ATTRIBUTE_UNUSED, char **errmsg)
{
  char *p;
  uint32_t iword;

  *errmsg = NULL;
  iword = opc->p_opc << 26;
  p = frag_more (4);
  memcpy (p, &iword, 4);
}

/* This routine is called for each instruction to be assembled.  */

void
md_assemble (char *str)
{
  my66000_opc_info_t *opc;
  char buffer[MAX_OP_STR_LEN + 1];
  int i;
  char *err1, *err2;

  /* Drop leading whitespace.  */
  while (ISSPACE (*str))
    str++;

  /* Copy the instruction into the buffer, searching for the end.  */
  for (i=0; i<MAX_OP_STR_LEN; i++)
    {
      char c = str[i];
      if (!c || is_end_of_line[c & 0xff] || ISSPACE(c))
	break;
      buffer[i] = TOLOWER(c);
    }
  buffer[i] = '\0';

  if (i == 0)
    as_bad ("%s: %s",_("Illegal instruction"), buffer);

  /* We have up to two times the same assembler name, with different
     encodings, like "add r1, r22,#1234" vs.  "add r1,r22,r17".  This
     is complicated a bit because "add r1,r22,#1234" is also valid
     with a 32 bit or even a 64-bit immediate.  The strategy is to
     look up a first variant.  If this gives an error, check if there
     is a second variant.  If it doesn't exist, queue the error from
     the first attempt and try the second variant.

     Note: The ordering of what is put into each map matters in this.
  */

  opc = (my66000_opc_info_t *) str_hash_find (s_opc_map_1, buffer);
  err1 = NULL;
  encode_instr (opc, &str[i], &err1);
  if (err1 != NULL)
    {
      opc = (my66000_opc_info_t *) str_hash_find (s_opc_map_1, buffer);
      if (opc == NULL)
	{
	  as_bad ("%s", err1);
	  return;
	}
      err2 = NULL;
      encode_instr (opc, &str[i], &err2);
      if (err2 != NULL)
	{
	  as_bad ("%s", err2);
	  return;
	}
    }
}

/* Put number into target byte order.  */
void
md_number_to_chars (char *ptr, valueT use, int nbytes)
{
  number_to_chars_littleendian (ptr, use, nbytes);
}

/* Translate internal representation of relocation info to BFD target
   format.  */

arelent *
tc_gen_reloc (asection *section ATTRIBUTE_UNUSED, fixS *fixp)
{
  arelent *rel;
  bfd_reloc_code_real_type r_type;

  rel = xmalloc (sizeof (arelent));
  rel->sym_ptr_ptr = xmalloc (sizeof (asymbol *));
  *rel->sym_ptr_ptr = symbol_get_bfdsym (fixp->fx_addsy);
  rel->address = fixp->fx_frag->fr_address + fixp->fx_where;

  r_type = fixp->fx_r_type;
  rel->addend = fixp->fx_addnumber;
  rel->howto = bfd_reloc_type_lookup (stdoutput, r_type);

  if (rel->howto == NULL)
    {
      as_bad_where (fixp->fx_file, fixp->fx_line,
		    _("Cannot represent relocation type %s"),
		    bfd_get_reloc_code_name (r_type));
      /* Set howto to a garbage value so that we can keep going.  */
      rel->howto = bfd_reloc_type_lookup (stdoutput, BFD_RELOC_32);
      assert (rel->howto != NULL);
    }

  return rel;
}

/* The location from which a PC relative jump should be calculated,
   given a PC relative reloc.  */
long
md_pcrel_from_section (fixS *fixP, segT sec)
{
  if (fixP->fx_addsy != (symbolS *) NULL &&
      (!S_IS_DEFINED (fixP->fx_addsy) ||
       (S_GET_SEGMENT (fixP->fx_addsy) != sec) ||
       S_IS_EXTERNAL (fixP->fx_addsy) || S_IS_WEAK (fixP->fx_addsy)))
    {
      /* The symbol is undefined (or is defined but not in this section).
	 Let the linker figure it out.  */
      return 0;
    }

  return fixP->fx_frag->fr_address + fixP->fx_where;
}

void
md_apply_fix (fixS *fixP ATTRIBUTE_UNUSED, valueT * valP ATTRIBUTE_UNUSED, segT seg ATTRIBUTE_UNUSED)
{
  /* Empty for now.  */
}
