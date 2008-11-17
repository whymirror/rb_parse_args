require './rb_parse_args_test.so'

puts "** example 1"
p ParseArgs.example1('test')
File.open('test.rb') do |f|
  p ParseArgs.example1(f)
end

puts
puts "** example 2"
p ParseArgs.example2(Hash, :background => "#FFFFFF")
p ParseArgs.example2(:width => 1.0)
p ParseArgs.example2()
