#[=======================================================================[.rst:
DuneMacros
----------

Core DUNE module for CMake.

.. cmake:command:: target_link_libraries

  .. code-block:: cmake

    target_link_libraries(<target> [<scope>] <libs>...)

  Overwrite of CMake's ``target_link_libraries``. If no ``<scope>``
  keyword (like ``PUBLIC``, ``INTERFACE``, ``PRIVATE`` etc.) is given, ``PUBLIC``
  is added. This is to fix problems with cmake policy ``CMP0023``.


.. cmake:command:: dune_target_link_libraries

  .. code-block:: cmake

    dune_target_link_libraries(<target> <library>)

  Alias for ``target_link_libraries(<target> PUBLIC <library>)``


.. cmake:command:: add_dune_all_flags

  .. code-block:: cmake

    add_dune_all_flags(<target>)

  Add the flags of all registered packages to the given ``<target>``.
  This function is superseded by :command:`dune_target_enable_all_packages`.


.. cmake:command:: target_link_dune_default_libraries

  .. code-block:: cmake

    target_link_dune_default_libraries(<target>)

  Alias for ``target_link_libraries(<target> PUBLIC ${DUNE_DEFAULT_LIBS})``

#]=======================================================================]
include_guard(GLOBAL)


enable_language(C) # Enable C to skip CXX bindings for some tests.

# By default use -pthread flag. This option is set at the beginning to enforce it for
# find_package(Threads) everywhere
set(THREADS_PREFER_PTHREAD_FLAG TRUE CACHE BOOL "Prefer -pthread compiler and linker flag")

include(FeatureSummary)
include(DuneEnableAllPackages)
include(DuneTestMacros)
include(OverloadCompilerFlags)
include(DuneSymlinkOrCopy)
include(DunePathHelper)
include(DuneExecuteProcess)
include(DunePathHelper)
include(DuneProject)
include(DuneTestMacros)
include(DuneUtilities)


macro(target_link_libraries)
  # do nothing if not at least the two arguments target and scope are passed
  if(${ARGC} GREATER_EQUAL 2)
    set(SCOPE ${ARGV1})
    if(${SCOPE} MATCHES "^(PRIVATE|INTERFACE|PUBLIC|LINK_PRIVATE|LINK_PUBLIC|LINK_INTERFACE_LIBRARIES)$")
      _target_link_libraries(${ARGN})
    else()
      message(DEPRECATION "Calling target_link_libraries without the <scope> argument is deprecated.")
      set(ARGUMENTS ${ARGN})
      list(INSERT ARGUMENTS 1 PUBLIC)
      _target_link_libraries(${ARGUMENTS})
    endif()
  endif()
endmacro(target_link_libraries)


macro(target_link_dune_default_libraries _target)
  foreach(_lib ${DUNE_DEFAULT_LIBS})
    target_link_libraries(${_target} PUBLIC ${_lib})
  endforeach()
endmacro(target_link_dune_default_libraries)


function(dune_target_link_libraries basename libraries)
  target_link_libraries(${basename} PUBLIC ${libraries})
endfunction(dune_target_link_libraries basename libraries)


macro(add_dune_all_flags targets)
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
