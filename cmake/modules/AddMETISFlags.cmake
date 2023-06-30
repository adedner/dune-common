# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

# Defines the functions to use METIS
#
# .. cmake_function:: add_dune_metis_flags
#
#    .. cmake_param:: targets
#       :positional:
#       :single:
#       :required:
#
#       A list of targets to use METIS with.
#
include_guard(GLOBAL)

# register HAVE_METIS for config.h
set(HAVE_METIS ${METIS_FOUND})

# register METIS library as dune package
dune_register_package_flags(
  LIBRARIES           $<TARGET_NAME_IF_EXISTS:METIS::METIS>
  COMPILE_DEFINITIONS $<$<TARGET_EXISTS:METIS::METIS>:HAVE_METIS=1>)

# Add function to link targets against METIS library
function(add_dune_metis_flags _targets)
  foreach(_target ${_targets})
    target_link_libraries(${_target}      PUBLIC $<TARGET_NAME_IF_EXISTS:METIS::METIS>)
    target_compile_definitions(${_target} PUBLIC $<$<TARGET_EXISTS:METIS::METIS>:HAVE_METIS=1>)
  endforeach()
endfunction(add_dune_metis_flags _targets)
