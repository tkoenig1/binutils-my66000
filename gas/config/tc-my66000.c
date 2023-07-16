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

/* Stuff for handling the pseudo-ops.  */
static void handle_jt (int);
static int needs_jt;

const pseudo_typeS md_pseudo_table[] =
{
 { "jt8",  handle_jt, 1 },
 { "jt16", handle_jt, 2 },
 { "jt32", handle_jt, 4 },
 { "jt64", handle_jt, 8 },

 /* .dword is just what other people call .quad.  */
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

static opcode_pos_t *opc_pos_list, *current_jt;

/* Allocate a new value and put it in the linked list.  */

static char *
opc_pos (char *p)
{
  opcode_pos_t *n;
  n = XCNEW (opcode_pos_t);
  n->frag = frag_now;
  n->pos = p - frag_now->fr_literal;
  n->next = opc_pos_list;
  opc_pos_list = n;
  //  fprintf (stderr,"opc_pos: n = %p frag = %p pos = %ld\n", n, n->frag, n->pos);
  return (char *) n;
}

/* Calculate the address of the instruction we set with opc_pos
 from the fr_opcde field.  */

static addressT
get_opc_addr (char *p)
{
  opcode_pos_t *fp = (opcode_pos_t *) p;
  //  fprintf (stderr,"get_opc_addr: fp = %p frag = %p\n", fp, fp->frag);
  return fp->frag->fr_address + fp->pos;
}

/* Get a pointer to an iword from our modified fr_opcode pointer.  */

static uint32_t *
get_opc_insn (char *p)
{
  opcode_pos_t *fp = (opcode_pos_t *) p;
  fragS *frag;
  uint32_t *ip;

  frag = fp->frag;
  ip = (uint32_t *) (frag->fr_literal + fp->pos);
  return ip;
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
  return 0;
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

#define N_MAP 5

static htab_t s_opc_map[N_MAP];

static void
build_opc_hashes (const my66000_opc_info_t * table)
{
  for (int i = 0; table[i].enc != MY66000_END; i++)
    {
      void ** slot;
      int j;

      if (table[i].enc == MY66000_BAD)
	continue;

      if (table[i].name != NULL)
	{
	  /* Loop over the N_MAP available hashmaps and look for a place
	     to store the name.  */
	  for (j = 0; j < N_MAP; j++)
	    {
	      slot = str_hash_insert (s_opc_map[j], table[i].name,
				      (void *) &table[i], 0);
	      if (slot == NULL)
		break;
	    }
	  if (j == N_MAP)
	    as_fatal (_ ("Internal error: more than %d equal opcodes: %s"),
		      N_MAP, table[i].name);
	}
    }

}

static htab_t rname_map, rbase_map, rind_map;
static htab_t hr_ro_map, hr_rw_map;
static htab_t vec_map;

#define MAX_REG_STR_LEN 10

void
md_begin (void)
{
   /* Build hashes for looking up the instructions.  */
  const my66000_opc_info_t **lst = my66000_opc_info_list;

  for (int j=0; j<N_MAP; j++)
    s_opc_map[j] = str_htab_create ();

  for (int k=0; lst[k]; k++)
    build_opc_hashes (lst[k]);

  /* Build hash for the register names.  */
  rname_map = str_htab_create ();
  rbase_map = str_htab_create ();
  rind_map  = str_htab_create ();
  for (int i = 0; i < 32; i++)
    {
      str_hash_insert (rname_map, my66000_rname[i],
		       (void *) &my66000_numtab[i], 0);
      str_hash_insert (rbase_map, my66000_rbase[i],
		       (void *) &my66000_numtab[i], 0);
      str_hash_insert (rind_map, my66000_rind[i],
		       (void *) &my66000_numtab[i], 0);
    }

  for (my66000_reg_alias_t * p = my66000_reg_alias; p->name; p++)
    {
      str_hash_insert (rname_map, p->name, (void *) &p->num, 0);
      str_hash_insert (rbase_map, p->name, (void *) &p->num, 0);
    }

  /* We treat the functions for HR as sort of register names.  */

  hr_ro_map = str_htab_create();
  hr_rw_map = str_htab_create();

  for (int i = 0; i < 16; i++)
    {
      if (my66000_hr_ro[i])
	str_hash_insert (hr_ro_map, my66000_hr_ro[i],
			(void *) &my66000_numtab[i], 0);
      if (my66000_hr_rw[i])
	str_hash_insert (hr_rw_map, my66000_hr_rw[i],
			 (void *) &my66000_numtab[i], 0);
    }

  /* Seting up the table for the VEC instruction, bitmap style.  */
  vec_map = str_htab_create();
  for (int i = 0; i < MY66000_VEC_BITS; i++)
    str_hash_insert (vec_map, my66000_vec_reg[i],(void *) &my66000_numtab[i], 0);

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
}


static char errbuf[100];

/* Match a non-whitespace character required by the syntax.  Issue
   error or advance ptr past the matched character.  */

static void
match_character (char c, char **ptr, char **errmsg)
{
  char *s;

  for (s = *ptr; ISSPACE(*s); s++)
    ;

  if (*s != c)
    {
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
  while (ISSPACE (*str))
    str++;

  input_line_pointer = str;

  /* Look for a delimiter and make sure that expr sees a
     null-terminated string.  */

  endp = str;
  while (1)
    {
      if (*endp == '\0' || *endp == ',' || *endp == ']' || *endp == ':'
	  || *endp == '<' || *endp == '>'
	  || is_end_of_line[(unsigned char) *endp])
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

/* Match an integer between 0 and 8 inclusive, this is for
   prediate clauses.  */

static int
match_max8 (char **ptr, char **errmsg)
{
  return match_integer (ptr, errmsg, 0, 8);
}

/* Match a six-bit positive constant.  */

static uint8_t
match_6bit (char **ptr, char **errmsg)
{
  return match_integer (ptr, errmsg, 0, 63);
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

static uint16_t
match_tf_list (char **ptr, char **errmsg)
{
  int num_true, num_false;
  char *str;

  str = *ptr;

  /* Drop leading whitespace.  */
  while (ISSPACE (*str))
    str++;

  num_true = 0;

  while (TOUPPER (*str) == 'T')
    {
      num_true++;
      str++;
    }
  if (num_true > 8)
    {
      strcpy (errbuf, "Too many true values");
      *errmsg = errbuf;
      return 0;
    }

  num_false = 0;
  while (TOUPPER (*str) == 'F')
    {
      num_false++;
      str++;
    }
  if (num_false > 8)
    {
      strcpy (errbuf, "Too many false values");
      *errmsg = errbuf;
      return 0;
    }
  *ptr = str;
  return (num_true) << 8 | num_false;
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
  while (ISSPACE (*s))
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
      snprintf (errbuf, sizeof(errbuf), "%s: %s", (_("bad register name")), buf);
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
  while (ISSPACE (*str))
    str++;

  input_line_pointer = str;

  endp = str;
  while (1)
    {
      if (*endp == '\0' || *endp == ',' || *endp == ']' || *endp == ':'
	  || *endp == '<' || *endp == '>'
	  || is_end_of_line[(unsigned char) *endp])
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
match_32_bit_vanilla (char **ptr, char **errmsg, expressionS *ex)
{
  return match_num_or_label (ptr, errmsg, ex, 32, true);
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

/* Match an INS pattern <a:b>.  */

static uint32_t
match_ins (char **ptr, char **errmsg, expressionS *ex)
{
  uint32_t v1, v2;
  v1 = match_6bit (ptr, errmsg);
  if (*errmsg)
    return 0;
  match_character (':', ptr, errmsg);
  if (*errmsg)
    return 0;
  v2 = match_6bit (ptr, errmsg);
  if (*errmsg)
    return 0;
  ex->X_op = O_constant;
  ex->X_add_number = (v1 << 6) + v2;
  return (v1 << 6) + v2;
}

#define RELAX_IMM4_PCREL	1
#define RELAX_IMM8_PCREL	2
#define RELAX_TT		3

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
  char *p = NULL, *p_op;
  int length = 4;
  expressionS imm, imm_st;
  int imm_size = 0, imm_st_size = 0;
  uint64_t val_imm = 0;
  _Bool imm_pcrel = false;
  int prthen = -1, prelse = -1;

  //  fprintf (stderr,"match_arglist : iword = %8.8x '%s' '%s'\n", iword, str, spec->fmt);
  for (; *fp; fp++)
    {
      //      fprintf (stderr, "fp = %s sp = %s\n", fp, sp);
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
	case MY66000_OPS_RBASE:
	  bits = match_register (&sp, errmsg, rbase_map);
	  break;
	case MY66000_OPS_HRRO:
	  bits = match_register (&sp, errmsg, hr_ro_map);
	  break;
	case MY66000_OPS_HRRW:
	  bits = match_register (&sp, errmsg, hr_rw_map);
	  break;
	case MY66000_OPS_IMM16:
	  bits = match_16bit (&sp, errmsg);
	  break;
	case MY66000_OPS_IMM13:
	  bits = match_16bit (&sp, errmsg);
	  if (bits & 0x7)
	    *errmsg = _("Incorrect alignment of disp13");
	  break;
	case MY66000_OPS_FL_ENTER:
	  bits = match_3bit (&sp, errmsg);
	  break;
	case MY66000_OPS_I1:
	case MY66000_OPS_I2:
	  bits = match_5bitu (&sp, errmsg);
	  break;

	case MY66000_OPS_SI5:
	  bits = match_5bits (&sp, errmsg);
	  break;
	case MY66000_OPS_BB1:
	case MY66000_OPS_WIDTH:
	case MY66000_OPS_OFFSET:
	  bits = match_6bit (&sp, errmsg);
	  break;

	case MY66000_OPS_W_BITR:
	  bits = match_6bit_p2 (&sp, errmsg);
	  break;

	case MY66000_OPS_UIMM16:
	  bits = match_16bit_u (&sp, errmsg);
	  needs_jt = bits + 1;
	  break;

	case MY66000_OPS_MSCALE:
	  bits = match_scale (&sp, errmsg);
	  break;

	case MY66000_OPS_CARRY:
	  bits = match_carry_list (&sp, errmsg);
	  break;

	case MY66000_OPS_TF:
	  bits = match_tf_list (&sp, errmsg);
	  break;

	case MY66000_OPS_PRTHEN:
	  bits = match_max8 (&sp, errmsg);
	  prthen = bits;
	  break;

	case MY66000_OPS_PRELSE:
	  bits = match_max8 (&sp, errmsg);
	  prelse = bits;
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
	    expressionS ex;
	    bits = match_16bit_or_label (&sp, errmsg, &ex);
	    if (*errmsg)
	      break;
	    if (ex.X_op == O_symbol)
	      {
		p = frag_more (length);
		fix_new_exp (frag_now,
			     p - frag_now->fr_literal,  /* where */
			     2,  /* size.  */
			     &ex, /* expression.  */
			     1,  /* pcrel.  */
			     BFD_RELOC_16_PCREL_S2
			     );
	      }
	  }
	  break;

	case MY66000_OPS_B26:
	  {
	    expressionS ex;
	    bits = match_26bit_or_label (&sp, errmsg, &ex);
	    if (*errmsg)
	      break;
	    if (ex.X_op == O_symbol)
	      {
		p = frag_more (length);
		fix_new_exp (frag_now,
			     p - frag_now->fr_literal,  /* where */
			     4,  /* size.  */
			     &ex, /* expression.  */
			     1,  /* pcrel.  */
			     BFD_RELOC_26_PCREL_S2
			     );
	      }
	  }
	  break;

	case MY66000_OPS_I32_PCREL:
	  imm_pcrel = true;
	  /* Fallthrough.  */

	case MY66000_OPS_I32_1:
	  val_imm = match_32_bit_or_label (&sp, errmsg, &imm);
	  if (*errmsg)
	    break;
	  imm_size = 4;
	  bits = 0;
	  break;

	case MY66000_OPS_I32_HEX:
	  val_imm = match_32_bit_vanilla (&sp, errmsg, &imm);
	  if (*errmsg)
	    break;
	  imm_size = 4;
	  bits = 0;
	  break;

	case MY66000_OPS_INS:
	  val_imm = match_ins (&sp, errmsg, &imm);
	  if (*errmsg)
	    break;
	  imm_size = 4;
	  bits = 0;
	  break;

	case MY66000_OPS_I64_HEX:
	  val_imm = match_64_bit_vanilla (&sp, errmsg, &imm);
	  if (*errmsg)
	    break;
	  imm_size = 8;
	  bits = 0;
	  break;

	case MY66000_OPS_I64_PCREL:
	  imm_pcrel = true;
	  /* Fallthrough.  */

	case MY66000_OPS_I64_1:
	  val_imm = match_64_bit_or_label (&sp, errmsg, &imm);
	  if (*errmsg)
	    break;
	  imm_size = 8;
	  bits = 0;
	  break;


	  /* A 32-bit constant.  This can occur in stb, sth and stb,
	     unused parts are masked away.  */
	case MY66000_OPS_I32_ST:
	  {
	    uint32_t mask, sz;
	    sz = my66000_get_imm_sz(iword);
	    match_integer_expr_ex (&sp, errmsg, INT32_MIN, UINT32_MAX, &imm_st);

	    if (*errmsg)
	      break;
	    mask = (1lu << (8lu << sz)) -1;
	    imm_st.X_add_number &= mask;
	    imm_st_size = 4;
	    bits = 0;
	  }

	  break;

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
  else if (prthen == 0 && prelse == 0)
    *errmsg = _("zero-length predicates not allowed");

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
      as_fatal ("Internal error: fmt='%s' iword = %8.8x patt = %8.8x mask = %8.8x",
		spec->fmt, iword, spec->patt, spec->mask);
    }
  //  fprintf (stderr, "matched : '%s' iword = %8.8x \n", spec->fmt, iword);
  /* Dept. of dirty tricks:  We force all jtt instructions to jttb so
     relaxation can figure out the correct size later.  */

  if (my66000_is_tt (iword))
    iword = my66000_set_tt_size (iword, 1);

  if (!p)
    p = frag_more (length);

  //  printf ("p = %p\n", p);
  md_number_to_chars (p, iword, 4);

  /* Remember if we're in a jump table instruction or not.  */
  if (my66000_is_tt (iword))
    current_jt = (opcode_pos_t *) opc_pos (p);
  else
    current_jt = NULL;

  if (imm_size == 0 && imm_st_size == 0)
    return;

  p_op = p;
  //  fprintf (stderr, "match_arglist: iword = %8.8x\n", iword);

  //  fprintf (stderr, "iword = %x p = %p\n", iword, p);
  /* Handle the immediates.  */
  if (imm_size > 0)
    {
      if (imm.X_op == O_symbol)
	{
	  gas_assert (imm_size == 4);

	  //	  fprintf (stderr,"imm_pcrel = %d reloc_type = %d p_op = %p\n", imm_pcrel, reloc_type, p_op);
#if 0
	  fix_new_exp (frag_now,
		       p - frag_now->fr_literal,
		       imm_size,
		       &imm,
		       imm_pcrel,
		       reloc_type
		       );
#else
	  frag_var (rs_machine_dependent, /* type */
		    8, /* max_chars */
		    4,  /* var, the number that is variable. */
		    RELAX_IMM4_PCREL, /* subtype  */
		    imm.X_add_symbol, /* symbol */
		    imm.X_add_number,    /* offset */
		    opc_pos(p_op));   /* Position of opcode.  */
#endif
	}
      else if (imm.X_op == O_constant)
	{
	  p = frag_more (imm_size);
	  md_number_to_chars (p, val_imm, imm_size);
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
	    reloc_type = imm_pcrel ? BFD_RELOC_32_PCREL : BFD_RELOC_32;
	  else
	    reloc_type = imm_pcrel ? BFD_RELOC_64_PCREL : BFD_RELOC_64;

	  fix_new_exp (frag_now,
		       p - frag_now->fr_literal,
		       imm_st_size,
		       &imm_st,
		       imm_pcrel,
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
  //  fprintf (stderr,"%s:\t%s\timm_size = %d\n", str, spec->fmt,imm_size);
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

  /* Drop leading whitespace.  */
  while (ISSPACE (*str))
    str++;

  /* Copy the instruction into the buffer, searching for the end.  */
  for (i = 0; i < MAX_OP_STR_LEN; i++)
    {
      char c = str[i];
      if (!c || is_end_of_line[(unsigned char) c] || ISSPACE(c))
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
   given a PC relative reloc.  Always the start of the instruction.   */

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

  switch (fixP->fx_frag->fr_subtype)
    {
    case RELAX_IMM4_PCREL:
    case RELAX_IMM8_PCREL:
    case RELAX_TT:
      return get_opc_addr (fixP->fx_frag->fr_opcode);
    default:
      return fixP->fx_where + fixP->fx_frag->fr_address;
    }
}

/* Calculate a PC-relative offset.  These are always relative to the
   start of the instruction, which we record in a pointer stashed
   away in the offset field.  */

static offsetT
calc_relative_offset (fragS *fragP)
{
  offsetT target_address = S_GET_VALUE (fragP->fr_symbol) + fragP->fr_offset;
  offsetT opcode_address = get_opc_addr (fragP->fr_opcode);
  //  fprintf (stderr, "calc_relative_offset: %ld %ld\n", target_address, opcode_address);
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

void
md_apply_fix (fixS *fixP, valueT * valP, segT seg ATTRIBUTE_UNUSED)
{
  char *buf = fixP->fx_where + fixP->fx_frag->fr_literal;
  uint32_t iword;
  uint16_t val16, val32;
  uint8_t val8;

  /* Remember value for tc_gen_reloc.  */
  fixP->fx_addnumber = *valP;

  //  fprintf (stderr,"md_apply_fix: *valP = %ld\n", *valP);
  /* FIXME: Look up the masks etc from the tables, eventually.  */
  switch (fixP->fx_r_type)
    {
    case BFD_RELOC_26_PCREL_S2:
      iword = (uint32_t) bfd_getl32 (buf);
      iword |= (*valP >> 2) & 0x3ffffff;
      bfd_putl32 ((bfd_vma) iword, buf);
      break;

    case BFD_RELOC_16_PCREL_S2:
      val16 = *valP >> 2;
      bfd_putl16 ((bfd_vma) val16, buf);
      break;

    case BFD_RELOC_32_PCREL:
      bfd_putl32 ((bfd_vma) *valP, buf);
      break;

    case BFD_RELOC_32_PCREL_S2:
      val32 = *valP >> 2;
      bfd_putl32 ((bfd_vma) val32, buf);
      break;

    case BFD_RELOC_64_PCREL:
      bfd_putl64 ((bfd_vma) *valP, buf);
      break;

    case BFD_RELOC_8:
      *buf = *valP;
      break;

    case BFD_RELOC_8_PCREL_S2:
      val8 = *valP >> 2;
      *buf = val8;
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
      as_fatal ("Unknown relocation %d in md_apply_fix",
		(int) fixP->fx_r_type);
      break;
    }

  if (fixP->fx_addsy == NULL && fixP->fx_pcrel == 0)
    fixP->fx_done = 1;

}

/* Calculate the length of an IP-relative offset that can be
   either 4 or 8 bytes.  If we don't know, return 8.  */

static int
relaxed_imm_length (fragS *fragP, segT segment, _Bool update)
{
  int ret;

  if (known_frag_symbol (fragP, segment))
    {
      offsetT val = calc_relative_offset (fragP);
      if (val > INT32_MIN && val < INT32_MAX)
	ret = 4;
      else
	ret = 8;
    }
  else
    ret = 8;

  if (update)
    {
      uint32_t *ip;
      ip = get_opc_insn (fragP->fr_opcode);
      if (ret == 4)
	fragP->fr_subtype = RELAX_IMM4_PCREL;
      else
	fragP->fr_subtype = RELAX_IMM8_PCREL;

      *ip = my66000_set_mem_size (*ip, ret);
    }
  return ret;
}


#define TT_MIN(bits) (-((offsetT) 1 << (bits - 1)))
#define TT_MAX(bits) (((offsetT) 1 << (bits - 1)) - 1)

/* Same, but for a TT relaxation, which is shifted by two bits.  */

static int
relaxed_tt_length (fragS *fragP, segT segment, _Bool update)
{
  int ret;
  uint32_t *ip;
  int size_insn;

  gas_assert (fragP->fr_subtype == RELAX_TT);
  ip = get_opc_insn (fragP->fr_opcode);
  size_insn = my66000_get_tt_size (*ip);
  if (known_frag_symbol (fragP, segment))
    {
      offsetT val = calc_relative_offset (fragP);
      if (val >= TT_MIN(8) && val <= TT_MAX(8))
	{
	  ret = 1;
	}
      else if (val >= TT_MIN(16) && val <= TT_MAX(16))
	{
	  ret = 2;
	}
      else if (val >= TT_MIN(32) && val <= TT_MAX(32))
	{
	  ret = 4;
	}
      else
	ret = 8;
    }
  else
    ret = 8;

  if (size_insn > ret)
    ret = size_insn;

  else if (update)
    *ip = my66000_set_tt_size (*ip, ret);

  return ret;
}

int
md_estimate_size_before_relax (fragS *fragP, segT segment)
{
  switch (fragP->fr_subtype)
    {
    case RELAX_IMM4_PCREL:
    case RELAX_IMM8_PCREL:
      fragP->fr_var = relaxed_imm_length (fragP, segment, false);
      break;
    case RELAX_TT:
      fragP->fr_var = relaxed_tt_length (fragP, segment, false);
      break;
    }
  return fragP->fr_var;
}

int
my66000_relax_frag (segT seg, fragS *fragP,
		    long stretch ATTRIBUTE_UNUSED)
{
  offsetT old_var = fragP->fr_var;

  switch (fragP->fr_subtype)
    {
    case RELAX_IMM4_PCREL:
    case RELAX_IMM8_PCREL:
      fragP->fr_var = relaxed_imm_length (fragP, seg, true);
      break;
    case RELAX_TT:
      fragP->fr_var = relaxed_tt_length (fragP, seg, true);
      break;
    default:
      as_fatal ("Subtype %d not handled", fragP->fr_subtype);
    }

  return fragP->fr_var - old_var;
}

void
md_convert_frag (bfd *abfd ATTRIBUTE_UNUSED,
		 segT sec ATTRIBUTE_UNUSED,
		 fragS *fragP)
{
  uint32_t iword;
  uint32_t *ip;
  expressionS ex;

  ip = get_opc_insn (fragP->fr_opcode);
  iword = bfd_getl32 ((char *) ip);

  ex.X_op = O_symbol;
  ex.X_add_symbol = fragP->fr_symbol;
  ex.X_add_number = fragP->fr_offset;

  if (fragP->fr_subtype == RELAX_IMM4_PCREL || fragP->fr_subtype == RELAX_IMM8_PCREL)
    {
      if (fragP->fr_var == 4)
	{
	  fix_new_exp (fragP, fragP->fr_fix, 4, &ex, true, BFD_RELOC_32_PCREL);
	  iword = my66000_set_imm_size (iword, 4);
	}
      else
	{
	  fix_new_exp (fragP, fragP->fr_fix, 8, &ex, true, BFD_RELOC_64_PCREL);
	  iword = my66000_set_imm_size (iword, 8);
	}
      *ip = iword;
    }

  else if (fragP->fr_subtype == RELAX_TT)
    {
      uint32_t size;
      bfd_reloc_code_real_type reloc;
      size = my66000_get_tt_size (iword);
      switch (size)
	{
	case 1:
	  reloc = BFD_RELOC_8_PCREL_S2;
	  break;
	case 2:
	  reloc = BFD_RELOC_16_PCREL_S2;
	  break;
	case 4:
	  reloc = BFD_RELOC_32_PCREL_S2;
	  break;
	case 8:
	  reloc = BFD_RELOC_64_PCREL_S2;
	  break;
	default:
	  abort();
	}
      fix_new_exp (fragP, fragP->fr_fix, size, &ex, true, reloc);
      //      fprintf (stderr, "fix_new_exp\n");
    }

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

  //  fprintf (stderr,"handle_it : num = %d needs_jt = %d is_tt = %d\n", num, needs_jt,
  //	   my66000_is_tt (this_iword));

  if (current_jt == NULL)
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
	      || is_end_of_line[(unsigned char) *endp])
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

      frag_var (rs_machine_dependent,
		8,
		1,
		RELAX_TT,
		ex.X_add_symbol,
		ex.X_add_number,
		(char *) current_jt);

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
