//
// rb_parse_args.h
// the function definitions
//
typedef struct {
  int n; /* the number of arguments matched */
  VALUE a[10]; /* the array of matched and typecast arguments */
} rb_arg_list;

int rb_parse_args(int, const VALUE *, const char *, rb_arg_list *);
int rb_parse_args_return(int, const VALUE *, const char *, rb_arg_list *);
