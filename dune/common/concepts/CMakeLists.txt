install(FILES
  collection.hh
  linearmap.hh
  matrix.hh
  multitype.hh
  vector.hh
  vectorspace.hh
DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dune/common/concepts)

add_subdirectory(test)