/* tc-my66000.h -- Assembler for My 66000.
   Copyright (C) 2023, 2024 Free Software Foundation, Inc.
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

extern void print_fixup (fixS *);

/* Characters which are used to indicate an exponent in a floating
   point number.  */
const char EXP_CHARS[] = "eEdD";

/* Characters which mean that a number is a floating point constant,
   as in 0d1.0.  */
const char FLT_CHARS[] = "dDeExXpP";


/* Target specific command line options.  */
enum options
{
  OPTION_MCMODEL = OPTION_MD_BASE,
};

const struct option md_longopts[] =
{
  {"mcmodel", required_argument, NULL, OPTION_MCMODEL},
  {NULL, no_argument, NULL, 0}
};

const size_t md_longopts_size = sizeof (md_longopts);

const char md_shortopts[1];

enum memory_model
  {
    TINY = 0,
    SMALL = 4,
    LARGE = 8,
  };

enum memory_model mcmodel = SMALL;

/* Stuff for handling the pseudo-ops.  */
static void handle_jt (int);
static int needs_jt;

const pseudo_typeS md_pseudo_table[] =
{
 { "jt8",  handle_jt, 1 },
 { "jt16", handle_jt, 2 },
 { "jt32", handle_jt, 4 },
 { "jt64", handle_jt, 8 },

 { "half", cons, 2 },
 { "word", cons, 4 },
 { "dword", cons, 8 },
 {0, 0, 0}
};

/* A type we use for the "fx_opocde" field, which is needed for
   recording the address of the actual opcode, plus the relocation
   info.  We keep this in a linked list so there is no complaint
   about memory leaks.  Not memory-efficient, but hey - this is the
   2020s.  */

typedef struct opcode_pos_t
{
  fragS *frag;
  addressT pos;
  struct opcode_pos_t *next;
} opcode_pos_t;


static struct my66000_frag current_tt;

static addressT
get_opc_addr (fragS *frag)
{
  fragS *f;

  if (frag->tc_frag_data.old_frag)
    f = frag->tc_frag_data.old_frag;
  else
    f = frag;

  return f->fr_address + (f->fr_opcode - f->fr_literal);
}

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
  switch (c)
    {
    case OPTION_MCMODEL:
      if (strcasecmp (arg, "tiny") == 0)
	mcmodel = TINY;
      else if (strcasecmp (arg, "small") == 0)
	mcmodel = SMALL;
      else if (strcasecmp (arg, "large") == 0)
	mcmodel = LARGE;
      else
	as_fatal (_("invalid -mcmodel= option: `%s'"), arg);
      break;
    default:
      return 0;
    }
  return 1;
}

void
md_show_usage (FILE *stream ATTRIBUTE_UNUSED)
{
  return;
}

/* Maximum instruction length, which is enough in any case.  Anything longer
 will just get truncated.  */
#define MAX_OP_STR_LEN 15

/* Duplicate names for instructions occur at most N_MAP times.  */

#define N_MAP 4

static htab_t s_opc_map[N_MAP];

#define COUNT_VARIANTS 0
#define DUMP_INSTRUCTIONS 0

long int num_instr;

/* This does not strictly belong in the assembler, but for
   an experimental architecture, it is nice to gather some
   statistics.  */

static const char *
instruction_type (my66000_encoding enc)
{
  switch (enc)
    {
    case MY66000_MRR:
    case MY66000_CALX:
    case MY66000_MRRL0:
    case MY66000_SI5:
    case MY66000_SI:
    case MY66000_SI_STD:
      return "mem";
    case MY66000_MM:
    case MY66000_MS_55:
    case MY66000_MS_60:
      return "mm";
    case MY66000_ARITH:
    case MY66000_ARITHS0:
    case MY66000_FLOAT:
    case MY66000_CVTS:
    case MY66000_CVTU:
    case MY66000_EADD:
      return "arith";
    case MY66000_MUX:
    case MY66000_FMAC:
    case MY66000_INS:
    case MY66000_MUX32:
    case MY66000_MUX64:
      return "mux";
    case MY66000_TRANS:
    case MY66000_FF1:
      return "sop";
    case MY66000_SHIFT:
    case MY66000_XOP0:
      return "shift";
    case MY66000_LOOPU:
    case MY66000_LOOPS:
      return "loop";
    case MY66000_ENTER:
      return "enter";
    case MY66000_EXIT:
      return "exit";
    case MY66000_PB1A:
    case MY66000_PB1B:
    case MY66000_PC:
      return "pb1";
    default:
      return "std";
    }
}

static void
count_instruction_variants (const my66000_opc_info_t *opc)
{
  my66000_encoding enc;
  const my66000_opcode_fmt_t *fmtlist;
  const my66000_fmt_spec_t *spec;

  enc = opc->enc;
  fmtlist = &my66000_opcode_fmt[enc];
  spec = fmtlist->spec;

  if (spec == NULL || spec->fmt == NULL)
    return;

  for (spec = fmtlist->spec; spec->fmt; spec++)
    num_instr ++;
}

static void
dump_instruction_variants (const my66000_opc_info_t *opc)
{
  my66000_encoding enc;
  const my66000_opcode_fmt_t *fmtlist;
  const my66000_fmt_spec_t *spec;

  enc = opc->enc;
  fmtlist = &my66000_opcode_fmt[enc];
  spec = fmtlist->spec;

  if (spec == NULL || spec->fmt == NULL)
    return;

  for (spec = fmtlist->spec; spec->fmt; spec++)
    printf ("%s\t%s\t%8.8x\t%s\n", opc->name, instruction_type (opc->enc),
	    opc->patt_opc | spec->patt,
	    spec->fmt);
}

static void
build_opc_hashes (const my66000_opc_info_t * table)
{

  for (int i = 0; table[i].enc != MY66000_END; i++)
    {
      //      void ** slot;
      int j;

      if (table[i].enc == MY66000_BAD)
	continue;

      if (table[i].name != NULL)
	{
	  /* Loop over the N_MAP available hashmaps and look for a place
	     to store the name.  */

	  if (COUNT_VARIANTS)
	    count_instruction_variants (&table[i]);

	  if (DUMP_INSTRUCTIONS)
	    dump_instruction_variants (&table[i]);

	  for (j = 0; j < N_MAP; j++)
	    {
	      void *ptr = str_hash_find (s_opc_map[j], table[i].name);
	      if (ptr == NULL)
		{
		  str_hash_insert (s_opc_map[j], table[i].name,
				   (void *) &table[i], 0);
		  break;
		}
	    }
	  if (j == N_MAP)
	    as_fatal (_ ("Internal error: more than %d equal opcodes: %s"),
		      N_MAP, table[i].name);
	}
    }
}


static htab_t rname_map, rbase_map, rind_map;
static htab_t hr_map;
static htab_t vec_map;
static htab_t loop_u_map, loop_s_map;

#define MAX_REG_STR_LEN 10

static void
dump_operands(void)
{
  const my66000_operand_info_t *tab = my66000_operand_table;
  for (int i=0; tab[i].letter < '{'; i++)
    {
      if (tab[i].oper != MY66000_OPS_INVALID)
	printf ("#\t%u\t%u\t%u\t%u\t%s\t%c\n", __builtin_popcount(tab[i].mask), tab[i].shift,
		tab[i].size, tab[i].seq, tab[i].desc, tab[i].letter);
    }
}

