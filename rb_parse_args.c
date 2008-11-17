//
// rb_parse_args.c
// the source code for rb_parse_args and derivatives
//
#include <ruby.h>
#include "rb_parse_args.h"

static ID s_to_a = 0, s_to_ary, s_to_i, s_to_int, s_to_s, s_to_str, s_read;

//
// a few macros to handle common type-checking tasks
// CHECK_ARG_TYPE only checks TYPE
// CHECK_ARG_NOT_NIL allows any non-nil object
// CHECK_ARG_COERCE duck types and checks TYPE
// CHECK_ARG_DATA ensures a specific type of T_DATA attached to a certain marking function
//
#define SET_ARG(o) args->a[n] = o, n = n + 1
#define CHECK_ARG(mac, t, d, cond, condarg) \
{ \
  if ((!x || m) && n < argc) \
  { \
    if (mac(argv[n]) == t) \
      SET_ARG(argv[n]); \
    else if (cond) \
      SET_ARG(condarg); \
    else \
    { \
      if (m) m = 0; \
      x = 1; \
    } \
  } \
  else if (m) \
    SET_ARG(d); \
  else \
    x = 1; \
}
#define CHECK_ARG_TYPE(t, d) CHECK_ARG(TYPE, t, d, 0, Qnil)
#define CHECK_ARG_NOT_NIL()  CHECK_ARG(NIL_P, 0, Qnil, 0, Qnil)
#define CHECK_ARG_COERCE(t, meth) \
  CHECK_ARG(TYPE, t, Qnil, rb_respond_to(argv[n], s_##meth), rb_funcall(argv[n], s_##meth, 0))
#define CHECK_ARG_RESPOND_TO(meth) \
  CHECK_ARG(NIL_P, 0, Qnil, rb_respond_to(argv[n], s_##meth), argv[n])
#define CHECK_ARG_DATA(func) CHECK_ARG(TYPE, T_DATA, Qnil, func(argv[n]), argv[n])

//
// rb_parse_args
// - a rb_scan_args replacement, designed to assist in typecasting, since
//   use of RSTRING_* and RARRAY_* macros are so common in Shoes.
//
// throws an exception if no match.
// returns 1 and up, the arg list matched.
// (args.n is set to the arg count, args.a is the args)
//
// rb_parse_args_return returns 0 if no match, rather than throwing an exception.
//
static int
rb_parse_args_p(unsigned char rais, int argc, const VALUE *argv, const char *fmt, rb_arg_list *args)
{
  int i = 1, m = 0, n = 0, nmin = 0, x = 0;
  const char *p = fmt;
  args->n = 0;

  if (s_to_a == 0)
  {
    s_to_a = rb_intern("to_a"); s_to_ary = rb_intern("to_ary");
    s_to_i = rb_intern("to_i"); s_to_int = rb_intern("to_int");
    s_to_s = rb_intern("to_s"); s_to_str = rb_intern("to_str");
    s_read = rb_intern("read");
  }

  do
  {
    if (*p == ',')
    {
      if ((x && !m) || n < argc) { i++; x = 0; if (nmin == 0 || nmin > n) { nmin = n; } n = 0; }
      else break;
    }
    else if (*p == '|')
    {
      if (!x) m = i;
    }
    else if (*p == 's') 
      CHECK_ARG_COERCE(T_STRING, to_str)
    else if (*p == 'S') 
      CHECK_ARG_COERCE(T_STRING, to_s)
    else if (*p == 'i')
      CHECK_ARG_COERCE(T_FIXNUM, to_int)
    else if (*p == 'I')
      CHECK_ARG_COERCE(T_FIXNUM, to_i)
    else if (*p == 'a')
      CHECK_ARG_COERCE(T_ARRAY, to_ary)
    else if (*p == 'A')
      CHECK_ARG_COERCE(T_ARRAY, to_a)
    else if (*p == 'k')
      CHECK_ARG_TYPE(T_CLASS, Qnil)
    else if (*p == 'h')
      CHECK_ARG_TYPE(T_HASH, Qnil)
    else if (*p == 'o')
      CHECK_ARG_NOT_NIL()
    else if (*p == '-')
      CHECK_ARG_RESPOND_TO(read)
    else if (*p == '&')
    {
      if (rb_block_given_p())
        SET_ARG(rb_block_proc());
      else
        SET_ARG(Qnil);
    }
    else break;
  }
  while (p++);

  if (!x && n >= argc)
    m = i;
  if (m)
    args->n = n;

  // printf("rb_parse_args(%s): %d %d (%d)\n", fmt, m, n, x);

  if (!m && rais)
  {
    if (argc < nmin)
      rb_raise(rb_eArgError, "wrong number of arguments (%d for %d)", argc, nmin);
    else
      rb_raise(rb_eArgError, "bad arguments");
  }

  return m;
}

int
rb_parse_args(int argc, const VALUE *argv, const char *fmt, rb_arg_list *args)
{
  return rb_parse_args_p(1, argc, argv, fmt, args);
}

int
rb_parse_args_return(int argc, const VALUE *argv, const char *fmt, rb_arg_list *args)
{
  return rb_parse_args_p(0, argc, argv, fmt, args);
}
