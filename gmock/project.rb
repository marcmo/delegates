cxx_configuration do
  source_lib "gmock",
    :sources => FileList['fused-src/**/*.cc'],
    :includes => ['fused-src']

  source_lib "gmock-source-only",
    :sources => FileList['fused-src/gmock-gtest-all.cc'],
    :includes => ['fused-src']
end
