# Defines the functions to use SuiteSparse
#
# .. cmake_function:: dune_add_suitesparse_flags
#
#    .. cmake_param:: targets
#       :positional:
#       :single:
#       :required:
#
#       A list of targets to use SuiteSparse with.
#

# set HAVE_SUITESPARSE for config.h
set(HAVE_SUITESPARSE ${SuiteSparse_FOUND})
set(HAVE_UMFPACK ${SuiteSparse_UMFPACK_FOUND})

# register all SuiteSparse related flags
if(SuiteSparse_FOUND)
  dune_register_package_flags(
    COMPILE_DEFINITIONS "ENABLE_SUITESPARSE=1"
    LIBRARIES SuiteSparse::SuiteSparse)
endif()

# Provide function to set target properties for linking to SuiteSparse
function(dune_add_suitesparse_flags _targets)
  if(SuiteSparse_FOUND)
    foreach(_target ${_targets})
      target_link_libraries(${_target} PUBLIC SuiteSparse::SuiteSparse)
      target_compile_definitions(${_target} PUBLIC ENABLE_SUITESPARSE=1)
    endforeach(_target)
  endif()
endfunction(dune_add_suitesparse_flags)

macro(add_dune_suitesparse_flags _targets)
  message(DEPRECATION "add_dune_suitesparse_flags is deprecated. Use 'dune_add_suitesparse_flags' instead.")
  dune_add_gmp_flags(${_targets})
endmacro(add_dune_suitesparse_flags)