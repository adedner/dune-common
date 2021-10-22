# this option enables the build of Python bindings for DUNE modules
option(DUNE_ENABLE_PYTHONBINDINGS "Enable Python bindings for DUNE" OFF)

if( DUNE_ENABLE_PYTHONBINDINGS )
  if(NOT Python3_Interpreter_FOUND)
    message(FATAL_ERROR "Python bindings require a Python 3 interpreter")
  endif()
  if(NOT Python3_INCLUDE_DIRS)
    message(FATAL_ERROR "Found a Python interpreter but the Python bindings also requires the Python libraries (a package named like python-dev package or python3-devel)")
  endif()

  include_directories("${Python3_INCLUDE_DIRS}")

  function(add_python_targets base)
    include(DuneSymlinkOrCopy)
    if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
      message(WARNING "Source and binary dir are the same, skipping symlink!")
    else()
      foreach(file ${ARGN})
        dune_symlink_to_source_files(FILES ${file}.py)
      endforeach()
    endif()
  endfunction()

  function(make_pybind11_docu)
    string(REPLACE "/" "_" targetname "docheader_${CMAKE_CURRENT_SOURCE_DIR}.hh")
    # set(outputheader "${CMAKE_CURRENT_BINARY_DIR}/docheader.hh")
    # need to generate header in source directory since it will need to be
    # found in a downstream module and the build dir is not part of the include list.
    set(outputheader "${CMAKE_CURRENT_SOURCE_DIR}/docheader.hh")
    set(files ${ARGN})
    set(alldefs "-std=c++17") # TODO: replace with actuall CXXFLAGS
    set(incdirs "-I${CMAKE_SOURCE_DIR}")
    foreach(mod ${ALL_DEPENDENCIES})
      message("${mod}")
      list(APPEND incdirs "-I${${mod}_INCLUDE_DIRS}")
    endforeach()
    # in the newer version for python bindings pybind11_mkdoc can be installed into
    # the venv - so this just has to work for now
    if(dune-common_INCLUDE_DIRS)
      set(mkdocPath ${dune-common_INCLUDE_DIRS}/dune/python/pybind11_mkdoc)
    else()
      set(mkdocPath ${CMAKE_SOURCE_DIR}/dune/python/pybind11_mkdoc)
    endif()
    add_custom_command(DEPENDS ${ARGN}
                       OUTPUT ${outputheader}
                       COMMAND PYTHONPATH=${mkdocPath} ${Python3_EXECUTABLE} -m pybind11_mkdoc
                          -o ${outputheader} ${incdirs} ${alldefs} ${files}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                      )
    add_custom_target(${targetname}
                      ALL
                      DEPENDS ${outputheader}
                      COMMENT "Generate documentation header for pybind11 bindings"
                     )
  endfunction()

  include(DuneAddPybind11Module)

  # Add a custom command that triggers the configuration of dune-py
  add_custom_command(TARGET install_python POST_BUILD
                     COMMAND ${Python3_EXECUTABLE} -m dune configure
                     )

endif()