void
md_begin (void)
{
   /* Build hashes for looking up the instructions.  */
  const my66000_opc_info_t **lst = my66000_opc_info_list;

  if (DUMP_INSTRUCTIONS)
    dump_operands();

  for (int j=0; j<N_MAP; j++)
    s_opc_map[j] = str_htab_create ();

  for (int k=0; lst[k]; k++)
    build_opc_hashes (lst[k]);

  /* Build hash for the register names.  */
  rname_map = str_htab_create ();
  rbase_map = str_htab_create ();
  rind_map  = str_htab_create ();
  /* We treat the functions for HR as sort of register names.  */
  hr_map = str_htab_create();
  for (int i = 0; i < 32; i++)
    {
      str_hash_insert (rname_map, my66000_rname[i],
		       (void *) &my66000_numtab[i], 0);
      str_hash_insert (rbase_map, my66000_rbase[i],
		       (void *) &my66000_numtab[i], 0);
      str_hash_insert (rind_map, my66000_rind[i],
		       (void *) &my66000_numtab[i], 0);
      if (my66000_hr_fcn[i])
	str_hash_insert (hr_map, my66000_hr_fcn[i],
			 (void *) &my66000_numtab[i], 0);
    }

  for (my66000_reg_alias_t * p = my66000_reg_alias; p->name; p++)
    {
      str_hash_insert (rname_map, p->name, (void *) &p->num, 0);
      str_hash_insert (rbase_map, p->name, (void *) &p->num, 0);
    }


  /* Seting up the table for the VEC instruction, bitmap style.  */
  vec_map = str_htab_create();
  for (int i = 0; i < MY66000_VEC_BITS; i++)
    str_hash_insert (vec_map, my66000_vec_reg[i],(void *) &my66000_numtab[i], 0);

  loop_u_map = str_htab_create();
  loop_s_map = str_htab_create();
  /* LOOP condition codes.  */
  for (int i = 0; i < MY66000_LOOP_CND; i++)
    {
      str_hash_insert (loop_u_map, my66000_loop_u[i], (void *) &my66000_numtab[i], 0);
      str_hash_insert (loop_s_map, my66000_loop_s[i], (void *) &my66000_numtab[i], 0);
    }

  /* Internal test for consistency.  We use the enum to index into
     the opcode fmt table, this needs to be right.  This could be be
     #ifdefed out for later production, or maybe not.  */
  int count = 0;
  for (const my66000_opcode_fmt_t *p = my66000_opcode_fmt;
       p->enc != MY66000_END; p++)
    {
      if (p->enc != count)
	as_fatal (_("opcode_fmt unordered for entry %d"), count);

      count ++;
    }
  if (COUNT_VARIANTS)
    fprintf (stderr,"%ld different instructions found\n", num_instr);

#if 1
  /* Apply some sanity checks to make sure the internal data
     structures are in a consistent state.  */

  my66000_opc_sanity_check();
#endif
}


static char errbuf[100];

/* Match a non-whitespace character required by the syntax.  Issue
   error or advance ptr past the matched character.  */

static void
match_character (char c, char **ptr, char **errmsg)
{
  char *s;

  for (s = *ptr; is_whitespace(*s); s++)
    ;

  if (*s != c)
    {
      if (*s == '\0')
	snprintf (errbuf, sizeof(errbuf),(_("Unexpected end of line")));
      else
	snprintf (errbuf, sizeof(errbuf),
		(_("unexpected character '%c', expecting '%c'")), *s, c);

      *errmsg = errbuf;
    }
  else
    *ptr = s + 1;
}

/* Match a string delimited by single quotes and advance fmt and
   ptr.  */

static void
match_string (const char **fmt, char **ptr, char **errmsg)
{
  char *s = *ptr;
  const char *f = *fmt + 1;
  const char *fmt0;

  match_character ('\'', &s, errmsg);
  if (*errmsg)
    return;

  fmt0 = f;
  while (1)
    {
      if (*f == '\'' && *s == '\'')
	{
	  *ptr = s;
	  *fmt = f;
	  return;
	}
      if (*f != TOLOWER(*s))
	{
	  snprintf (errbuf, sizeof(errbuf),
		    _("Mismatch in string match: %s vs %s"),
		    *ptr, fmt0);
	  return;
	}
      f ++;
      s ++;
    }
}


/* Match an integer and record the resulting expression in ex.  */;

static void
match_integer_expr_ex (char **ptr, char **errmsg, offsetT minval, offsetT maxval,
		       expressionS *ex)

{
  char *save, *endp, *str;
  char saved_char;

  *errmsg = NULL;
  save = input_line_pointer;
  str = *ptr;

  /* Drop leading whitespace.  */
  while (is_whitespace (*str))
    str++;

  input_line_pointer = str;

  /* Look for a delimiter and make sure that expr sees a
     null-terminated string.  */

  endp = str;
  while (1)
    {
      if (*endp == '\0' || *endp == ',' || *endp == ']' || *endp == ':'
	  || *endp == '<' || *endp == '>'
	  || is_end_of_stmt(*endp))
	break;
      endp++;
    }

  saved_char = *endp;
  *endp = '\0';
  expression_and_evaluate (ex);
  *endp = saved_char;
  input_line_pointer = save;
  if (ex->X_op != O_constant)
    {
      strcpy (errbuf, "Not a constant");
      *errmsg = errbuf;
      return;
    }
  else if (ex->X_add_number < minval || ex->X_add_number > maxval)
    {
      strcpy (errbuf, "Constant out of range");
      *errmsg = errbuf;
      return;
    }
  *ptr = endp;
}

/* Match an integer; just a wrapper for the _ex version.  */

static uint64_t
match_integer (char **ptr, char **errmsg, offsetT minval, offsetT maxval)
{
  expressionS ex;
  match_integer_expr_ex (ptr, errmsg, minval, maxval, &ex);
  return ex.X_add_number;
}

static uint16_t
match_16bit (char **ptr, char **errmsg)
{
  uint16_t res;
  res = match_integer (ptr, errmsg, INT16_MIN, INT16_MAX);
  return res;
}

static uint16_t
match_3bit (char **ptr, char **errmsg)
{
  uint16_t res;
  res = match_integer (ptr, errmsg, 0, 7);
  return res;
}

static uint16_t
match_2bit (char **ptr, char **errmsg)
{
  uint16_t res;
  res = match_integer (ptr, errmsg, 0, 3);
  return res;
}

/* Match a five-bit unsigned constant.  */

static uint8_t
match_5bitu (char **ptr, char **errmsg)
{
  return match_integer (ptr, errmsg, 0, 31) & 31;
}

/* Match a five-bit signed constant.  */

static uint8_t
match_5bits (char **ptr, char **errmsg)
{
  return match_integer (ptr, errmsg, -16, 15) & 31;
}

/* Match a six-bit positive constant.  */

static uint8_t
match_6bit (char **ptr, char **errmsg)
{
  uint8_t res;
  res = match_integer (ptr, errmsg, 0, 63);
  return res;
}

/* Match a six-bit positive constant that is also a power of two.  */

static uint8_t
match_6bit_p2 (char **ptr, char **errmsg)
{
  uint8_t ret;
  ret = match_integer (ptr, errmsg, 0, 63);
  if (*errmsg == NULL)
    {
      switch (ret)
	{
	case 1: case 2: case 4: case 8: case 16:
	  return ret;
	default:
	  sprintf (errbuf, "Illegal constant %u", ret);
	  *errmsg = errbuf;
	  break;
	}
    }
  return ret;
}

static uint16_t
match_16bit_u (char **ptr, char **errmsg)
{
  return match_integer (ptr, errmsg, 0, UINT16_MAX);
}


static uint16_t
match_imm13 (char **ptr, char **errmsg)
{
  uint16_t res;
  res = match_integer (ptr, errmsg, 0, UINT16_MAX);
  if (*errmsg)
    return 0;
  if ((res & 0x7) != 0)
    {
      sprintf (errbuf, "IMM13 not properly aligned");
      *errmsg = errbuf;
      return 0;
    }
  return res;
}
/* Match a modifier list item and set the pointer to the
   beginning of the next item or the trailing }.  */

static uint8_t
match_carry_item (char **ptr, char **errmsg)
{
  uint16_t ret;
  bool valid;
  char *p = *ptr, *pe;

  valid = true;
  ret = 0;
  if (p[0] == '-')
    {
      ret = 0;
      pe = &p[1];
    }
  else if (TOUPPER(p[0]) == 'I' && TOUPPER(p[1]) != 'O')
    {
      ret = 1;
      pe = &p[1];
    }
  else if (TOUPPER(p[0]) == 'O')
    {
      ret = 2;
      pe = &p[1];
    }
  else if (TOUPPER(p[0]) == 'I' && TOUPPER(p[1]) == 'O')
    {
      ret = 3;
      pe = &p[2];
    }
  else
    {
      pe = p;
      valid = false;
    }

  if (valid) {
    if (*pe == ',')
      *ptr = pe + 1;
    else if (*pe == '}')
      *ptr = pe;
    else
      valid = false;
  }

  if (valid)
    return ret;

  snprintf (errbuf, sizeof(errbuf),
	    (_("unexpected character '%c' in modifier list")), *pe);
  *errmsg = errbuf;
  return 0;
}

