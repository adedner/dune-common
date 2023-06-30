# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

# Defines the functions to use ParMETIS
#
# .. cmake_function:: add_dune_parmetis_flags
#
#    .. cmake_param:: targets
#       :positional:
#       :single:
#       :required:
#
#       A list of targets to use ParMETIS with.
#
include_guard(GLOBAL)

# set HAVE_PARMETIS for config.h
set(HAVE_PARMETIS ${ParMETIS_FOUND})

# register all ParMETIS related flags
dune_register_package_flags(
  LIBRARIES           $<TARGET_NAME_IF_EXISTS:ParMETIS::ParMETIS>
  COMPILE_DEFINITIONS $<$<TARGET_EXISTS:ParMETIS::ParMETIS>:HAVE_PARMETIS=1>)

# add function to link against the ParMETIS library
function(add_dune_parmetis_flags _targets)
  foreach(_target ${_targets})
    target_link_libraries(${_target}      PUBLIC $<TARGET_NAME_IF_EXISTS:ParMETIS::ParMETIS>)
    target_compile_definitions(${_target} PUBLIC $<$<TARGET_EXISTS:ParMETIS::ParMETIS>:HAVE_PARMETIS=1>)
  endforeach()
endfunction(add_dune_parmetis_flags)
