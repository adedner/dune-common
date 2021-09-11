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
include(DunePathHelper)
include(DuneProject)
include(DuneSymlinkOrCopy)
include(DuneTestMacros)
include(DuneUtilities)
include(FeatureSummary)
include(OverloadCompilerFlags)


macro(dune_target_link_default_libraries _target)
  target_link_libraries(${_target} PUBLIC ${DUNE_LIBS})
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


# extract the required c++ standard from a given target
macro(dune_target_get_cxx_standard target var)
  get_target_property(${var} ${target} CXX_STANDARD)
  if(NOT ${var})
    # detect standard version from compile features
    get_target_property(_features ${target} INTERFACE_COMPILE_FEATURES)
    get_target_property(_features2 ${target} COMPILE_FEATURES)
    list(APPEND _features ${_features2})
    list(FILTER _features INCLUDE REGEX "cxx_std_[0-9]+")
    if(_features)
      # if multiple features are set, use the highest standard
      list(TRANSFORM _features REPLACE "cxx_std_([0-9]+)" "\\1")
      list(SORT _features ORDER DESCENDING)
      list(GET _features 0 ${var})
    endif()
    unset(_features)
    unset(_features2)
  endif()
  if(NOT ${var})
    # detect standard version from compile flags
    get_target_property(_options ${target} INTERFACE_COMPILE_OPTIONS)
    get_target_property(_options2 ${target} COMPILE_OPTIONS)
    list(APPEND _options ${_options2})
    list(FILTER _options INCLUDE REGEX "std=(c|gnu)\\+\\+[0-9a-z]+")
    if(_options)
      list(TRANSFORM _options REPLACE ".*std=(c|gnu)\\+\\+([0-9a-z]+).*" "\\2")
      set(_map_key "98" "0x" "11" "1y" "14" "1z" "17" "2a" "20" "2b" "23" "2c" "26")
      set(_map_val "98" "11" "11" "14" "14" "17" "17" "20" "20" "23" "23" "26" "26")
      # if multiple options are set, use the highest standard
      list(SORT _options ORDER DESCENDING)
      list(GET _options 0 ${var})
      list(FIND _map_key "${${var}}" _idx) # map the std flag to a valid standard version
      list(GET _map_val ${_idx} ${var})
    endif()
    unset(_options)
    unset(_options2)
    unset(_idx)
  endif()
endmacro(dune_target_get_cxx_standard)


# function to check whether c++ standards for all modules are the same
function(dune_check_compatible_cxx_standard)
  dune_target_get_cxx_standard(dunecommon cxx_standard_dunecommon)

  get_property(libraries GLOBAL PROPERTY DUNE_MODULE_LIBRARIES)
  if(cxx_standard_dunecommon)
    foreach(lib ${libraries})
      dune_target_get_cxx_standard(${lib} cxx_standard)
      if(cxx_standard AND NOT ${cxx_standard} EQUAL ${cxx_standard_dunecommon})
        message(WARNING "Library ${lib} requests cxx_std_${cxx_standard} whereas "
                        "dunecommon requires cxx_std_${cxx_standard_dunecommon}. "
                        "Mixing standards is undefined behaviour. Use a global "
                        "CMAKE_CXX_STANDARD instead.")
      endif()
      unset(cxx_standard)
    endforeach(lib)
  endif()
endfunction(dune_check_compatible_cxx_standard)
