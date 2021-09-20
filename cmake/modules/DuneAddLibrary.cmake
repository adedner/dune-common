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
    A list of dependency the libraris is explicitly linked against

  ``COMPILE_OPTIONS``
    Any additional compile flags for building the library.

  ``OUTPUT_NAME``
    Name of the library file, e.g. ``lib<libname>.so`` or ``lib<libname>.a``.

  ``EXPORT_NAME`
    Name of the exported target to be used when linking against the library
    Note, the ``<exportname>`` is always prefixed with the ``Dune::`` namespace.

  ``NO_EXPORT``
    If omitted the library is exported for usage in other modules.


  .. code-block:: cmake

    dune_add_library(<basename> INTERFACE
      [LINK_LIBRARIES <targets>...]
      [COMPILE_OPTIONS "<flags>;..."]
      [EXPORT_NAME <exportname>]
      [NO_EXPORT]
    )

    Create an interface library target with ``<basename>`` for the library name.
    An interface target does not contain any sources but my contain flags and
    dependencies.

  ``LINK_LIBRARIES``
    A list of dependency the libraris is explicitly linked against

  ``COMPILE_OPTIONS``
    Any additional compile flags for building the library.

  ``EXPORT_NAME`
    Name of the exported target to be used when linking against the library
    Note, the ``<exportname>`` is always prefixed with the ``Dune::`` namespace.

  ``NO_EXPORT``
    If omitted the library is exported for usage in other modules.


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
    A list of dependency the libraris is explicitly linked against

  ``COMPILE_OPTIONS``
    Any additional compile flags for building the library.

#]=======================================================================]
include_guard(GLOBAL)


### Public interface for creating a module library
function(dune_add_library _name)
  cmake_parse_arguments(DUNE_LIB "OBJECT;INTERFACE" "" "" ${ARGN})

  if(DUNE_LIB_OBJECT)
    dune_add_library_object(${_name} ${ARGN})
  elseif(DUNE_LIB_INTERFACE)
    dune_add_library_interface(${_name} ${ARGN})
  else()
    dune_add_library_normal(${_name} ${ARGN})
  endif()
endfunction(dune_add_library)


# ------------------------------------------------------------------------
# Internal macros and functions
# ------------------------------------------------------------------------