/* The number of carries still active.  */

static int num_carry;

/* Match a carry list.  Issue an error if we are still in another
   carry list, and record the number of carries found, without
   trailing '-'. '{' and '}' are already taken care of in the format
   string.  */

static uint16_t
match_carry_list (char **ptr, char **errmsg)
{
  uint16_t ret = 0;

  for (int shft=0; shft<16; shft += 2)
    {
      uint16_t item = match_carry_item (ptr, errmsg);
      if (*errmsg)
	{
	  num_carry = 0;
	  return 0;
	}

      if (item != 0)
	{
	  ret |= item << shft;
	  num_carry = (shft >> 1) + 2;
	}

      if (**ptr == '}')
	return ret;
    }
  snprintf (errbuf, sizeof(errbuf), _("too many items in modifier argument list"));
  return 0;
}

static uint8_t
match_tf_list (char **ptr, char **errmsg, char ch)
{
  int num = 0;
  char *str;

  str = *ptr;

  /* Drop leading whitespace.  */
  while (is_whitespace (*str))
    str++;

  while (TOUPPER (*str) == ch)
    {
      num++;
      str++;
    }
  if (num > 8)
    {
      sprintf (errbuf, "Too many %c values", ch);
      *errmsg = errbuf;
      return 0;
    }
  *ptr = str;
  return num;
}

/* Match a register name from map and return its number, or, on
   error, return 0 and set errmsg to something useful.  */

static uint32_t
match_register (char **ptr, char **errmsg, htab_t map)
{
  uint32_t reg;
  char *s = *ptr;
  char *rp;
  int i;
  char buf[MAX_REG_STR_LEN + 1];

  /* Drop leading whitespace.  */
  while (is_whitespace (*s))
    s++;

  /* Search for the end of the potential register name.  */
  for (i = 0; i < MAX_REG_STR_LEN && ISALNUM(s[i]); i++)
    buf[i] = TOLOWER(s[i]);


  buf[i] = '\0';
  //  fprintf (stderr,"match_register : '%s'", buf);

  /* Look for the register from map.  */
  rp = (char *) str_hash_find (map, buf);
  if (rp == NULL)
    {
      //      fprintf (stderr,"failed\n");
      snprintf (errbuf, sizeof(errbuf), "%s: %s", (_("Unexpected string")), buf);
      *errmsg = errbuf;
      reg = 0;  /* Error will be reported via errmsg anyway.  */
    }
  else
    {
      *ptr = s + i;
      reg = *rp;
      //      fprintf (stderr,"%d\n", reg);
    }
  return reg;
}

/* Match a list of registers vor VEC.  We take care of the two versions by
   using the appropriate register map.  */

static uint32_t
match_vec (char **ptr, char **errmsg, htab_t map)
{
  uint32_t rnum;
  uint32_t ret = 0;
  char *p = *ptr;
  while (1)
    {
      if (*p == '}')
	break;
      if (*p == '\0')
	{
	  snprintf (errbuf, sizeof(errbuf),
		    _("unexpected end of VEC string"));
	  return 0;
	}
      rnum = match_register (&p, errmsg, map);
      if (*errmsg)
	return 0;
      ret |= 1u << rnum;
      if (*p == ',')
	p++;
    }
  *ptr = p;
  return ret;
}

/* Match number with n bits or a label.  If it is a signed number,
   return the result truncated to an unsigned number (if needed).  If
   "vanilla" is set, the number is unsigned, and no labels are
   accepted.  */

static uint64_t
match_num_or_label (char **ptr, char **errmsg, expressionS *ex,
		    uint32_t bit, bool vanilla)
{
  uint64_t ret;
  char *endp, *save, *str;
  char saved_char;

  *errmsg = NULL;
  save = input_line_pointer;
  str = *ptr;

  /* Drop leading whitespace.  */
  while (is_whitespace (*str))
    str++;

  input_line_pointer = str;

  endp = str;
  while (1)
    {
      if (*endp == '\0' || *endp == ',' || *endp == ']' || *endp == ':'
	  || *endp == '<' || *endp == '>'
	  || is_end_of_stmt (*endp))
	break;
      endp++;
    }

  saved_char = *endp;
  *endp = '\0';
  expression (ex);
  *endp = saved_char;
  input_line_pointer = save;
  switch (ex->X_op)
    {
    case O_constant:
      if (bit < 64)
	{
	  int64_t minval, maxval;
	  if (vanilla)
	    {
	      minval = 0;
	      maxval = ((int64_t) 1 << bit) - 1;
	    }
	  else
	    {
	      minval = -((int64_t) 1 << (bit - 1));
	      maxval =  ((int64_t) 1 << (bit - 1)) - 1;
	    }
	  if (ex->X_add_number < minval || ex->X_add_number > maxval)
	    {
	      strcpy (errbuf, "Constant out of range");
	      *errmsg = errbuf;
	      return 0;
	    }
	}
      ret = (uint64_t) ex->X_add_number;
      break;
    case O_symbol:
      if (vanilla)
	goto syntax;

      ret = 0;
      break;
    default:
      goto syntax;
      return 0;
    }
  *ptr = endp;
  if (bit < 64)
    ret &= (((uint64_t) 1) << bit) - 1;

  return ret;

 syntax:
  strcpy (errbuf, "Syntax error constant");
      *errmsg = errbuf;
      return 0;
}

static uint16_t
match_16bit_or_label (char **ptr, char **errmsg, expressionS *ex)
{
  return match_num_or_label (ptr, errmsg, ex, 16, false);
}

static uint32_t
match_26bit_or_label (char **ptr, char **errmsg, expressionS *ex)
{
  return match_num_or_label (ptr, errmsg, ex, 26, false);
}

static uint32_t
match_32_bit_or_label (char **ptr, char **errmsg, expressionS *ex)
{
  return match_num_or_label (ptr, errmsg, ex, 32, false);
}

static uint64_t
match_64_bit_or_label (char **ptr, char **errmsg, expressionS *ex)
{
  return match_num_or_label (ptr, errmsg, ex, 64, false);
}

static uint64_t
match_64_bit_vanilla (char **ptr, char **errmsg, expressionS *ex)
{
  return match_num_or_label (ptr, errmsg, ex, 64, true);
}

static uint8_t
match_scale (char **ptr, char **errmsg)
{
  return match_integer (ptr, errmsg, 0, 3);
}

static uint8_t
match_ins_width (char **ptr, char **errmsg)
{
  return match_integer (ptr, errmsg, 1, 64);
}

/* Do the matching for a 32- or 64-bit constant "by hand", this is
   needed to differentiate between different size float formats.
   C'est la vie... */

static uint64_t
match_hex (char **ptr, char **errmsg, int bytes)
{
  char *str;
  *errmsg = NULL;
  str = *ptr;
  uint64_t val;
  int i;

  /* Drop leading whitespace.  */
  while (is_whitespace (*str))
    str++;

  if (str[0] != '0' && (str[1] != 'x' || str[1] != 'X'))
    {
      strcpy (errbuf, "Need starting 0x for hex constant");
      *errmsg = errbuf;
      return 0;
    }
  str += 2;
  val = 0;
  /* Roll our own strtoul, with checking the number of digits.  */
  for (i = 0; i<2*bytes; i++)
    {
      if (ISDIGIT(str[i]))
	{
	  val = 16*val + str[i] - '0';
	}
      else
	{
	  char ch = TOLOWER(str[i]);
	  if (ch >= 'a' && ch <= 'f')
	    {
	      val = 16*val + ch - 'a';
	    }
	  else
	    {
	      snprintf (errbuf, sizeof(errbuf), "Hex digit required, seen %c",
		       str[i]);
	      *errmsg = errbuf;
	      return 0;
	    }
	}
    }
  if (ISXDIGIT(str[i]))
    {
      strcpy (errbuf, "Too many hex digits");
      return 0;
    }
  *ptr = str + i;
  return val;
}


