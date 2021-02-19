add_subdirectory(common)
add_subdirectory(test)
add_subdirectory(pybind11)

install(FILES embed.hh
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dune/python
        )
