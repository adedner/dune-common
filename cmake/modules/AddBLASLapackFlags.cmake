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

unset(LAPACK_COMPILE_FLAGS)
if(HAVE_LAPACK)
  include(CMakePushCheckState)
  cmake_push_check_state()
  set(CMAKE_REQUIRED_LIBRARIES ${LAPACK_LIBRARIES})
  check_function_exists("dsyev_" LAPACK_NEEDS_UNDERLINE)
  cmake_pop_check_state()
  set(LAPACK_COMPILE_FLAGS "-DLAPACK_COMPILE_FLAGS;-DHAVE_LAPACK=1")
endif()

# register Lapack library as dune package
dune_register_package_flags(
  LIBRARIES           $<$<BOOL:${LAPACK_FOUND}>:${LAPACK_LIBRARIES}>
  COMPILE_DEFINITIONS $<$<BOOL:${LAPACK_FOUND}>:${LAPACK_COMPILE_FLAGS}>)

dune_register_package_flags(
  LIBRARIES           $<$<BOOL:${BLAS_FOUND}>:${BLAS_LIBRARIES}>
  COMPILE_DEFINITIONS $<$<BOOL:${BLAS_FOUND}>:HAVE_BLAS=1>)

# add function to link against the BLAS/Lapack library
function(add_dune_blas_lapack_flags _targets)
  foreach(_target ${_targets})
    target_link_libraries(${_target}      PUBLIC $<$<BOOL:${LAPACK_FOUND}>:${LAPACK_LIBRARIES}>)
    target_compile_definitions(${_target} PUBLIC $<$<BOOL:${LAPACK_FOUND}>:${LAPACK_COMPILE_FLAGS}>)

    target_link_libraries(${_target}      PUBLIC $<$<BOOL:${BLAS_FOUND}>:${BLAS_LIBRARIES}>)
    target_compile_definitions(${_target} PUBLIC $<$<BOOL:${BLAS_FOUND}>:HAVE_BLAS=1>)
  endforeach()
endfunction(add_dune_blas_lapack_flags)