/* Match an INS pattern, #o,#w.  Dept. of dirty tricks: This breaks
   only matching single operands, but anything else would be too
   complicated for now.  */

static uint32_t
match_ins (char **ptr, char **errmsg, expressionS *ex)
{
  uint32_t width, offset;
  offset = match_6bit (ptr, errmsg);
  if (*errmsg)
    return 0;

  match_character (',', ptr, errmsg);
  match_character ('#', ptr, errmsg);
  if (*errmsg)
    return 0;

  width = match_ins_width (ptr, errmsg);

  if (*errmsg)
    return 0;

  if (offset + width > 64)
    {
      sprintf (errbuf, "offset + width > 64");
      *errmsg = errbuf;
      return 0;
    }

  if (width == 64)
    width = 0;

  ex->X_op = O_constant;
  ex->X_add_number = (width << 6) + offset;
  return (width << 6) + offset;
}

typedef enum {
  RELAX_NONE = 0,
  RELAX_TT_FIRST,  /* First round of JTT.  */
  RELAX_TT_8_S2,
  RELAX_TT_16_S2,
  RELAX_TT_32_S2,
  RELAX_TT_64_S2,
  RELAX_CALL_26,
  RELAX_CALL_32,
  RELAX_CALL_64,
  RELAX_BR_26,
  RELAX_BR_32,
  RELAX_BR_64,
  RELAX_BCND_16,
  RELAX_BCND_26,
  RELAX_BCND_32,
  RELAX_BCND_64,
  RELAX_IMM_32,
  RELAX_IMM_64,
  RELAX_IMM_32_PCREL,
  RELAX_IMM_64_PCREL,
  RELAX_TT_LAST,
  RELAX_LAST,
} relax_type;

/* Der offset muss hier auch noch rein!  */

struct relax_tabS
{
  relax_type relax_num;	/* Our very own relaxation number.  */
  int reloc_num;	/* The corresponding BFD relocation number.  */
  int bits;		/* Signed bits of range.  */
  unsigned size_var;		/* Length of variable frag.  */
  relax_type smallest;	/* Looking backwards, how small can we get? */
  _Bool pcrel;
  my66000_set_opc set_opc;
  enum memory_model mcmodel; /* Valid for this memory model or higher.  */
};

#define RANGE_MIN(bits) (-((offsetT) 1 << (bits - 1)))
#define RANGE_MAX(bits) (((offsetT) 1 << (bits - 1)) - 1)

/* Table for relocation modes. smallest = 0 means that there is no
   bigger one.  */

const struct relax_tabS relax_tab[RELAX_LAST+1] =
{
  {RELAX_NONE,	       BFD_RELOC_NONE,	       0, 0, RELAX_NONE,       0, NULL, TINY},
  {RELAX_TT_FIRST,     BFD_RELOC_NONE,	       0, 1, RELAX_TT_FIRST,   1, NULL, TINY},
  {RELAX_TT_8_S2,      BFD_RELOC_8_PCREL_S2,  10, 1, RELAX_TT_8_S2, 1, my66000_set_tt, TINY},
  {RELAX_TT_16_S2,     BFD_RELOC_16_PCREL_S2, 18, 2, RELAX_TT_8_S2, 1, my66000_set_tt, TINY},
  {RELAX_TT_32_S2,     BFD_RELOC_32_PCREL_S2, 34, 4, RELAX_TT_8_S2, 1, my66000_set_tt, TINY},
  {RELAX_TT_64_S2,     BFD_RELOC_64_PCREL_S2, 64, 8, 0,		    1, my66000_set_tt, LARGE},
  {RELAX_CALL_26,      BFD_RELOC_26_PCREL_S2, 28, 4, RELAX_CALL_26, 1, my66000_set_call, TINY},
  {RELAX_CALL_32,      BFD_RELOC_32_PCREL,    32, 8, RELAX_CALL_26, 1, my66000_set_call, SMALL},
  {RELAX_CALL_64,      BFD_RELOC_64_PCREL,    64, 12, 0,	    1, my66000_set_call, LARGE},
  {RELAX_BR_26,	       BFD_RELOC_26_PCREL_S2, 28, 4, RELAX_BR_26,   1, my66000_set_branch, TINY},
  {RELAX_BR_32,	       BFD_RELOC_32_PCREL_S2, 34, 8, RELAX_BR_26,   1, my66000_set_branch, SMALL},
  {RELAX_BR_64,	       BFD_RELOC_64_PCREL_S2, 64, 12, RELAX_BR_26,  1, my66000_set_branch, LARGE},
  {RELAX_BCND_16,      BFD_RELOC_16_PCREL_S2, 18, 4, RELAX_BCND_16, 1, my66000_set_bcnd, TINY},
  {RELAX_BCND_26,      BFD_RELOC_26_PCREL_S2, 28, 8, RELAX_BCND_16, 1, my66000_set_bcnd, TINY},
  {RELAX_BCND_32,      BFD_RELOC_32_PCREL_S2, 34,12, RELAX_BCND_16, 1, my66000_set_bcnd, SMALL},
  {RELAX_BCND_64,      BFD_RELOC_64_PCREL_S2, 64,16, 0,		    1, my66000_set_bcnd, LARGE},
  {RELAX_IMM_32,       BFD_RELOC_32,	      32, 8, RELAX_IMM_32,  0, my66000_set_imm, SMALL},
  {RELAX_IMM_64,       BFD_RELOC_64,	       0, 12, 0,	    0, my66000_set_imm, LARGE},
  {RELAX_IMM_32_PCREL, BFD_RELOC_32_PCREL,    32, 8, RELAX_IMM_32_PCREL, 1, my66000_set_imm, SMALL},
  {RELAX_IMM_64_PCREL, BFD_RELOC_64_PCREL,     0, 12, 0,       		 1, my66000_set_imm, LARGE},
  {RELAX_LAST,	       BFD_RELOC_NONE,	       0, 1, 0, 		 0, NULL, TINY},
};

/* Count commas in a string, for returning early if the number of
   arguments does not match.  Ignore anything between curly braces,
   and if a letter occurs in *except, pretend it's an extra comma.
   Yes, this is a hack.
*/

static int
n_commas (const char *p, const char *except)
{
  int ret = 0;
  int n_curly = 0;
  while (*p)
    {
      if (*p == '{')
	n_curly ++;
      else if (*p == '}')
	n_curly --;

      if (n_curly == 0)
	{
	  ret += *p == ',';
	  if (except)
	    ret += strchr (except, *p) != NULL;
	}
      p++;
    }
  return ret;
}

void
my66000_tc_frag_init (fragS *fragP ATTRIBUTE_UNUSED, size_t max_chars ATTRIBUTE_UNUSED)
{
  //  fprintf (stderr,"tc_frag_init: %p\n", fragP);
}

/* Attempt a match of the arglist pointed to by str against fmt.  If
   errmsg is set, the match was a failure; otherwise issue issue the
   instruction.

   One issue is the handling of fixups, because we the algorithm here
   is match-and-reject.  */

