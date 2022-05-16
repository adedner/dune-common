# create Doxyfile.in and Doxyfile
dune_add_doxygen_target()

install(
  FILES
    Doxystyle
    doxygen-macros
  DESTINATION
    ${CMAKE_INSTALL_DATAROOTDIR}/dune-common/doc/doxygen
  )
