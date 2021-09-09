#[=======================================================================[.rst:
DuneMacros
----------

Core DUNE module for CMake.


.. cmake:command:: dune_target_link_default_libraries

  .. code-block:: cmake

    dune_target_link_default_libraries(<target>)

  Alias for ``target_link_libraries(<target> PUBLIC ${DUNE_DEFAULT_LIBS})``


.. cmake:command:: dune_target_link_libraries

  .. code-block:: cmake

    dune_target_link_libraries(<target> <library>)

  Alias for ``target_link_libraries(<target> PUBLIC <library>)``


.. cmake:command:: add_dune_all_flags

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
include(DunePathHelper)
include(DuneProject)
include(DuneSymlinkOrCopy)
include(DuneTestMacros)
include(DuneUtilities)
include(FeatureSummary)
include(OverloadCompilerFlags)


macro(dune_target_link_default_libraries _target)
  foreach(_lib ${DUNE_DEFAULT_LIBS})
    target_link_libraries(${_target} PUBLIC ${_lib})
  endforeach()
endmacro(dune_target_link_default_libraries)

# deprecated
macro(target_link_dune_default_libraries _target)
  message(DEPRECATION "target_link_dune_default_libraries is deprecated. Use "
    "'dune_target_link_default_libraries' instead.")
    dune_target_link_default_libraries(${ARGV})
endmacro(target_link_dune_default_libraries)


macro(dune_target_link_libraries _target _lib)
  target_link_libraries(${_target} PUBLIC ${_lib})
endmacro(dune_target_link_libraries)


macro(dune_add_all_flags targets)
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
endmacro(dune_add_all_flags)

# deprecated
macro(add_dune_all_flags targets)
  message(DEPRECATION "add_dune_all_flags is deprecated. Use "
    "'dune_add_all_flags' instead.")
    dune_add_all_flags(${ARGV})
endmacro(add_dune_all_flags targets)