# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

#[=======================================================================[.rst:
DuneAddLibrary
--------------

Add a library to a Dune module.

.. cmake:command:: dune_add_library

  Create a new library target. There are three different interfaces following
  the standard cmake signatures.

  .. code-block:: cmake

    dune_add_library(<basename> [STATIC|SHARED|MODULE]
      [SOURCES <sources...>]
      [LINK_LIBRARIES <targets>...]
      [COMPILE_OPTIONS "<flags>;..."]
      [OUTPUT_NAME <libname>]
      [EXPORT_NAME <exportname>]
      [NO_EXPORT]
      [NO_MODULE_LIBRARY]
    )

    Create a new library target with ``<basename>`` for the library name. On Unix
    this created ``lib<libname>.so`` or ``lib<libname>.a``. The target properties
    are automatically filled with the given (optional) arguments.

    A dune library is (by default) exported into the ``<export-set>`` given by the
    global name ``${ProjectName}-targets`` if the parameter ``NO_EXPORT`` is not
    given. This ``<export-set>`` is automatically installed and exported in the
    ``dune_finalize_project()`` function.

  ``SOURCES``
    The source files from which to build the library.

  ``LINK_LIBRARIES``
    A list of dependency the libraries is explicitly linked against.

  ``COMPILE_OPTIONS``
    Any additional compile flags for building the library.

  ``OUTPUT_NAME``
    Name of the library file, e.g. ``lib<libname>.so`` or ``lib<libname>.a``.

  ``EXPORT_NAME``
    Name of the exported target to be used when linking against the library.
    We recommend to choose a scoped export name with a camel title case matching your
    library name (e.g., Dune::Common, Dune::ISTL, Dune::MultiDomainGrid, ...)

  ``NO_EXPORT``
    If omitted the library is exported for usage in other modules.

  ``NO_MODULE_LIBRARY``
    If omitted the library is added to the global property ``<module>_LIBRARIES``
    and used for internal module configuration. If ``NO_EXPORT`` is omitted, the library
    will additionally be added to the global property ``<module>_INTERFACE_LIBRARIES``.


  .. code-block:: cmake

    dune_add_library(<basename> INTERFACE
      [LINK_LIBRARIES <targets>...]
      [COMPILE_OPTIONS "<flags>;..."]
      [EXPORT_NAME <exportname>]
      [NO_EXPORT]
      [NO_MODULE_LIBRARY]
    )

    Create an interface library target with ``<basename>`` for the library name.
    An interface target does not contain any sources but my contain flags and
    dependencies.

  ``LINK_LIBRARIES``
    A list of dependency the libraries is explicitly linked against.

  ``COMPILE_OPTIONS``
    Any additional compile flags for building the library.

  ``EXPORT_NAME``
    Name of the exported target to be used when linking against the library.
    We recommend to choose a scoped export name with a camel title case matching your
    library name (e.g., Dune::Common, Dune::ISTL, Dune::MultiDomainGrid, ...)

  ``NO_EXPORT``
    If omitted the library is exported for usage in other modules.

  ``NO_MODULE_LIBRARY``
    If omitted the library is added to the global property ``<module>_LIBRARIES``
    and used for internal module configuration. If ``NO_EXPORT`` is omitted, the library
    will additionally be added to the global property ``<module>_INTERFACE_LIBRARIES``.


  .. code-block:: cmake

    dune_add_library(<basename> OBJECT
      [SOURCES <sources...>]
      [LINK_LIBRARIES <targets>...]
      [COMPILE_OPTIONS "<flags>;..."]
    )

    Create an object library target ``<basename>`` to collect multiple sources
    to be added to a library target later. Note, this utility is deprecated.
    Create a regular library target in a parent scope and add the sources
    directly using ``target_sources(<target> PRIVATE <sources>...)`` instead.

  ``SOURCES``
    The source files from which to build the library.

  ``LINK_LIBRARIES``
    A list of dependency the libraries is explicitly linked against.

  ``COMPILE_OPTIONS``
    Any additional compile flags for building the library.

