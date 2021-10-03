# The DUNE way to compile MPI applications is to use the CXX
# compiler with MPI flags usually used for C. CXX bindings
# are deactivated to prevent ABI problems.
#
# .. cmake_function:: add_dune_mpi_flags
#
#    .. cmake_param:: targets
#       :single:
#       :required:
#       :positional:
#
#       The target list to add the MPI flags to.
#
include_guard(GLOBAL)

# text for feature summary
set_package_properties("MPI" PROPERTIES
  DESCRIPTION "Message Passing Interface library"
  PURPOSE "Parallel programming on multiple processors")

if(MPI_C_FOUND)
  set(HAVE_MPI ${MPI_C_FOUND} CACHE INTERNAL "")

  dune_register_package_flags(COMPILE_DEFINITIONS "ENABLE_MPI=1"
                              LIBRARIES MPI::MPI_C)
endif()

# adds MPI flags to the targets
function(add_dune_mpi_flags)
  cmake_parse_arguments(ADD_MPI "SOURCE_ONLY;OBJECT;INTERFACE" "" "" ${ARGN}) # ignored
  set(_targets ${ADD_MPI_UNPARSED_ARGUMENTS})

  if(MPI_C_FOUND)
    foreach(_target ${_targets})
      get_target_property(_type ${_target} TYPE)
      set(_scope "PUBLIC")
      if (${_type} STREQUAL "INTERFACE_LIBRARY")
        set(_scope "INTERFACE")
      endif()
      target_link_libraries(${_target} ${_scope} MPI::MPI_C)
      target_compile_definitions(${_target} ${_scope} "ENABLE_MPI=1")
    endforeach(_target)
  endif()
endfunction(add_dune_mpi_flags)
