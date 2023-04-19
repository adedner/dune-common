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

find_package(TBB)

set_property(GLOBAL APPEND PROPERTY DUNE_DEPENDENCY_REGISRTY "TBB")
set_property(GLOBAL PROPERTY DUNE_DEPENDENCY_REGISRTY_TBB "include(AddTBBFlags)")

# set variable for config.h
set(HAVE_TBB ${TBB_FOUND})

# perform DUNE-specific setup tasks
include(DuneEnableAllPackages)
if (TBB_FOUND)
  dune_register_package_flags(
    LIBRARIES TBB::tbb
    )
endif()

# function for adding TBB flags to a list of targets
function(add_dune_tbb_flags _targets)
  if(TBB_FOUND)
    foreach(_target ${_targets})
      target_link_libraries(${_target} PUBLIC TBB::tbb)
    endforeach(_target)
  endif()
endfunction(add_dune_tbb_flags)
