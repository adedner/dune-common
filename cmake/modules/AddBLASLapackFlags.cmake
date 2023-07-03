# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

# Defines the functions to use BLAS/Lapack
#
# .. cmake_function:: add_dune_blas_lapack_flags
#
#    .. cmake_param:: targets
#       :positional:
#       :single:
#       :required:
#
#       A list of targets to use BLAS/Lapack with.
#
include_guard(GLOBAL)

include(FeatureSummary)
set_package_properties("BLAS" PROPERTIES
  DESCRIPTION "fast linear algebra routines")
set_package_properties("LAPACK" PROPERTIES
  DESCRIPTION "fast linear algebra routines")

# register HAVE_BLAS and HAVE_LAPACK for config.h
set(HAVE_BLAS ${BLAS_FOUND})
set(HAVE_LAPACK ${LAPACK_FOUND})

unset(LAPACK_COMPILE_OPTIONS_FLAGS)
  if(HAVE_LAPACK)
    set(LAPACK_COMPILE_OPTIONS_FLAGS "-DHAVE_LAPACK=1")
  endif()
  include(CMakePushCheckState)
  cmake_push_check_state()
  set(CMAKE_REQUIRED_LIBRARIES ${LAPACK_LIBRARIES})
  check_function_exists("dsyev_" LAPACK_NEEDS_UNDERLINE)
  cmake_pop_check_state()
  if(LAPACK_NEEDS_UNDERLINE)
  list(APPEND LAPACK_COMPILE_OPTIONS_FLAGS "-DLAPACK_NEEDS_UNDERLINE")
endif()

function(add_dune_blas_lapack_flags)
  dune_parse_arguments(ARG "" "" "" "SCOPE:PUBLIC:INTERFACE,PRIVATE,PUBLIC" ${ARGN})

  foreach(_target ${ARG_UNPARSED_ARGUMENTS})
    target_link_libraries(${_target} ${ARG_SCOPE}
      "$<$<BOOL:${LAPACK_FOUND}>:${LAPACK_LIBRARIES}>")
    target_compile_options(${_target} ${ARG_SCOPE}
      "$<$<BOOL:${LAPACK_FOUND}>:${LAPACK_COMPILE_OPTIONS_FLAGS}>")
    target_link_libraries(${_target} ${ARG_SCOPE}
      "$<$<BOOL:${BLAS_FOUND}>:${BLAS_LIBRARIES}>")
    target_compile_definitions(${_target} ${ARG_SCOPE}
      "$<$<BOOL:${BLAS_FOUND}>:HAVE_BLAS=1>")
  endforeach(_target)
endfunction(add_dune_blas_lapack_flags)

# register Lapack library as dune package
add_dune_blas_lapack_flags(${ProjectName}-all-packages INTERFACE)