### Create a regular library target
function(dune_add_library_normal _name)
  cmake_parse_arguments(DUNE_LIB
    "NO_EXPORT;STATIC;SHARED;MODULE"
    "COMPILE_FLAGS;COMPILE_OPTIONS;OUTPUT_NAME;EXPORT_NAME"
    "ADD_LIBS;LINK_LIBRARIES;SOURCES" ${ARGN})
  list(APPEND DUNE_LIB_SOURCES ${DUNE_LIB_UNPARSED_ARGUMENTS})
  dune_expand_object_libraries(DUNE_LIB_SOURCES DUNE_LIB_ADD_LIBS DUNE_LIB_COMPILE_FLAGS)
  list(APPEND DUNE_LIB_LINK_LIBRARIES ${DUNE_LIB_ADD_LIBS})
  list(APPEND DUNE_LIB_COMPILE_OPTIONS ${DUNE_LIB_COMPILE_FLAGS})

  set(_type)
  if(DUNE_LIB_STATIC)
    set(_type "STATIC")
  elseif(DUNE_LIB_SHARED)
    set(_type "SHARED")
  elseif(DUNE_LIB_MODULE)
    set(_type "MODULE")
  endif()

  # create lib
  add_library(${_name} ${_type} ${DUNE_LIB_SOURCES})

  # link with specified libraries from parameter ADD_LIBS
  target_link_libraries(${_name} PUBLIC "${DUNE_LIB_LINK_LIBRARIES}")

  # set target options from COMPILE_FLAGS
  target_compile_options(${_name} PUBLIC "${DUNE_LIB_COMPILE_OPTIONS}")

  # Build library in ${PROJECT_BINARY_DIR}/lib
  set_target_properties(${_name} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

  if(DUNE_LIB_OUTPUT_NAME)
    set_target_properties(${_name} PROPERTIES OUTPUT_NAME ${DUNE_LIB_OUTPUT_NAME})
  endif()

  # prepare the export of the library
  if(NOT DUNE_LIB_NO_EXPORT)
    if(NOT DUNE_LIB_EXPORT_NAME)
      set(DUNE_LIB_EXPORT_NAME ${_name})
    endif()

    set_target_properties(${_name} PROPERTIES EXPORT_NAME ${DUNE_LIB_EXPORT_NAME})
    add_library(Dune::${DUNE_LIB_EXPORT_NAME} ALIAS ${_name})

    # register create library in global property DUNE_MODULE_LIBRARIES
    set_property(GLOBAL APPEND PROPERTY DUNE_MODULE_LIBRARIES Dune::${DUNE_LIB_EXPORT_NAME})

    # install targets to use the libraries in other modules.
    install(TARGETS ${_name}
      EXPORT ${ProjectName}-targets DESTINATION ${CMAKE_INSTALL_LIBDIR})
  else()
    # register create library in global property DUNE_MODULE_LIBRARIES
    set_property(GLOBAL APPEND PROPERTY DUNE_MODULE_LIBRARIES ${_name})
  endif()
endfunction(dune_add_library_normal)


### Create an interface target that does not compile any sources
function(dune_add_library_interface _name)
  cmake_parse_arguments(DUNE_LIB
    "NO_EXPORT;INTERFACE"
    "COMPILE_FLAGS;COMPILE_OPTIONS;EXPORT_NAME"
    "ADD_LIBS;LINK_LIBRARIES" ${ARGN})
  list(APPEND DUNE_LIB_LINK_LIBRARIES ${DUNE_LIB_ADD_LIBS})
  list(APPEND DUNE_LIB_COMPILE_OPTIONS ${DUNE_LIB_COMPILE_FLAGS})

  # create lib
  add_library(${_name} INTERFACE)

  # link with specified libraries from parameter ADD_LIBS
  target_link_libraries(${_name} INTERFACE "${DUNE_LIB_LINK_LIBRARIES}")

  # set target options from COMPILE_FLAGS
  target_compile_options(${_name} INTERFACE "${DUNE_LIB_COMPILE_OPTIONS}")

  # prepare the export of the library
  if(NOT DUNE_LIB_NO_EXPORT)
    if(NOT DUNE_LIB_EXPORT_NAME)
      set(DUNE_LIB_EXPORT_NAME ${_name})
    endif()

    set_target_properties(${_name} PROPERTIES EXPORT_NAME ${DUNE_LIB_EXPORT_NAME})
    add_library(Dune::${DUNE_LIB_EXPORT_NAME} ALIAS ${_name})

    # register create library in global property DUNE_MODULE_LIBRARIES
    set_property(GLOBAL APPEND PROPERTY DUNE_MODULE_LIBRARIES Dune::${DUNE_LIB_EXPORT_NAME})

    # install targets to use the libraries in other modules.
    install(TARGETS ${_name}
      EXPORT ${ProjectName}-targets DESTINATION ${CMAKE_INSTALL_LIBDIR})
  else()
    # register create library in global property DUNE_MODULE_LIBRARIES
    set_property(GLOBAL APPEND PROPERTY DUNE_MODULE_LIBRARIES ${_name})
  endif()
endfunction(dune_add_library_interface)


### Create an object library that can be used to transport a set of sources
### \deprecated
function(dune_add_library_object _name)
  message(DEPRECATION "The function dune_add_library(<obj> OBJECT ...) is deprecated. "
    "Create a regular target in a parent scope, e.g., by dune_add_library(<target>), "
    "and fill it with sources using target_source(<target> PRIVATE <sources>...).")

  cmake_parse_arguments(DUNE_LIB
    "OBJECT"
    "COMPILE_FLAGS;COMPILE_OPTIONS"
    "ADD_LIBS;LINK_LIBRARIES;SOURCES" ${ARGN})
  list(APPEND DUNE_LIB_SOURCES ${DUNE_LIB_UNPARSED_ARGUMENTS})
  list(APPEND DUNE_LIB_LINK_LIBRARIES ${DUNE_LIB_ADD_LIBS})
  list(APPEND DUNE_LIB_COMPILE_OPTIONS ${DUNE_LIB_COMPILE_FLAGS})

  foreach(source ${DUNE_LIB_SOURCES})
    list(APPEND full_path_sources ${CMAKE_CURRENT_SOURCE_DIR}/${source})
  endforeach()

  # register sources, libs and flags for building the library later
  define_property(GLOBAL PROPERTY DUNE_LIB_${_name}_SOURCES
    BRIEF_DOCS "Convenience property with sources for library ${_name}. DO NOT EDIT!"
    FULL_DOCS "Convenience property with sources for library ${_name}. DO NOT EDIT!")
  set_property(GLOBAL PROPERTY DUNE_LIB_${_name}_SOURCES
    "${full_path_sources}")
  define_property(GLOBAL PROPERTY DUNE_LIB_${_name}_ADD_LIBS
    BRIEF_DOCS "Convenience property with libraries for library ${_name}. DO NOT EDIT!"
    FULL_DOCS "Convenience property with libraries for library ${_name}. DO NOT EDIT!")
  set_property(GLOBAL PROPERTY DUNE_LIB_${_name}_ADD_LIBS
    "${DUNE_LIB_LINK_LIBRARIES}")
  define_property(GLOBAL PROPERTY DUNE_LIB_${_name}_COMPILE_FLAGS
    BRIEF_DOCS "Convenience property with compile flags for library ${_name}. DO NOT EDIT!"
    FULL_DOCS "Convenience property with compile flags for library ${_name}. DO NOT EDIT!")
  set_property(GLOBAL PROPERTY DUNE_LIB_${_name}_COMPILE_FLAGS
    "${DUNE_LIB_COMPILE_OPTIONS}")
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
