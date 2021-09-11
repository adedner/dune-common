#[=======================================================================[.rst:
DuneMacros
----------

Core DUNE module for CMake.


.. cmake:command:: dune_target_link_default_libraries

  .. code-block:: cmake

    dune_target_link_default_libraries(<target>)

  Alias for ``target_link_libraries(<target> PUBLIC ${DUNE_LIBS})``


.. cmake:command:: dune_target_link_libraries

  .. code-block:: cmake

    dune_target_link_libraries(<target> <library>)

  Alias for ``target_link_libraries(<target> PUBLIC <library>)``


.. cmake:command:: dune_add_all_flags

  .. code-block:: cmake

    dune_add_all_flags(<target>)

  Add the flags of all registered packages to the given ``<target>``.
  This function is superseded by :command:`dune_target_enable_all_packages`.

#]=======================================================================]
include_guard(GLOBAL)

enable_language(C) # Enable C to skip CXX bindings for some tests.

# By default use -pthread flag. This option is set at the beginning to enforce it for
# find_package(Threads) everywhere
set(THREADS_PREFER_PTHREAD_FLAG TRUE CACHE BOOL "Prefer -pthread compiler and linker flag")

include(DuneAddLibrary)
include(DuneEnableAllPackages)
include(DuneExecuteProcess)
include(DuneModuleDependencies)
include(DuneModuleInformation)
include(DunePathHelper)
include(DuneProject)
include(DuneReplaceProperties)
include(DuneSymlinkOrCopy)
include(DuneTestMacros)
include(DuneUtilities)

# deprecated
function(dune_target_link_libraries _target _lib)
  message(DEPRECATION "dune_target_link_libraries is deprecated. Use "
    "'target_link_libraries(<target> PUBLIC <lib>)' instead.")
  target_link_libraries(${_target} PUBLIC ${_lib})
endfunction(dune_target_link_libraries)

# deprecated
macro(target_link_dune_default_libraries _target)
  message(DEPRECATION "target_link_dune_default_libraries is deprecated. Use "
    "'target_link_libraries(<target> PUBLIC \${DUNE_LIBS})' instead.")
  target_link_libraries(${_target} PUBLIC ${DUNE_LIBS})
endmacro(target_link_dune_default_libraries)

# deprecated
macro(add_dune_all_flags targets)
  message(DEPRECATION "add_dune_all_flags is deprecated. Use "
    "'dune_target_enable_all_packages(<targets>...)' instead.")

  get_property(incs GLOBAL PROPERTY ALL_PKG_INCS)
  get_property(defs GLOBAL PROPERTY ALL_PKG_DEFS)
  get_property(libs GLOBAL PROPERTY ALL_PKG_LIBS)
  get_property(opts GLOBAL PROPERTY ALL_PKG_OPTS)
  foreach(target ${targets})
    set_property(TARGET ${target} APPEND PROPERTY INCLUDE_DIRECTORIES ${incs})
    set_property(TARGET ${target} APPEND PROPERTY COMPILE_DEFINITIONS ${defs})
    target_link_libraries(${target} PUBLIC ${DUNE_LIBS} ${libs})
    target_compile_options(${target} PUBLIC ${opts})
  endforeach()
endmacro(add_dune_all_flags targets)


# find a package if not already found
macro(dune_find_package _name)
  cmake_parse_arguments(DUNE_PKG "" "TARGET" "" ${ARGN})

  # TODO: This does not yet work correctly
  if((NOT DEFINED ${_name}_FOUND OR ${_name}_FOUND) AND
     (NOT DUNE_PKG_TARGET OR NOT TARGET ${DUNE_PKG_TARGET}))
    find_package(${_name} ${DUNE_PKG_UNPARSED_ARGUMENTS})
  endif()
endmacro(dune_find_package)
