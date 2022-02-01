#[=======================================================================[.rst:
DuneModuleDependencies
----------------------

Macros to extract dependencies between Dune modules by inspecting the
``dune.module`` files.

.. cmake:command:: dune_create_dependency_tree

  .. code-block:: cmake

    dune_create_dependency_tree()

  Create a tree of dune module dependencies by inspecting the ``dune.module``
  files recursively.


.. cmake:command:: dune_process_dependency_macros

  .. code-block:: cmake

    dune_process_dependency_macros()

  Include the corresponding ``Dune<module>Macros.cmake`` file of all
  dependencies if this file exists.

#]=======================================================================]
include_guard(GLOBAL)

include(DuneEnableAllPackages)
include(DuneModuleInformation)
include(DuneUtilities)

macro(dune_create_dependency_tree)
  # collect direct (and transitive) dependencies
  set(ALL_DEPENDENCIES)
  if(${ProjectName}_DEPENDS_MODULE OR ${ProjectName}_SUGGESTS_MODULE)
    set(ALL_DEPENDENCIES ${${ProjectName}_DEPENDS_MODULE} ${${ProjectName}_SUGGESTS_MODULE})
    dune_create_dependency_leafs(
      "${${ProjectName}_DEPENDS_MODULE}"
      "${${ProjectName}_DEPENDS_VERSION}"
      "${${ProjectName}_SUGGESTS_MODULE}"
      "${${ProjectName}_SUGGESTS_VERSION}")
  endif()

  # process list of dependencies
  if(ALL_DEPENDENCIES)
    list(REVERSE ALL_DEPENDENCIES)
    list(REMOVE_DUPLICATES ALL_DEPENDENCIES)
  endif()
endmacro(dune_create_dependency_tree)


# TODO: do not include <Module>Macros.cmake but use automatic <module>-config.cmake files
macro(dune_process_dependency_macros)
  foreach(_mod ${ALL_DEPENDENCIES} ${ProjectName})
    if(NOT ${_mod}_PROCESSED)
      # module not processed yet
      set(${_mod}_PROCESSED ${_mod})
      # Search for a cmake files containing tests and directives
      # specific to this module
      dune_module_to_macro(_cmake_mod_name "${_mod}")
      set(_macro "${_cmake_mod_name}Macros")
      set(_mod_cmake _mod_cmake-NOTFOUND) # Prevent false positives due to caching
      find_file(_mod_cmake
        NAMES "${_macro}.cmake"
        PATHS ${CMAKE_MODULE_PATH}
        NO_DEFAULT_PATH
        NO_CMAKE_FIND_ROOT_PATH)
      if(_mod_cmake)
        message(STATUS "Performing tests for ${_mod} (from ${_mod_cmake})")
        include(${_mod_cmake})
      else()
        # only print output for found module dependencies
        # otherwise it's obvious that we cannot find a ModuleMacros.cmake file
        if (${${_mod}_FOUND}})
          message(STATUS "No module specific tests for module '${_mod}' ('${_macro}.cmake' not found)")
        endif()
      endif()
      if(${_mod}_LIBRARIES)
        foreach(_lib ${${_mod}_LIBRARIES})
          list(INSERT DUNE_LIBS 0 "${_lib}")
        endforeach()
      endif()
    endif()
  endforeach()
endmacro(dune_process_dependency_macros)


# ------------------------------------------------------------------------
# Internal macros
# ------------------------------------------------------------------------

