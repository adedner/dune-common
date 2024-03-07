# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

# Defines the functions to use MPFR
#
# .. cmake_function:: add_dune_mpfr_flags
#
#    .. cmake_param:: targets
#       :positional:
#       :single:
#       :required:
#
#       A list of targets to use MPFR with.
#
include_guard(GLOBAL)

# set HAVE_MPFR for the config.h file
set(HAVE_MPFR ${MPFR_FOUND})

# register all GMP related flags
if(MPFR_FOUND)
  dune_register_package_flags(
    LIBRARIES MPFR::mpreal
    COMPILE_DEFINITIONS "HAVE_MPFR=1"
  )
endif()

# add function to link against the MPFR library
function(add_dune_mpfr_flags _targets)
  if(MPFR_FOUND)
    foreach(_target ${_targets})
      target_link_libraries(${_target} PUBLIC MPFR::mpreal)
      target_compile_definitions(${_target} PUBLIC HAVE_MPFR=1)
    endforeach(_target ${_targets})
  endif(MPFR_FOUND)
endfunction(add_dune_mpfr_flags)
