require 'cxx'
BuildDir = "BuildDir"

dependent_projects =  [ './project.rb', 'gmock/project.rb']
# tc = "clang"
tc = "gcc"
cxx(dependent_projects, BuildDir, tc, './') do
  unittest_flags = {
    :DEFINES => ['UNIT_TEST'],
    :FLAGS => ["-O0","-g3","-Wall","-std=c++11"]
  }
  Provider.modify_cpp_compiler(tc, unittest_flags)
end

# require 'cxx'
#
# cpp11_flags = {
#   :FLAGS => ["-std=c++11","-stdlib=libc++","-Wall"]
# }
#
# cxx(Dir['**/project.rb',"../gtest/project.rb"], 'out', "clang", './') do
#   # Provider.modify_cpp_compiler("clang", cpp11_flags)
#   # Provider["clang"][:LINKER][:FLAGS] = ["-std=c++11","-stdlib=libc++"]
# end

