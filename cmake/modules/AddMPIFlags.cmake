# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

# The DUNE way to compile MPI applications is to use the CXX
# compiler with MPI flags usually used for C. CXX bindings
# are deactivated to prevent ABI problems.
#
# .. cmake_function:: add_dune_mpi_flags
#
#    .. cmake_param:: targets
#       :single:
#       :required:
#       :positional:
#
#       The target list to add the MPI flags to.
#
include_guard(GLOBAL)

# text for feature summary
set_package_properties("MPI" PROPERTIES
  DESCRIPTION "Message Passing Interface library"
  PURPOSE "Parallel programming on multiple processors")

if(TARGET Dune::Imported::MPI_C)
  set(HAVE_MPI ${MPI_C_FOUND})
  dune_register_package_flags(
    LIBRARIES
      Dune::Imported::MPI_C)
endif()

# adds MPI flags to the targets
function(add_dune_mpi_flags)
  cmake_parse_arguments(ADD_MPI "SOURCE_ONLY;OBJECT" "" "" ${ARGN}) # ignored
  set(targets ${ADD_MPI_UNPARSED_ARGUMENTS})

  if(TARGET Dune::Imported::MPI_C)
    foreach(target ${targets})
      target_link_libraries(${target} PUBLIC Dune::Imported::MPI_C)
    endforeach(target)
  endif()
endfunction(add_dune_mpi_flags)