static void
match_arglist (uint32_t iword, const my66000_fmt_spec_t *spec, char *str,
	       char **errmsg)
{
  const char *fp = spec->fmt;
  char *sp = str;
  const my66000_operand_info_t *info;
  char *p = NULL; // , *p_op;
  int length = 4;
  expressionS imm, imm_st;
  int imm_size = 0, imm_st_size = 0;
  uint64_t val_tmp;
  _Bool imm_st_pcrel = false;
  relax_type relax_imm = RELAX_NONE, relax_imm_st = RELAX_NONE;

  // fprintf (stderr,"match_arglist : iword = %8.8x '%s' '%s'\n", iword, str, spec->fmt);
  /* Early check - if the number of commas do not agree, this cannot match.  */
  if (n_commas (str,NULL) != n_commas (spec->fmt,"g"))
    {
      snprintf (errbuf, sizeof(errbuf),(_("Wrong number of operands")));
      *errmsg = errbuf;
      return;
    }

  memset(&imm, 0, sizeof(imm));
  memset(&imm_st, 0, sizeof(imm_st));
  relax_imm = RELAX_NONE;
  for (; *fp; fp++)
    {
      // fprintf (stderr, "fp = %s sp = %s\n", fp, sp);
      uint32_t bits = 0;
      if (*fp == '\'')
	{
	  match_string (&fp, &sp, errmsg);
	  if (*errmsg)
	    {
	      //	      fprintf (stderr,"match_string failed\n");
	      return;
	    }

	  continue;
	}
      if (!ISALPHA(*fp))
	{
	  match_character (*fp, &sp, errmsg);
	  if (*errmsg)
	    {
	      //	      fprintf (stderr,"match_character failed: %c\n", *fp);
	      return;
	    }

	  continue;
	}

      info = &my66000_operand_table[*fp - 'A'];
      //      fprintf (stderr,"'%c' %s\n",info->letter, info->desc);
      switch (info->oper)
	{
	case MY66000_OPS_DST:
	case MY66000_OPS_SRC1:
	case MY66000_OPS_SRC2:
	case MY66000_OPS_SRC3:
	  bits = match_register (&sp, errmsg, rname_map);
	  break;
	case MY66000_OPS_RINDEX:
	  bits = match_register (&sp, errmsg, rind_map);
	  break;
	case MY66000_OPS_IP_BASE:
	  bits = match_register (&sp, errmsg, rbase_map);
	  if (bits != 0)
	    {
	      snprintf (errbuf, sizeof(errbuf),_("IP should be zero"));
	      *errmsg = errbuf;
	    }
	  break;
	case MY66000_OPS_RBASE:
	  bits = match_register (&sp, errmsg, rbase_map);
	  break;
	case MY66000_OPS_HRFCN:
	  bits = match_register (&sp, errmsg, hr_map);
	  break;
	case MY66000_OPS_IMM16:
	  bits = match_16bit (&sp, errmsg);
	  break;
	case MY66000_OPS_IMM13:
	  bits = match_imm13 (&sp, errmsg) / 8;
	  break;

	case MY66000_OPS_FL_ENTER:
	  bits = match_2bit (&sp, errmsg);
	  break;

	case MY66000_OPS_FL_EXIT:
	  bits = match_3bit (&sp, errmsg);
	  break;

	case MY66000_OPS_I1:
	case MY66000_OPS_I2:
	case MY66000_OPS_I3:
	case MY66000_OPS_BB1A:
	  bits = match_5bitu (&sp, errmsg);
	  break;

	case MY66000_OPS_SI5:
	  bits = match_5bits (&sp, errmsg);
	  break;

	case MY66000_OPS_BB1B:
	  bits = match_integer (&sp, errmsg, 32, 63) - 32;
	  break;

	case MY66000_OPS_WIDTH:
	  bits = match_integer (&sp, errmsg, 0, 64) & 63;
	  break;

	case MY66000_OPS_OFFSET:
	  bits = match_6bit (&sp, errmsg);
	  break;

	case MY66000_OPS_W_BITR:
	  bits = match_6bit_p2 (&sp, errmsg);
	  break;

	case MY66000_OPS_IMM16JT:
	  bits = match_16bit_u (&sp, errmsg);
	  needs_jt = bits + 1;
	  break;

	case MY66000_OPS_MSCALE:
	  bits = match_scale (&sp, errmsg);
	  break;

	case MY66000_OPS_CARRY:
	  bits = match_carry_list (&sp, errmsg);
	  break;

	case MY66000_OPS_P_THEN:
	  bits = match_tf_list (&sp, errmsg, 'T');
	  break;

	case MY66000_OPS_P_ELSE:
	  bits = match_tf_list (&sp, errmsg, 'F');
	  break;

	case MY66000_OPS_LOOP_U:
	  bits = match_register (&sp, errmsg, loop_u_map);
	  break;

	case MY66000_OPS_LOOP_S:
	  bits = match_register (&sp, errmsg, loop_s_map);
	  break;

	case MY66000_OPS_VEC:
	  bits = match_vec (&sp, errmsg, vec_map);
	  break;

	  /* Dept. of dirty tricks: We use the fact that branches
	     within the instruction word are always the last
	     argument.  If we made it this far, we can already
	     allocate the memory for the instruction.  If that
	     turns out to be wrong, we'll find out via the internal
	     error below.  */

	case MY66000_OPS_B16:
	  {
	    bits = match_16bit_or_label (&sp, errmsg, &imm);
	    if (*errmsg)
	      break;
	    if (imm.X_op == O_symbol)
	      relax_imm = RELAX_BCND_16;
	  }
	  break;

	case MY66000_OPS_B26:
	  {
	    bits = match_26bit_or_label (&sp, errmsg, &imm);
	    if (*errmsg)
	      break;
	    if (imm.X_op == O_symbol)
	      relax_imm = my66000_is_call (iword) ? RELAX_CALL_26 : RELAX_BR_26;
	    break;
	  }
	  break;

	case MY66000_OPS_I32_PCREL:
	  relax_imm = RELAX_IMM_32_PCREL;
	  match_32_bit_or_label (&sp, errmsg, &imm);
	  if (*errmsg)
	    break;
	  imm_size = 4;
	  bits = 0;
	  break;
	  /* Fallthrough.  */

	case MY66000_OPS_I32_1:
	  match_32_bit_or_label (&sp, errmsg, &imm);
	  if (imm.X_op == O_symbol && mcmodel > SMALL)
	    {
	      sprintf(errbuf, "32-bit symbol for 64-bit constant");
	      *errmsg = errbuf;
	      break;
	    }

	  relax_imm = RELAX_IMM_32;
	  if (*errmsg)
	    break;
	  imm_size = 4;
	  bits = 0;
	  break;

	case MY66000_OPS_I32_2:
	  match_32_bit_or_label (&sp, errmsg, &imm_st);
	  if (*errmsg)
	    break;
	  imm_st_size = 4;
	  bits = 0;
	  break;

	  /* FIXME: Ideally, this should also be included in the fixups.
	     Right now, we don't do that.  */

	case MY66000_OPS_PCREL16:
	  bits = match_16bit (&sp, errmsg);
	  break;

	case MY66000_OPS_I16_HI:
	  {
	    uint16_t hi;
	    hi = match_16bit (&sp, errmsg);
	    if (*errmsg)
	      break;
	    imm.X_op = O_constant;
	    imm.X_add_number |= (hi << 16);
	    imm_size = 4;
	    bits = 0;
	    break;
	  }

	case MY66000_OPS_I16_LO:
	  {
	    uint16_t lo;
	    lo = match_16bit (&sp, errmsg);
	    if (*errmsg)
	      break;
	    imm.X_op = O_constant;
	    imm.X_add_number |= lo;
	    imm_size = 4;
	    bits = 0;
	    break;
	  }

	case MY66000_OPS_VEC32:
	  val_tmp = match_vec (&sp, errmsg, rname_map);
	  if (*errmsg)
	    break;
	  imm_size = 4;
	  bits = 0;
	  imm.X_op = O_constant;
	  imm.X_add_number = val_tmp;
	  break;

	case MY66000_OPS_I32_HEX:
	  val_tmp = match_hex (&sp, errmsg, 4);
	  if (*errmsg)
	    break;
	  imm.X_op = O_constant;
	  imm.X_add_number = val_tmp;
	  imm_size = 4;
	  bits = 0;
	  break;

	case MY66000_OPS_INS:
	  val_tmp = match_ins (&sp, errmsg, &imm);
	  if (*errmsg)
	    break;
	  imm.X_op = O_constant;
	  imm.X_add_number = val_tmp;
	  imm_size = 4;
	  bits = 0;
	  break;

	case MY66000_OPS_I64_HEX:
	  val_tmp = match_hex (&sp, errmsg, 8);
	  if (*errmsg)
	    break;
	  imm.X_op = O_constant;
	  imm.X_add_number = val_tmp;
	  imm_size = 8;
	  bits = 0;
	  break;

	case MY66000_OPS_I64_PCREL:
	  if (mcmodel < LARGE)
	    {
	      snprintf (errbuf, sizeof(errbuf),(_("64-bit offsets only valid for "
						  "-mcmodel=large")));
	      break;
	    }

	  /* Fallthrough.  */

	case MY66000_OPS_I64_1:
	  val_tmp = match_64_bit_or_label (&sp, errmsg, &imm);
	  if (*errmsg)
	    break;
	  relax_imm = RELAX_IMM_64;
	  imm.X_add_number = val_tmp;
	  imm_size = 8;
	  bits = 0;
	  break;

	  /* A 32-bit constant.  This can occur in stb, sth and stb,
	     unused parts are masked away.  */
	case MY66000_OPS_I32_ST:
	  {
	    uint32_t mask, sz;
	    sz = my66000_get_imm_sz (iword);
	    match_integer_expr_ex (&sp, errmsg, INT32_MIN, UINT32_MAX, &imm_st);

	    if (*errmsg)
	      break;
	    mask = (1lu << (8lu << sz)) -1;
	    imm_st.X_add_number &= mask;
	    imm_st_size = 4;
	    bits = 0;
	  }

	  break;

	case MY66000_OPS_I8_MS:
	  {
	    uint8_t u;

	    match_64_bit_vanilla (&sp, errmsg, &imm_st);
	    if (*errmsg)
	      break;

	    u = imm_st.X_add_number;
	    if (u < 16 || u >= (uint8_t) -16)
	      {
		snprintf (errbuf, sizeof(errbuf),
			  (_("Number could fit in five bits: %u")), u);
		*errmsg = errbuf;
		break;
	      }
	    imm_st.X_add_number = u;
	    imm_st_size = 4;
	    bits = 0;
	    break;
	  }
	case MY66000_OPS_I5_MS:
	  {
	    uint8_t u;
	    match_64_bit_vanilla (&sp, errmsg, &imm_st);
	    u = imm_st.X_add_number;
	    if (*errmsg)
	      break;
	    if (u >= 16 && u < (uint8_t) -16)
	      {
		snprintf (errbuf, sizeof(errbuf),
			  (_("Number does not fit in five bits: %u")), u);
		*errmsg = errbuf;
		break;
	      }
	    bits = u & 0x1f;
	    break;
	  }

	case MY66000_OPS_I64_ST:
	  match_64_bit_vanilla (&sp, errmsg, &imm_st);
	  if (*errmsg)
	    break;
	  imm_st_size = 8;
	  bits = 0;
	  break;

	default:
	  as_fatal ("operand '%c' not handled", *fp);
	}

      if (*errmsg)
	return;

      iword |= bits << info->shift;
    }

  if (*sp != '\0')
    *errmsg = _("junk at end of argument list");

  if (*errmsg)
    {
      if (p)
	as_fatal ("Internal error: failure after memory already allocated");

      //      fprintf (stderr, "errmsg = %s\n", *errmsg);
      //      fprintf (stderr,"failure\n");
      return;
    }

  iword |= spec->patt;

  if (((spec->patt ^ iword) & spec->mask) != 0)
    {
      as_fatal ("Internal error: pattern collides with iword fmt='%s'\n"
		"       3         2         1\n"
		"      10987654321098765432109876543210\n"
		"iword %32.32b\n"
		"patt  %32.32b\n"
		"mask  %32.32b\n",
		spec->fmt, iword, spec->patt, spec->mask);
    }
  //  fprintf (stderr, "matched : '%s' iword = %8.8x \n", spec->fmt, iword);

  if (relax_imm == RELAX_NONE && relax_imm_st == RELAX_NONE
      && imm_size == 0 && imm_st_size == 0)
    {
      dwarf2_emit_insn (0);
      p = frag_more (length);
      /* Dept. of dirty tricks:  We force all jtt instructions to jttb so
	 relaxation can figure out the correct size later.  */

      if (my66000_is_tt (iword))
	{
	  current_tt.old_frag = frag_now;
	  current_tt.p = p;
	  iword = my66000_set_tt_size (iword, 1);
	  //	  fprintf (stderr,"tt : p = %p\n", p);
	}
      /* else */
      /* 	{ */
      /* 	  current_tt.old_frag = NULL; */
      /* 	  current_tt.p = NULL; */
      /* 	} */

      md_number_to_chars (p, iword, 4);
      return;
    }

  if (relax_imm != RELAX_NONE && imm.X_op == O_symbol)
    {
      fragS *old_frag;
      dwarf2_emit_insn (0);
      old_frag = frag_now;
      p = frag_var (rs_machine_dependent,
		    8 + mcmodel,
		    relax_tab[relax_imm].size_var,
		    relax_imm,
		    imm.X_add_symbol,
		    imm.X_add_number,
		    0);  /* Schreib hier die Adresse rein.  */
      // fprintf (stderr,"iword = %x p = %p\n",iword,p);
      while (old_frag->fr_next->fr_next != NULL)
	old_frag = old_frag->fr_next;

      old_frag->fr_opcode = p;
      md_number_to_chars (p, iword, 4);
      imm_size = 0;
    }
  // p_op = p;

  if (!p)
    {
      dwarf2_emit_insn (0);
      p = frag_more (4);
      md_number_to_chars (p, iword, 4);
    }

  //  fprintf (stderr, "iword = %x p = %p\n", iword, p);
  /* Handle the immediates.  */
  if (imm_size > 0)
    {
      if (imm.X_op == O_constant)
	{
	  p = frag_more (imm_size);
	  md_number_to_chars (p, imm.X_add_number, imm_size);
	}
      else
	as_fatal ("Weird expression value");
    }
  if (imm_st_size > 0)
    {
      p = frag_more (imm_st_size);
      if (imm_st.X_op == O_symbol)
	{
	  int reloc_type;
	  if (imm_st_size == 4)
	    reloc_type = imm_st_pcrel ? BFD_RELOC_32_PCREL : BFD_RELOC_32;
	  else
	    reloc_type = imm_st_pcrel ? BFD_RELOC_64_PCREL : BFD_RELOC_64;
	  /* XXX */
	  fix_new_exp (frag_now,
		       p - frag_now->fr_literal,
		       imm_st_size,
		       &imm_st,
		       imm_st_pcrel,
		       reloc_type
		       );
	}
      else if (imm_st.X_op == O_constant)
	{
	  md_number_to_chars (p, imm_st.X_add_number, imm_st_size);
	}
      else
	as_fatal ("Weird expression value");
    }
#if 0
  fprintf (stderr,"%s:\t%s\timm_size = %d val_imm = %lu\n",
	   str, spec->fmt,imm_size, val_imm);
  fprintf (stderr,"%s:\t%s\timm_st_size = %d add_number = %lu\n",
	   str, spec->fmt,imm_st_size, imm_st.X_add_number);
#endif
  return;
}

