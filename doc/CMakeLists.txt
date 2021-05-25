add_subdirectory("doxygen")
add_subdirectory("buildsystem")
add_subdirectory("comm")
add_subdirectory("sphinx")
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dunecontrol.1
  DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
