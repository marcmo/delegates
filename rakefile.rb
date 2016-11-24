require 'rake/clean'

BUILD_DIR="build"
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

file DELEGATETEST => "#{BUILD_DIR}/delegateTests.o" do
  sh "#{CC} -o #{DELEGATETEST} #{BUILD_DIR}/delegateTests.o"
end
file CLOSURETEST => "#{BUILD_DIR}/closureTests.o" do
  sh "#{CC} -o #{CLOSURETEST} #{BUILD_DIR}/closureTests.o"
end

rule ".o" => [->(f){locate_source(f)}, BUILD_DIR] do |t|
  sh "#{CC} #{CFLAGS} #{INC} -o#{t.name} #{t.source}"
end

def locate_source(o_file)
  SOURCE_FILES.detect { |f|
    detected = (f.ext('') == o_file.pathmap("%{^#{BUILD_DIR},#{SRC_DIR}}X"))
    detected
  }
end

desc "build gh-pages"
task :site do
  cd "site" do
    sh "hugo -d ../gh-pages"
  end
end

desc "build & deploy gh-pages to github"
task :deploy => :site do
  cd "gh-pages" do
    sh "git add ."
    sh "git commit -m 'updated docs'"
  end
  sh "git push origin gh-pages:gh-pages"
end
