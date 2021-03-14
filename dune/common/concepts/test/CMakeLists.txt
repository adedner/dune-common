dune_add_test(SOURCES test-vectorconcept.cc
  LINK_LIBRARIES dunecommon
  CMAKE_GUARD HAVE_CXX_CONCEPTS
  LABELS quick)
