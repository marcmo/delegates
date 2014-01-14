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

