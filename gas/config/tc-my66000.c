/* tc-my66000.h -- header file for tc-my66000.c.
   Copyright (C) 2018 Free Software Foundation, Inc.
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
build_hashess (const my66000_opc_info_t * table)
{
  printf ("Calling hashnames %p\n",table);
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

void
md_begin (void)
{
  s_opc_map_1 = str_htab_create ();
  s_opc_map_2 = str_htab_create ();
  build_hashess (my66000_opc_info);
  bfd_set_arch_mach (stdoutput, TARGET_ARCH, 0);
}

/* This routine is called for each instruction to be assembled.  */

void
md_assemble (char *str)
{
  my66000_opc_info_t *opcode;
  char buffer[MAX_OP_STR_LEN + 1];
  int i;
  char *output;

  /* Drop leading whitespace.  */
  while (ISSPACE (*str))
    str++;

  /* Copy the instruction into the buffer, searching for the end.  */
  for (i=0; i<MAX_OP_STR_LEN; i++)
    {
      char c = str[i];
      if (!c || is_end_of_line[c & 0xff] || ISSPACE(c))
	break;
      buffer[i] = c;
    }
  buffer[i] = '\0';

  printf ("buffer = %s\n", buffer);
  if (i == 0)
    as_bad (_("Can't find opcode "));

  opcode = (my66000_opc_info_t *) str_hash_find (s_opc_map_1, buffer);
  if (opcode == NULL)
    {
      as_bad (_("unknown opcode %s"), str);
      return;
    }
  output = frag_more (4);
  memset (output, 0, 4);
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
