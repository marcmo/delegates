cxx_configuration do

  exe "sample",
    :sources => FileList['tests/**/sample.cpp'],
    :includes => ['include']

end