macro(dune_find_module module)
  cmake_parse_arguments(DUNE_FIND "REQUIRED" "VERSION" "" ${ARGN})
  if(DUNE_FIND_REQUIRED)
    set(required REQUIRED)
    set_package_properties(${module} PROPERTIES TYPE REQUIRED)
    set(_warning_level "FATAL_ERROR")
  else()
    unset(required)
    set(_warning_level "WARNING")
    set_package_properties(${module} PROPERTIES TYPE OPTIONAL)
  endif()

  if(DUNE_FIND_VERSION MATCHES "(>=|=|<=).*")
    string(REGEX REPLACE "(>=|=|<=)(.*)" "\\1" DUNE_FIND_VERSION_OP ${DUNE_FIND_VERSION})
    string(REGEX REPLACE "(>=|=|<=)(.*)" "\\2" DUNE_FIND_VERSION_NUMBER ${DUNE_FIND_VERSION})
    string(STRIP ${DUNE_FIND_VERSION_NUMBER} DUNE_FIND_VERSION_NUMBER)
    extract_major_minor_version("${DUNE_FIND_VERSION_NUMBER}" DUNE_FIND_VERSION)
    set(DUNE_FIND_VERSION_STRING
      "${DUNE_FIND_VERSION_MAJOR}.${DUNE_FIND_VERSION_MINOR}.${DUNE_FIND_VERSION_REVISION}")
  else()
    set(DUNE_FIND_VERSION_STRING "0.0.0")
  endif()

  # search for dune module only if not found or provided otherwise
  if(NOT ${module}_FOUND)
    if(NOT (${module}_DIR OR ${module}_ROOT OR "${CMAKE_PREFIX_PATH}" MATCHES ".*${module}.*"))
      string(REPLACE  ${ProjectName} ${module} ${module}_DIR ${PROJECT_BINARY_DIR})
    endif()
    find_package(${module} ${required} NO_CMAKE_PACKAGE_REGISTRY)
  endif()

  # check whether dependency matches version requirement
  if(${module}_FOUND)
    set(DUNE_FOUND_VERSION_STRING ${${module}_VERSION}) # version string provided by find_package

    unset(module_version_wrong)
    if(DUNE_FIND_VERSION_OP MATCHES ">=")
      if(NOT (DUNE_FOUND_VERSION_STRING VERSION_GREATER_EQUAL DUNE_FIND_VERSION_STRING))
        set(module_version_wrong 1)
      endif()
    elseif(DUNE_FIND_VERSION_OP MATCHES "<=")
      if(NOT (DUNE_FOUND_VERSION_STRING VERSION_LESS_EQUAL DUNE_FIND_VERSION_STRING))
        set(module_version_wrong 1)
      endif()
    elseif(DUNE_FIND_VERSION_OP MATCHES "=" AND
        NOT (DUNE_FOUND_VERSION_STRING VERSION_EQUAL DUNE_FIND_VERSION_STRING))
      set(module_version_wrong 1)
    endif()

    if(module_version_wrong)
      message(${_warning_level} "Could not find requested version of module ${module}. "
        "Requested version was ${DUNE_FIND_VERSION}, found version is ${DUNE_FOUND_VERSION_STRING}")
      set(${module}_FOUND OFF)
    else()
      dune_module_to_uppercase(DUNE_MOD_NAME_UPPERCASE ${module})

      # set module version
      set(${DUNE_MOD_NAME_UPPERCASE}_VERSION          "${${module}_VERSION}")
      set(${DUNE_MOD_NAME_UPPERCASE}_VERSION_MAJOR    "${${module}_VERSION_MAJOR}")
      set(${DUNE_MOD_NAME_UPPERCASE}_VERSION_MINOR    "${${module}_VERSION_MINOR}")
      set(${DUNE_MOD_NAME_UPPERCASE}_VERSION_REVISION "${${module}_VERSION_PATCH}")
    endif()
  endif()

  if(NOT ${module}_FOUND AND required)
    message(FATAL_ERROR "Could not find required module ${module}.")
  endif()
  set(DUNE_${module}_FOUND ${${module}_FOUND})
endmacro(dune_find_module module)


macro(dune_process_dependency_leafs modules versions is_required next_level_deps
    next_level_sugs)
  # modules, and versions are not real variables, make them one
  set(mmodules ${modules})
  set(mversions ${versions})
  list(LENGTH mmodules mlength)
  list(LENGTH mversions vlength)
  if(NOT mlength EQUAL vlength)
    message(STATUS "mmodules=${mmodules} modules=${modules}")
    message(STATUS "mversions=${mversions} versions=${mversions}")
    message(FATAL_ERROR "List of modules and versions do not have the same length!")
  endif()
  if(mlength GREATER 0)
    math(EXPR length "${mlength}-1")
    foreach(i RANGE 0 ${length})
      list(GET mmodules ${i} _mod)
      list(GET mversions ${i} _ver)
      dune_find_module(${_mod} ${is_required} VERSION "${_ver}")
      set(${_mod}_SEARCHED ON)
      if(NOT "${is_required}" STREQUAL "")
        set(${_mod}_REQUIRED ON)
        set(${next_level_deps} ${${_mod}_DEPENDS} ${${next_level_deps}})
      else(NOT "${is_required}" STREQUAL "")
        set(${next_level_sugs} ${${_mod}_DEPENDS} ${${next_level_sugs}})
      endif()
      set(${next_level_sugs} ${${_mod}_SUGGESTS} ${${next_level_sugs}})
    endforeach()
  endif()
  if(${next_level_sugs})
    list(REMOVE_DUPLICATES ${next_level_sugs})
  endif()
  if(${next_level_deps})
    list(REMOVE_DUPLICATES ${next_level_deps})
  endif()
endmacro(dune_process_dependency_leafs)


macro(dune_create_dependency_leafs depends depends_versions suggests suggests_versions)
  set(deps "")
  set(sugs "")
  #Process dependencies
  if(NOT "${depends}" STREQUAL "")
    dune_process_dependency_leafs("${depends}" "${depends_versions}" REQUIRED deps sugs)
  endif()
  # Process suggestions
  if(NOT "${suggests}" STREQUAL "")
    dune_process_dependency_leafs("${suggests}" "${suggests_versions}" "" deps sugs)
  endif()
  split_module_version("${deps}" next_mod_depends next_depends_versions)
  split_module_version("${sugs}" next_mod_suggests next_suggests_versions)
  set(ALL_DEPENDENCIES ${ALL_DEPENDENCIES} ${next_mod_depends} ${next_mod_suggests})
  # Move to next level
  if(next_mod_suggests OR next_mod_depends)
    dune_create_dependency_leafs("${next_mod_depends}" "${next_depends_versions}"
      "${next_mod_suggests}" "${next_suggests_versions}")
  endif()
endmacro(dune_create_dependency_leafs)
