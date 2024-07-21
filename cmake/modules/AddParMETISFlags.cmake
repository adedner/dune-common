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
if(TARGET Dune::Imported::ParMETIS)
  dune_register_package_flags(LIBRARIES Dune::Imported::ParMETIS)
endif()

# add function to link against the ParMETIS library
function(add_dune_parmetis_flags _targets)
  if(TARGET Dune::Imported::ParMETIS)
    foreach(_target ${_targets})
      target_link_libraries(${_target} PUBLIC Dune::Imported::ParMETIS)
    endforeach(_target)
  endif()
endfunction(add_dune_parmetis_flags)
