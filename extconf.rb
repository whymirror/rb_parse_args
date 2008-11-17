require 'mkmf'

puts "** this extension is only a test of rb_parse_args!"
puts "-- you are supposed to copy rb_parse_args.c and rb_parse_args.h"
puts "-- into your C project to use them."

have_library("c")
create_makefile("rb_parse_args_test")
