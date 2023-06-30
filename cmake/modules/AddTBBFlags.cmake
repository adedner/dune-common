# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

# Defines the functions to use TBB
#
# .. cmake_function:: add_dune_tbb_flags
#
#    .. cmake_param:: targets
#       :positional:
#       :single:
#       :required:
#
#       A list of targets to use TBB with.
#
include_guard(GLOBAL)

# set variable for config.h
set(HAVE_TBB ${TBB_FOUND})

# perform DUNE-specific setup tasks
dune_register_package_flags(
  LIBRARIES           $<TARGET_NAME_IF_EXISTS:TBB::tbb>
  COMPILE_DEFINITIONS $<$<TARGET_EXISTS:TBB::tbb>:HAVE_TBB=1>
  )

# function for adding TBB flags to a list of targets
function(add_dune_tbb_flags _targets)
  foreach(_target ${_targets})
    target_link_libraries(${_target}      PUBLIC $<TARGET_NAME_IF_EXISTS:TBB::tbb>)
    target_compile_definitions(${_target} PUBLIC $<$<TARGET_EXISTS:TBB::tbb>:HAVE_TBB=1>)
  endforeach()
endfunction(add_dune_tbb_flags)