static void
encode_instr (const my66000_opc_info_t *opc, char *str, char **errmsg)
{
  char *p;
  uint32_t iword;
  my66000_encoding enc;
  const my66000_opcode_fmt_t *fmtlist;
  const my66000_fmt_spec_t *spec;

  *errmsg = NULL;
  iword = opc->patt_opc;
  enc = opc->enc;
  fmtlist = &my66000_opcode_fmt[enc];

  spec = fmtlist->spec;

  /* If we don't have an argument list, we're done.  */

  if (spec == NULL || spec->fmt == NULL || spec->fmt[0] == '\0')
    {
      dwarf2_emit_insn (0);
      p = frag_more (4);
      //      fprintf (stderr,"no spec : %p\n", p);
      memcpy (p, &iword, 4);
      return;
    }

  /* Try to match the different operand specifications.  */
  for (spec = fmtlist->spec; spec->fmt; spec++)
    {
      *errmsg = NULL;
      match_arglist (iword, spec, str, errmsg);
      if (*errmsg == NULL)
	{
	  // fprintf (stderr,"success\n");
	  return;
	}
    }
  if (*errmsg == NULL)
    *errmsg = (_("garbeled operand list"));

  // fprintf (stderr,"failure\n");
}

/* This routine is called for each instruction to be assembled.  */

