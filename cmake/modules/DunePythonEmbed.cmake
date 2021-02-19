# Provide a CMake function that takes care of embedded Python interpreter applications
# in the context of the Dune build system
#
# .. cmake_function:: dune_python_embedded_target
#
#    .. cmake_param:: TARGETS
#       :multi:
#
#       One or more targets that are embedding the Python interpreter.
#

function(dune_python_embedded_target)
  # Parse Arguments
  set(OPTION)
  set(SINGLE)
  set(MULTI TARGETS)
  include(CMakeParseArguments)
  cmake_parse_arguments(PYEMBED "${OPTION}" "${SINGLE}" "${MULTI}" ${ARGN})
  if(PYEMBED_UNPARSED_ARGUMENTS)
    message(WARNING "Unparsed arguments in dune_python_embedded_target: This often indicates typos!")
  endif()

  # Link these targets against the Python libraries
  foreach(target ${PYEMBED_TARGETS})
    target_link_libraries(${target} PRIVATE Python3::Python)
  endforeach()

  # Export the build-time Python environment directory through RPATH
  get_filename_component(env_python_dir ${DUNE_PYTHON_VIRTUALENV_EXECUTABLE} DIRECTORY)
  set_property(
    TARGET ${PYEMBED_TARGETS}
    APPEND
    PROPERTY BUILD_RPATH
    "${env_python_dir}"
  )

  # Export the install-time Python environment directory through RPATH
  get_filename_component(python_dir ${Python3_EXECUTABLE} DIRECTORY)
  set_property(
    TARGET ${PYEMBED_TARGETS}
    APPEND
    PROPERTY INSTALL_RPATH
    "${python_dir}"
  )
endfunction()
