cmake_minimum_required(VERSION 3.13)
project("dune-common" LANGUAGES C CXX)

# make sure our own modules are found
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

#include the dune macros
include(DuneMacros)

# start a dune project with information from dune.module
dune_project(dunecommon)

# minimal c++ standard required
target_compile_features(dunecommon PUBLIC cxx_std_17)

# link against LAPACK or BLAS
add_dune_blas_lapack_flags(dunecommon)

# link against Threading Building Blocks
add_dune_tbb_flags(dunecommon)

# Create the module library for dune-common
dune_add_library(dunecommon)

# Set properties to the dunecommon target
add_dune_blas_lapack_flags(dunecommon)
add_dune_tbb_flags(dunecommon)

# add subdirectories to execute CMakeLists.txt there
add_subdirectory(bin)
add_subdirectory(cmake)
add_subdirectory(doc)
add_subdirectory(dune)
add_subdirectory(lib)
add_subdirectory(share)

# if Python bindings are enabled, include necessary sub directories.
if(DUNE_ENABLE_PYTHONBINDINGS)
  add_subdirectory(python)
endif()

# finalize the dune project, e.g. generating config.h etc.
dune_finalize_project()
