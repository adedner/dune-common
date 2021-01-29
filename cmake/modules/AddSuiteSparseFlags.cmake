# Defines the functions to use SuiteSparse
#
# .. cmake_function:: add_dune_suitesparse_flags
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
function(add_dune_suitesparse_flags _targets)
  if(SuiteSparse_FOUND)
    foreach(_target ${_targets})
      target_link_libraries(${_target} PUBLIC SuiteSparse::SuiteSparse)
      target_compile_definitions(${_target} PUBLIC HAVE_SUITESPARSE HAVE_UMFPACK
        $<$<BOOL:${HAVE_SUITESPARSE_AMD}>:HAVE_SUITESPARSE_AMD>
        $<$<BOOL:${HAVE_SUITESPARSE_BTF}>:HAVE_SUITESPARSE_BTF>
        $<$<BOOL:${HAVE_SUITESPARSE_CAMD}>:HAVE_SUITESPARSE_CAMD>
        $<$<BOOL:${HAVE_SUITESPARSE_CCOLAMD}>:HAVE_SUITESPARSE_CCOLAMD>
        $<$<BOOL:${HAVE_SUITESPARSE_CHOLMOD}>:HAVE_SUITESPARSE_CHOLMOD>
        $<$<BOOL:${HAVE_SUITESPARSE_COLAMD}>:HAVE_SUITESPARSE_COLAMD>
        $<$<BOOL:${HAVE_SUITESPARSE_CXSPARSE}>:HAVE_SUITESPARSE_CXSPARSE>
        $<$<BOOL:${HAVE_SUITESPARSE_KLU}>:HAVE_SUITESPARSE_KLU>
        $<$<BOOL:${HAVE_SUITESPARSE_LDL}>:HAVE_SUITESPARSE_LDL>
        $<$<BOOL:${HAVE_SUITESPARSE_RBIO}>:HAVE_SUITESPARSE_RBIO>
        $<$<BOOL:${HAVE_SUITESPARSE_SPQR}>:HAVE_SUITESPARSE_SPQR>
        $<$<BOOL:${HAVE_SUITESPARSE_UMFPACK}>:HAVE_SUITESPARSE_UMFPACK>
      )
    endforeach(_target)
  endif()
endfunction(add_dune_suitesparse_flags)
