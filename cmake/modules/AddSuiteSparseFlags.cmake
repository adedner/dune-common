# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

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
include_guard(GLOBAL)

# set HAVE_SUITESPARSE for config.h
set(HAVE_SUITESPARSE ${SuiteSparse_FOUND})
set(HAVE_UMFPACK ${SuiteSparse_UMFPACK_FOUND})

# register all SuiteSparse related flags
dune_register_package_flags(
  COMPILE_DEFINITIONS   $<$<TARGET_EXISTS:SuiteSparse::SuiteSparse>:HAVE_SUITESPARSE=1>
  LIBRARIES             $<TARGET_NAME_IF_EXISTS:SuiteSparse::SuiteSparse>)

# Provide function to set target properties for linking to SuiteSparse
function(add_dune_suitesparse_flags _targets)
  foreach(_target ${_targets})
    target_link_libraries(${_target}      PUBLIC $<TARGET_NAME_IF_EXISTS:SuiteSparse::SuiteSparse>)
    target_compile_definitions(${_target} PUBLIC $<$<TARGET_EXISTS:SuiteSparse::SuiteSparse>:HAVE_SUITESPARSE=1>)
  endforeach()
endfunction(add_dune_suitesparse_flags)