void
md_assemble (char *str)
{
  my66000_opc_info_t *opc;
  char buffer[MAX_OP_STR_LEN + 1];
  int i, j;
  char *errmsg;

  // fprintf(stderr,"md_assemble : frag_now->fr_literal = %p\n", frag_now->fr_literal);
  /* Drop leading whitespace.  */
  while (is_whitespace (*str))
    str++;

  /* Copy the instruction into the buffer, searching for the end.  */
  for (i = 0; i < MAX_OP_STR_LEN; i++)
    {
      char c = str[i];
      if (!c || is_end_of_stmt (c) || is_whitespace(c))
	break;
      buffer[i] = TOLOWER(c);
    }
  buffer[i] = '\0';

  if (i == 0)
    as_bad ("%s '%s'",_("Illegal instruction"), buffer);

  /* If we see an instruction while there are still .jt statements to do,
     this is an error.  */
  if (needs_jt)
    as_bad ("%d .jt labels still needed", needs_jt);

  /* We have up to N_MAP times the same assembler name, with different
     encodings, like "add r1, r22,#1234" vs.  "add r1,r22,r17".  This
     complicates things a bit because "add r1,r22,#1234" is also valid
     with a 32 bit or even a 64-bit immediate.  The strategy is to
     look up a first variant.  If this gives an error, check if there
     is a second variant.  If it doesn't exist, issue the latest
     error.

     Note: The ordering of what is put into each map matters in this.
  */

  //  fprintf (stderr,"%d: %s\n", num_carry, str);
  errmsg = NULL;
  for (j = 0; j < N_MAP; j++)
    {
      opc = (my66000_opc_info_t *) str_hash_find (s_opc_map[j], buffer);
      if (opc == NULL)
	{
	  if (errmsg != NULL)
	    as_bad ("%s: %s", buffer, errmsg);
	  else
	    as_bad ("illegal instruction '%s'", buffer);
	  return;
	}
      if (opc->enc == MY66000_CARRY && num_carry > 0)
	{
	  as_bad ("nested carry");
	  return;
	}

      errmsg = NULL;
      encode_instr (opc, &str[i], &errmsg);
      if (errmsg == NULL)
	break;
    }
  if (errmsg)
    {
      as_bad ("%s", errmsg);
      return;
    }
    if (num_carry > 0)
      num_carry --;
}

/* Put number into target byte order.  */
void
md_number_to_chars (char *ptr, valueT use, int nbytes)
{
  number_to_chars_littleendian (ptr, use, nbytes);
}

/* Translate internal representation of relocation info to BFD target
   format.  FIXME: This probably does not work yet.  */

arelent *
tc_gen_reloc (asection *section ATTRIBUTE_UNUSED, fixS *fixp)
{
  arelent *rel;
  bfd_reloc_code_real_type r_type;

  //    print_fixup (fixp);
  rel = xmalloc (sizeof (arelent));
  rel->sym_ptr_ptr = xmalloc (sizeof (asymbol *));
  *rel->sym_ptr_ptr = symbol_get_bfdsym (fixp->fx_addsy);
  rel->address = fixp->fx_frag->fr_address + fixp->fx_where;

  r_type = fixp->fx_r_type;
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
  rel->addend = fixp->fx_addnumber + fixp->fx_offset;
  return rel;
}

/* The location from which a PC relative jump should be calculated,
   given a PC relative reloc.  Always the start of the instruction,
   whose frag is found in tc_frag_data.old_frag.  */

long
md_pcrel_from_section (fixS *fixP, segT sec)
{
  //  print_fixup (fixP);
  if (fixP->fx_addsy != (symbolS *) NULL &&
      (!S_IS_DEFINED (fixP->fx_addsy) ||
       (S_GET_SEGMENT (fixP->fx_addsy) != sec) ||
       S_IS_EXTERNAL (fixP->fx_addsy) || S_IS_WEAK (fixP->fx_addsy)))
    {
      /* The symbol is undefined (or is defined but not in this section).
	 Let the linker figure it out.  */
      return 0;
    }

  return get_opc_addr (fixP->fx_frag);
}

/* Calculate a PC-relative offset.  These are always relative to the
   start of the instruction, which we record in a pointer stashed
   away in the offset field.  */

static offsetT
calc_relative_offset (fragS *fragP)
{
  offsetT target_address = S_GET_VALUE (fragP->fr_symbol) + fragP->fr_offset;
  offsetT opcode_address = get_opc_addr (fragP);
  //  fprintf(stderr,"calc_relative_offset: %s S_GET_VALUE=%ld\n",S_GET_NAME(fragP->fr_symbol),
  //	  (long int) S_GET_VALUE(fragP->fr_symbol));
  return target_address - opcode_address;
}

/* Check if we know the numeric value of the symbol.  */

static bool
known_frag_symbol (fragS *fragP, segT segment)
{
  return fragP->fr_symbol != NULL
    && S_IS_DEFINED (fragP->fr_symbol)
    && !S_IS_WEAK (fragP->fr_symbol)
    && segment == S_GET_SEGMENT (fragP->fr_symbol);
}

#if 0

/* Check the range for diferent relocaltions shifted by two.  */

static void
check_reloc_range (valueT valu, int bits, fixS *fixP)
{
  offsetT vmax = ((valueT) 1 << (bits - 1)) - 1;
  offsetT vmin = - ((valueT) 1 << (bits - 1));
  offsetT vals = (offsetT) valu;
  if (vals < vmin || vals > vmax)
    as_bad_where (fixP->fx_file, fixP->fx_line,
		  "relocation overflow: %ld not between %ld and %ld",
		  (long int) vals, (long int) vmin, (long int) vmax);
}

#endif

void
md_apply_fix (fixS *fixP, valueT * valP, segT seg)
{
  char *buf = fixP->fx_where + fixP->fx_frag->fr_literal;
  uint32_t iword;
  uint16_t val16;
  uint32_t val32;
  uint64_t val64;
  uint8_t val8;
  offsetT val = *valP;

  /* Don't do anything for a different segment.  */

  if (fixP->fx_addsy != (symbolS *) NULL &&
      (!S_IS_DEFINED (fixP->fx_addsy) ||
       (S_GET_SEGMENT (fixP->fx_addsy) != seg) ||
       S_IS_EXTERNAL (fixP->fx_addsy) || S_IS_WEAK (fixP->fx_addsy)))
    return;

  /* FIXME: Look up the masks etc from the tables, eventually.  */
  /* For the fixups that are shifted, we do the range checking here.  */

  //  fprintf(stderr,"md_apply_fix: literal = %p buf = %p where=%lu\n", fixP->fx_frag->fr_literal,
  //	  buf,fixP->fx_where);
  switch (fixP->fx_r_type)
    {
    case BFD_RELOC_26_PCREL_S2:
      //      check_reloc_range (*valP, 28, fixP);
      iword = (uint32_t) bfd_getl32 (buf);
      iword |= (val / 4) & 0x3ffffff;
      bfd_putl32 ((bfd_vma) iword, buf);
      fixP ->fx_no_overflow = 1;
      break;

    case BFD_RELOC_16_PCREL_S2:
      //      check_reloc_range (*valP, 18, fixP);
      val16 = val / 4;
      bfd_putl16 ((bfd_vma) val16, buf);
      fixP ->fx_no_overflow = 1;
      break;

    case BFD_RELOC_32_PCREL:
      bfd_putl32 ((bfd_vma) *valP, buf);
      break;

    case BFD_RELOC_32_PCREL_S2:
      //      check_reloc_range (*valP, 32, fixP);
      val32 = val / 4;
      bfd_putl32 ((bfd_vma) val32, buf);
      fixP ->fx_no_overflow = 1;
      break;

    case BFD_RELOC_64_PCREL:
      bfd_putl64 ((bfd_vma) *valP, buf);
      break;

    case BFD_RELOC_64_PCREL_S2:
      //      check_reloc_range (*valP, 64, fixP);
      val64 = val / 4;
      bfd_putl64 ((bfd_vma) val64, buf);
      fixP ->fx_no_overflow = 1;
      break;

    case BFD_RELOC_8:
      *buf = *valP;
      break;

    case BFD_RELOC_8_PCREL_S2:
      //      check_reloc_range (*valP, 10, fixP);
      val8 = val / 4;
      *buf = val8;
      fixP ->fx_no_overflow = 1;
      break;

    case BFD_RELOC_16:
      bfd_putl16 ((bfd_vma) *valP, buf);
      break;

    case BFD_RELOC_32:
      bfd_putl32 ((bfd_vma) *valP, buf);
      break;

    case BFD_RELOC_64:
      bfd_putl64 ((bfd_vma) *valP, buf);
      break;
    default:
      as_fatal ("Unknown relocation %s in md_apply_fix",
		bfd_get_reloc_code_name(fixP->fx_r_type));
      break;
    }

  if (fixP->fx_addsy == NULL && fixP->fx_pcrel == 0)
    fixP->fx_done = 1;

  return;
}

