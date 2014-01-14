cxx_configuration do

  exe "sample01",
    :sources => FileList['tests/**/sample01.cpp'],
    :includes => ['include']

  exe "sample",
    :sources => FileList['tests/**/sample.cpp'],
    :includes => ['include']

end
