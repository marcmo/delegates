require 'rake/clean'

BUILD_DIR="BuildDir"
SRC_DIR="tests"
SOURCE_FILES = FileList.new("#{SRC_DIR}/**/*.cpp")
CLEAN.include(BUILD_DIR)
CC="g++"
CFLAGS="-c -Wall -O0 -g3 -std=c++11"
INC=FileList["include"].pathmap("-I%p")
DELEGATETEST="#{BUILD_DIR}/delegateTest.exe"
CLOSURETEST="#{BUILD_DIR}/closureTest.exe"

directory BUILD_DIR

desc 'run all tests'
task :run => [DELEGATETEST,CLOSURETEST] do
  sh "./#{DELEGATETEST}"
  sh "./#{CLOSURETEST}"
end

task :default => [DELEGATETEST,CLOSURETEST]

file DELEGATETEST => "#{BUILD_DIR}/DelegateTests.o" do
	sh "#{CC} -o #{DELEGATETEST} #{BUILD_DIR}/DelegateTests.o"
end
file CLOSURETEST => "#{BUILD_DIR}/ClosureTests.o" do
	sh "#{CC} -o #{CLOSURETEST} #{BUILD_DIR}/ClosureTests.o"
end

rule ".o" => [->(f){locate_source(f)}, BUILD_DIR] do |t|
  sh "#{CC} #{CFLAGS} #{INC} -o#{t.name} #{t.source}"
end

def locate_source(o_file)
  SOURCE_FILES.detect { |f|
    f.ext('') == o_file.pathmap("%{^#{BUILD_DIR},#{SRC_DIR}}X")
  }
end
