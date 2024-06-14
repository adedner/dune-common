# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

include(DuneStreams)
dune_set_minimal_debug_level()

# search for lapack
find_package(LAPACK)
include(AddBLASLapackFlags)

find_package(GMP)
include(AddGMPFlags)
find_package(QuadMath)
include(AddQuadMathFlags)

# find program for image manipulation
find_package(Inkscape)
include(UseInkscape)

# find the threading library
find_package(Threads)
include(AddThreadsFlags)

# find the MPI library
if(NOT TARGET Dune::Imported::MPI_C)
  find_package(MPI 3.0 COMPONENTS C)
  include(AddMPIFlags)

  # create global imported target
  if(TARGET MPI::MPI_C)
    add_library(Dune::Imported::MPI_C IMPORTED INTERFACE GLOBAL)
    target_link_libraries(Dune::Imported::MPI_C INTERFACE MPI::MPI_C)
    target_compile_definitions(Dune::Imported::MPI_C INTERFACE
      HAVE_MPI=1 MPICH_SKIP_MPICXX=1 OMPI_SKIP_MPICXX=1 MPI_NO_CPPBIND=1 MPIPP_H _MPICC_H)
  endif()
endif()


# find library for Threading Building Blocks
find_package(TBB)
include(AddTBBFlags)

# find libraries for graph partitioning
find_package(PTScotch)
include(AddPTScotchFlags)
find_package(METIS)
include(AddMETISFlags)

if(NOT TARGET Dune::Imported::ParMETIS)
  find_package(ParMETIS 4.0)
  include(AddParMETISFlags)

  # create global imported target
  if(TARGET ParMETIS::ParMETIS)
    add_library(Dune::Imported::ParMETIS IMPORTED INTERFACE GLOBAL)
    target_link_libraries(Dune::Imported::ParMETIS INTERFACE ParMETIS::ParMETIS)
    if(HAVE_PARMETIS)
      target_compile_definitions(Dune::Imported::ParMETIS INTERFACE HAVE_PARMETIS=1)
    endif()
    if(HAVE_SCOTCH_METIS)
      target_compile_definitions(Dune::Imported::ParMETIS INTERFACE HAVE_PTSCOTCH_PARMETIS=1)
    endif()
  endif()
endif()

# find libraries for sparse matrix factorizations
if(NOT TARGET Dune::Imported::SuiteSparse)
  find_package(SuiteSparse OPTIONAL_COMPONENTS CHOLMOD LDL SPQR UMFPACK)
  include(AddSuiteSparseFlags)

  # create global imported target
  if(TARGET SuiteSparse::SuiteSparse)
    add_library(Dune::Imported::SuiteSparse IMPORTED INTERFACE GLOBAL)
    target_link_libraries(Dune::Imported::SuiteSparse INTERFACE SuiteSparse::SuiteSparse)
    foreach(_component ${SuiteSparse_FOUND_COMPONENTS})
      target_compile_definitions(Dune::Imported::SuiteSparse INTERFACE HAVE_SUITESPARSE_${_component}=1)
    endforeach(_component)
  endif()
endif()


# try to find the Vc library
set(MINIMUM_VC_VERSION)
if((CMAKE_CXX_COMPILER_ID STREQUAL Clang) AND
    (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7))
  message(STATUS "Raising minimum acceptable Vc version to 1.4.1 due to use of Clang 7 (or later), see https://gitlab.dune-project.org/core/dune-common/issues/132")
  set(MINIMUM_VC_VERSION 1.4.1)
endif()
find_package(Vc ${MINIMUM_VC_VERSION} NO_MODULE)
include(AddVcFlags)

# Run the python extension of the Dune cmake build system
include(DunePythonCommonMacros)