#]=======================================================================]
include_guard(GLOBAL)


# Public interface for creating a module library
function(dune_add_library _name)
  cmake_parse_arguments(ARG "OBJECT;INTERFACE" "" "" ${ARGN})

  if(ARG_OBJECT)
    dune_add_library_object(${_name} ${ARGN})
  elseif(ARG_INTERFACE)
    dune_add_library_interface(${_name} ${ARGN})
  else()
    dune_add_library_normal(${_name} ${ARGN})
  endif()
endfunction(dune_add_library)


# ------------------------------------------------------------------------
# Internal macros and functions
# ------------------------------------------------------------------------


# Create a regular library target
function(dune_add_library_normal _name)
  cmake_parse_arguments(ARG
    "NO_EXPORT;NO_MODULE_LIBRARY;STATIC;SHARED;MODULE"
    "COMPILE_FLAGS;COMPILE_OPTIONS;OUTPUT_NAME;EXPORT_NAME"
    "ADD_LIBS;LINK_LIBRARIES;SOURCES" ${ARGN})
  list(APPEND ARG_SOURCES ${ARG_UNPARSED_ARGUMENTS})
  dune_expand_object_libraries(ARG_SOURCES ARG_ADD_LIBS ARG_COMPILE_FLAGS)
  list(APPEND ARG_LINK_LIBRARIES ${ARG_ADD_LIBS})
  list(APPEND ARG_COMPILE_OPTIONS ${ARG_COMPILE_FLAGS})

  set(_type)
  if(ARG_STATIC)
    set(_type "STATIC")
  elseif(ARG_SHARED)
    set(_type "SHARED")
  elseif(ARG_MODULE)
    set(_type "MODULE")
  endif()

  # Create the library target
  add_library(${_name} ${_type} ${ARG_SOURCES})

  # Link with specified libraries from parameter ADD_LIBS
  target_link_libraries(${_name} PUBLIC "${ARG_LINK_LIBRARIES}")

  # Set target options from COMPILE_FLAGS
  target_compile_options(${_name} PUBLIC "${ARG_COMPILE_OPTIONS}")

  # Build library in ${PROJECT_BINARY_DIR}/lib
  set_target_properties(${_name} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

  # Set an output name for the created library file
  if(ARG_OUTPUT_NAME)
    set_target_properties(${_name} PROPERTIES OUTPUT_NAME ${ARG_OUTPUT_NAME})
  endif()

  if(NOT ARG_NO_EXPORT)
    if(NOT ARG_EXPORT_NAME)
      message(DEPRECATION
        "The function dune_add_library(<lib> ...) now requires to provide NO_EXPORT or EXPORT_NAME. "
        "We recommend to choose a scoped export name with a camel title case matching your library name "
        "(e.g., Dune::Common, Dune::ISTL, OPM::Grid, ...)\n"
        " * Calls to `dune_add_library(<lib> ...)` without export specification will be supported until Dune 2.11\n"
        " * Consumption of unscoped targets `<lib>` will be supported until Dune 2.12")
      set(ARG_EXPORT_NAME __dune_impl_${_name})
    endif()

    set(${ProjectName}_EXPORT_SET ${ProjectName}-targets CACHE INTERNAL "")

    # Install targets to use the libraries in other modules.
    add_library(${ARG_EXPORT_NAME} ALIAS ${_name})
    set_target_properties(${_name} PROPERTIES EXPORT_NAME ${ARG_EXPORT_NAME})
    install(TARGETS ${_name}
      EXPORT ${${ProjectName}_EXPORT_SET} DESTINATION ${CMAKE_INSTALL_LIBDIR})

    # Install (unaliased) targets to use the libraries in other modules.
    # NOTE: Remove when compatibility with 2.9 is not needed anymore (e.g., 2.13)
    add_library(_dune_unaliased_${_name} INTERFACE)
    target_link_libraries(_dune_unaliased_${_name} INTERFACE ${ARG_EXPORT_NAME})
    set_target_properties(_dune_unaliased_${_name} PROPERTIES EXPORT_NAME ${_name})
    install(TARGETS _dune_unaliased_${_name}
      EXPORT ${${ProjectName}_EXPORT_SET} DESTINATION ${CMAKE_INSTALL_LIBDIR})

    # Register target as an exported library
    if(NOT ARG_NO_MODULE_LIBRARY)
      set_property(GLOBAL APPEND PROPERTY ${ProjectName}_INTERFACE_LIBRARIES ${ARG_EXPORT_NAME})
    endif()
  endif()

  # Register library in global property <module>LIBRARIES
  if(NOT ARG_NO_MODULE_LIBRARY)
    set_property(GLOBAL APPEND PROPERTY ${ProjectName}_LIBRARIES ${name})
  endif()
endfunction(dune_add_library_normal)


# Create an interface target that does not compile any sources
function(dune_add_library_interface _name)
  cmake_parse_arguments(ARG
    "NO_EXPORT;NO_MODULE_LIBRARY;INTERFACE"
    "COMPILE_FLAGS;COMPILE_OPTIONS;EXPORT_NAME"
    "ADD_LIBS;LINK_LIBRARIES" ${ARGN})
  list(APPEND ARG_LINK_LIBRARIES ${ARG_ADD_LIBS})
  list(APPEND ARG_COMPILE_OPTIONS ${ARG_COMPILE_FLAGS})

  # Create the library target
  add_library(${_name} INTERFACE)

  # Link with specified libraries from parameter LINK_LIBRARIES
  target_link_libraries(${_name} INTERFACE "${ARG_LINK_LIBRARIES}")

  # Set target options from COMPILE_FLAGS
  target_compile_options(${_name} INTERFACE "${ARG_COMPILE_OPTIONS}")

  if(NOT ARG_NO_EXPORT)
    if(NOT ARG_EXPORT_NAME)
      message(DEPRECATION
        "The function dune_add_library(<lib> ...) now requires to provide NO_EXPORT or EXPORT_NAME. "
        "We recommend to choose a scoped export name with a camel title case matching your library name "
        "(e.g., Dune::Common, Dune::ISTL, OPM::Grid, ...)\n"
        " * Calls to `dune_add_library(<lib> ...)` without export specification will be supported until Dune 2.11\n"
        " * Consumption of unscoped targets `<lib>` will be supported until Dune 2.12")
      set(ARG_EXPORT_NAME __dune_impl_${_name})
    endif()

    set(${ProjectName}_EXPORT_SET ${ProjectName}-targets CACHE INTERNAL "")

    # Install targets to use the libraries in other modules.
    add_library(${ARG_EXPORT_NAME} ALIAS ${_name})
    set_target_properties(${_name} PROPERTIES EXPORT_NAME ${ARG_EXPORT_NAME})
    install(TARGETS ${_name}
      EXPORT ${${ProjectName}_EXPORT_SET} DESTINATION ${CMAKE_INSTALL_LIBDIR})

    # Install (unaliased) targets to use the libraries in other modules.
    # NOTE: Remove when compatibility with 2.9 is not needed anymore (e.g., 2.13)
    add_library(_dune_unaliased_${_name} INTERFACE)
    target_link_libraries(_dune_unaliased_${_name} INTERFACE ${ARG_EXPORT_NAME})
    set_target_properties(_dune_unaliased_${_name} PROPERTIES EXPORT_NAME ${_name})
    install(TARGETS _dune_unaliased_${_name}
      EXPORT ${${ProjectName}_EXPORT_SET} DESTINATION ${CMAKE_INSTALL_LIBDIR})

    # Register target as an exported library
    if(NOT ARG_NO_MODULE_LIBRARY)
      set_property(GLOBAL APPEND PROPERTY ${ProjectName}_INTERFACE_LIBRARIES ${ARG_EXPORT_NAME})
    endif()
  endif()

  # Register library in global property <module>_LIBRARIES
  if(NOT ARG_NO_MODULE_LIBRARY)
    set_property(GLOBAL APPEND PROPERTY ${ProjectName}_LIBRARIES ${name})
  endif()
endfunction(dune_add_library_interface)


# Create an object library that can be used to transport a set of sources
# \deprecated
function(dune_add_library_object _name)
  message(DEPRECATION "The function dune_add_library(<obj> OBJECT ...) is deprecated. "
    "Create a regular target in a parent scope, e.g., by dune_add_library(<target>), "
    "and fill it with sources using target_sources(<target> PRIVATE <sources>...).")

  cmake_parse_arguments(ARG
    "OBJECT"
    "COMPILE_FLAGS;COMPILE_OPTIONS"
    "ADD_LIBS;LINK_LIBRARIES;SOURCES" ${ARGN})
  list(APPEND ARG_SOURCES ${ARG_UNPARSED_ARGUMENTS})
  list(TRANSFORM ARG_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
  list(APPEND ARG_LINK_LIBRARIES ${ARG_ADD_LIBS})
  list(APPEND ARG_COMPILE_OPTIONS ${ARG_COMPILE_FLAGS})

  # Register sources, libs and flags for building the library later
  define_property(GLOBAL PROPERTY DUNE_LIB_${_name}_SOURCES
    BRIEF_DOCS "Convenience property with sources for library ${_name}. DO NOT EDIT!"
    FULL_DOCS "Convenience property with sources for library ${_name}. DO NOT EDIT!")
  set_property(GLOBAL PROPERTY DUNE_LIB_${_name}_SOURCES
    "${ARG_SOURCES}")
  define_property(GLOBAL PROPERTY DUNE_LIB_${_name}_ADD_LIBS
    BRIEF_DOCS "Convenience property with libraries for library ${_name}. DO NOT EDIT!"
    FULL_DOCS "Convenience property with libraries for library ${_name}. DO NOT EDIT!")
  set_property(GLOBAL PROPERTY DUNE_LIB_${_name}_ADD_LIBS
    "${ARG_LINK_LIBRARIES}")
  define_property(GLOBAL PROPERTY DUNE_LIB_${_name}_COMPILE_FLAGS
    BRIEF_DOCS "Convenience property with compile flags for library ${_name}. DO NOT EDIT!"
    FULL_DOCS "Convenience property with compile flags for library ${_name}. DO NOT EDIT!")
  set_property(GLOBAL PROPERTY DUNE_LIB_${_name}_COMPILE_FLAGS
    "${ARG_COMPILE_OPTIONS}")
endfunction(dune_add_library_object)


function(dune_expand_object_libraries _SOURCES_var _ADD_LIBS_var _COMPILE_FLAGS_var)
  set(_new_SOURCES "")
  set(_new_ADD_LIBS "${${_ADD_LIBS_var}}")
  set(_new_COMPILE_FLAGS "${${_COMPILE_FLAGS_var}}")
  set(_regex "_DUNE_TARGET_OBJECTS:([a-zA-Z0-9_-]+)_")
  foreach(_source ${${_SOURCES_var}})
    string(REGEX MATCH ${_regex} _matched "${_source}")
    if(_matched)
      string(REGEX REPLACE "${_regex}" "\\1" _basename  "${_source}")
      foreach(var _SOURCES _ADD_LIBS _COMPILE_FLAGS)
        get_property(_prop GLOBAL PROPERTY DUNE_LIB_${_basename}${var})
        list(APPEND _new${var} "${_prop}")
      endforeach()
    else()
      list(APPEND _new_SOURCES "${_source}")
    endif()
  endforeach()

  foreach(var _SOURCES _ADD_LIBS _COMPILE_FLAGS)
    set(${${var}_var} "${_new${var}}" PARENT_SCOPE)
  endforeach()
endfunction(dune_expand_object_libraries)
