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
enable_language(C)
dune_find_package(MPI 3.0 COMPONENTS C TARGET MPI::MPI_C)
include(AddMPIFlags)

# find library for Threading Building Blocks
dune_find_package(TBB TARGET TBB:tbb)
include(AddTBBFlags)

# find libraries for graph partitioning
dune_find_package(PTScotch TARGET PTScotch::Scotch)
include(AddPTScotchFlags)

dune_find_package(METIS TARGET METIS::METIS)
include(AddMETISFlags)

dune_find_package(ParMETIS 4.0 TARGET ParMETIS::ParMETIS)
include(AddParMETISFlags)

# try to find the Vc library
set(MINIMUM_VC_VERSION)
if((CMAKE_CXX_COMPILER_ID STREQUAL Clang) AND
    (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7))
  message("Raising minimum acceptable Vc version to 1.4.1 due to use of Clang 7 (or later), see https://gitlab.dune-project.org/core/dune-common/issues/132")
  set(MINIMUM_VC_VERSION 1.4.1)
endif()
find_package(Vc ${MINIMUM_VC_VERSION} NO_MODULE)
include(AddVcFlags)

# Run the python extension of the Dune cmake build system
include(DunePythonCommonMacros)
