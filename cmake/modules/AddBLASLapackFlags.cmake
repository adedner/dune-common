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

find_package(LAPACK)
set_property(GLOBAL APPEND PROPERTY DUNE_DEPENDENCY_REGISRTY "BLAS")
set_property(GLOBAL PROPERTY        DUNE_DEPENDENCY_REGISRTY_BLAS "include(AddBLASLapackFlags)")
set_property(GLOBAL APPEND PROPERTY DUNE_DEPENDENCY_REGISRTY "LAPACK")
set_property(GLOBAL PROPERTY        DUNE_DEPENDENCY_REGISRTY_LAPACK "include(AddBLASLapackFlags)")

# register HAVE_BLAS and HAVE_LAPACK for config.h
set(HAVE_BLAS ${BLAS_FOUND})
set(HAVE_LAPACK ${LAPACK_FOUND})

# register Lapack library as dune package
include(DuneEnableAllPackages)
if(HAVE_LAPACK)
  dune_register_package_flags(
    LIBRARIES "${LAPACK_LIBRARIES}"
    COMPILE_DEFINITIONS "ENABLE_LAPACK=1")

  include(CMakePushCheckState)
  cmake_push_check_state()
  set(CMAKE_REQUIRED_LIBRARIES ${LAPACK_LIBRARIES})
  check_function_exists("dsyev_" LAPACK_NEEDS_UNDERLINE)
  cmake_pop_check_state()
elseif(HAVE_BLAS)
  dune_register_package_flags(
    LIBRARIES "${BLAS_LIBRARIES}"
    COMPILE_DEFINITIONS "ENABLE_BLAS=1")
endif()

# add function to link against the BLAS/Lapack library
function(add_dune_blas_lapack_flags _targets)
  foreach(_target ${_targets})
    if(LAPACK_FOUND)
      target_link_libraries(${_target} PUBLIC ${LAPACK_LIBRARIES})
      target_compile_definitions(${_target} PUBLIC ENABLE_LAPACK=1)
    elseif(BLAS_FOUND)
      target_link_libraries(${_target} PUBLIC ${BLAS_LIBRARIES})
      target_compile_definitions(${_target} PUBLIC ENABLE_BLAS=1)
    endif()
  endforeach(_target)
endfunction(add_dune_blas_lapack_flags)