/* Find the smallest relaxation for a frag.  */

static relax_type
find_smallest_relaxation (fragS *fragP, segT segment)
{
  relax_type relax = fragP->fr_subtype;
  offsetT offset;
  relax_type smallest, i;

  smallest = relax_tab[relax].smallest;

  if (smallest == 0)
    return relax;

  gas_assert (relax_tab[relax].relax_num == relax);
  if (!known_frag_symbol (fragP, segment))
    {
      for (i = smallest; relax_tab[i].smallest != 0; i++)
	{
	  if (relax_tab[i+1].mcmodel > mcmodel)
	    return i;
	}
      return i;
    }

  offset = calc_relative_offset (fragP);

  for (i = smallest; relax_tab[i].smallest == smallest; i++)
    {
      offsetT min = RANGE_MIN (relax_tab[i].bits);
      offsetT max = RANGE_MAX (relax_tab[i].bits);
      if (offset >= min && offset <= max)
	break;
    }
  return i;
}

int
md_estimate_size_before_relax (fragS *fragP, segT segment)
{
  relax_type smallest;

  if (fragP->fr_subtype == RELAX_TT_FIRST)
    return 1;

  smallest = find_smallest_relaxation (fragP, segment);
  fragP->fr_var = relax_tab[smallest].size_var;
  fragP->fr_subtype = relax_tab[smallest].relax_num;
  return fragP->fr_var;
}


static _Bool
is_tt_frag (fragS *fragP)
{
  switch (fragP->fr_subtype)
    {
    case RELAX_TT_FIRST:
    case RELAX_TT_8_S2:
    case RELAX_TT_16_S2:
    case RELAX_TT_32_S2:
    case RELAX_TT_64_S2:
      return 1;
    default:
      return 0;
    }
}

static int
relax_tt_frag (segT seg, fragS *fragP)
{
  offsetT old_var = fragP->fr_var;
  offsetT smallest_var, tt_var;
  uint32_t *ip, iword;
  relax_type smallest;

  if (fragP->fr_subtype == RELAX_TT_FIRST)
    {
      fragP->fr_subtype = RELAX_TT_8_S2;
      return 1;
    }

  smallest = find_smallest_relaxation (fragP, seg);
  smallest_var = relax_tab[smallest].size_var;
  ip = (uint32_t *) fragP->fr_opcode;
  iword = *ip;
  tt_var = my66000_get_tt_size (iword);
  //  fprintf(stderr,"smallest_var = %ld tt_var = %ld\n", smallest_var, tt_var);
  if (tt_var < smallest_var)
    {
      iword = my66000_set_tt_size (iword, smallest_var);
      *ip = iword;
    }
  else if (tt_var > smallest_var)
    {
      smallest_var = tt_var;
    }
  fragP->fr_var = smallest_var;
  switch (smallest_var)
    {
    case 1:
      fragP->fr_subtype = RELAX_TT_8_S2;
      break;
    case 2:
      fragP->fr_subtype = RELAX_TT_16_S2;
      break;
    case 4:
      fragP->fr_subtype = RELAX_TT_32_S2;
      break;
    case 8:
      fragP->fr_subtype = RELAX_TT_64_S2;
      break;
    default:
      as_fatal (_("relax_tt_frag: smallest_var = %d"),smallest);
    }
  return fragP->fr_var - old_var;
}

int
my66000_relax_frag (segT seg, fragS *fragP,
		    long stretch ATTRIBUTE_UNUSED)
{
  offsetT old_var = fragP->fr_var;
  relax_type smallest;

  if (is_tt_frag(fragP))
    return relax_tt_frag (seg, fragP);

  old_var = fragP->fr_var;
  smallest = find_smallest_relaxation (fragP, seg);
  fragP->fr_var = relax_tab[smallest].size_var;
  fragP->fr_subtype = smallest;
  return fragP->fr_var - old_var;
}

void
md_convert_frag (bfd *abfd ATTRIBUTE_UNUSED,
		 segT sec ATTRIBUTE_UNUSED,
		 fragS *fragP)
{
  relax_type relax;
  addressT offset;
  fixS *fixp;

  relax = fragP->fr_subtype;

  if (relax_tab[relax].set_opc)
    offset = relax_tab[relax].set_opc (fragP->fr_opcode, fragP->fr_var);
  else
    offset = 0;

  //  fprintf(stderr,"convert_frag: %s %ld\n", S_GET_NAME(fragP->fr_symbol),
  //	  (long int) S_GET_VALUE(fragP->fr_symbol));
  fixp = fix_new (fragP,
		  fragP->fr_fix + offset,
		  fragP->fr_var - offset,
		  fragP->fr_symbol,
		  fragP->fr_offset,
		  relax_tab[relax].pcrel,
		  relax_tab[relax].reloc_num);
  // print_fixup(fixp);
  fixp->fx_addnumber += offset;
  /* Special casing because these are, in fact, two instructions.  Should
     ideally be done via a table, but well... */

  if (relax == RELAX_BCND_32 || relax == RELAX_BCND_64 || relax == RELAX_IMM_32
      || relax == RELAX_IMM_64)
    fixp->fx_addnumber -= 4;

  fragP->fr_fix += fragP->fr_var;
}

/* Handle jump tables.  We simply ignore the size the compiler tells us
   and use relaxation to figure it out later.  Only look at expressions.  */

static void
handle_jt (int num ATTRIBUTE_UNUSED)
{
  char *endp, *cp;
  char saved_char;
  expressionS ex;
  char *errmsg = NULL;
  fragS *old_frag;

  //  fprintf (stderr,"handle_it : num = %d needs_jt = %d is_tt = %d\n", num, needs_jt,
  //	   my66000_is_tt (this_iword));

  if (needs_jt <= 0)
    {
      as_bad (".jt directive witout preceding jump table instruction");
      return;
    }

  cp = input_line_pointer;
  while (needs_jt > 0)
    {
      endp = cp;
      while (1)
	{
	  if (*endp == '\0' || *endp == ',' || *endp == ']' || *endp == ':'
	      || *endp == '<' || *endp == '>'
	      || is_end_of_stmt (*endp))
	    break;
	  endp ++;
	}

      saved_char = *endp;
      *endp = '\0';
      input_line_pointer = cp;
      expression_and_evaluate (&ex);
      *endp = saved_char;
      if (ex.X_op != O_symbol)
	as_bad ("Need symbol in .jt directive");
      needs_jt --;
      cp = endp;

      /* Insert frag_var here.  */

      old_frag = frag_now;
      frag_var (rs_machine_dependent,
		8,
		1,
		RELAX_TT_FIRST,
		ex.X_add_symbol,
		ex.X_add_number,
		0);

      while (old_frag->fr_next->fr_next != NULL)
	old_frag = old_frag->fr_next;

      old_frag->tc_frag_data.old_frag = current_tt.old_frag;
      old_frag->fr_opcode = current_tt.p;
      if (needs_jt > 0)
	{
	  match_character (',', &cp, &errmsg);
	  if (errmsg)
	    as_bad ("%s", errmsg);
	}
    }

  input_line_pointer = cp;

  /* Aligh to a four-byte boundary - there should be a .p2align 2
     there, but just in case the user forgot.  */

  do_align (2, (char *) 0, 0, 0);
}
