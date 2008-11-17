//
// test.c
// example uses of rb_parse_args
//
#include <ruby.h>
#include "rb_parse_args.h"

VALUE
parse_args_example1(int argc, VALUE *argv, VALUE self)
{
  int match;
  rb_arg_list args;
  match = rb_parse_args(argc, argv, "s|h,-|h", &args);
  /* a string and optionally a hash OR an IO and an optional hash */
  return INT2FIX(match);
}

VALUE
parse_args_example2(int argc, VALUE *argv, VALUE self)
{
  int match;
  rb_arg_list args;
  match = rb_parse_args(argc, argv, "kh,h,", &args);
  /* 1 = a class and a hash
   * 2 = a hash only
   * 3 = no arguments */
  return INT2FIX(match);
}

void
Init_rb_parse_args_test()
{
  VALUE mParseArgs = rb_define_module("ParseArgs");
  rb_define_singleton_method(mParseArgs, "example1", parse_args_example1, -1);
  rb_define_singleton_method(mParseArgs, "example2", parse_args_example2, -1);
}
