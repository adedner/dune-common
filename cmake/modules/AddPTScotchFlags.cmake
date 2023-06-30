# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

# Defines the functions to use PTScotch
#
# .. cmake_function:: add_dune_ptscotch_flags
#
#    .. cmake_param:: targets
#       :positional:
#       :single:
#       :required:
#
#       A list of targets to use PTScotch with.
#
include_guard(GLOBAL)

# set HAVE_PTSCOTCH for config.h
set(HAVE_PTSCOTCH ${PTScotch_FOUND})

# register all PTScotch related flags
dune_register_package_flags(LIBRARIES $<TARGET_NAME_IF_EXISTS:PTScotch::Scotch>
  COMPILE_DEFINITIONS $<$<TARGET_EXISTS:PTScotch::Scotch>:HAVE_SCOTCH=1>)

  dune_register_package_flags(LIBRARIES $<TARGET_NAME_IF_EXISTS:PTScotch::PTScotch>
  COMPILE_DEFINITIONS $<$<TARGET_EXISTS:PTScotch::PTScotch>:HAVE_PTSCOTCH=1>)


function(add_dune_ptscotch_flags _targets)
  foreach(_target ${_targets})
    target_link_libraries(${_target}      PUBLIC $<TARGET_NAME_IF_EXISTS:PTScotch::Scotch>)
    target_compile_definitions(${_target} PUBLIC $<$<TARGET_EXISTS:PTScotch::Scotch>:HAVE_SCOTCH=1>)
  endforeach()
  foreach(_target ${_targets})
    target_link_libraries(${_target}      PUBLIC $<TARGET_NAME_IF_EXISTS:PTScotch::PTScotch>)
    target_compile_definitions(${_target} PUBLIC $<$<TARGET_EXISTS:PTScotch::PTScotch>:HAVE_PTSCOTCH=1>)
  endforeach()
endfunction(add_dune_ptscotch_flags)
