if(NOT DUNE_DISABLE_SIMD_TESTS)
  add_subdirectory(test)
endif()

if(NOT VC_FOUND)
  dune_exclude_dir_from_headercheck()
endif()

#install headers
install(FILES
  base.hh
  defaults.hh
  interface.hh
  io.hh
  loop.hh
  simd.hh
  standard.hh
  test.hh # may be used from dependent modules
  vc.hh
DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dune/common/simd)
