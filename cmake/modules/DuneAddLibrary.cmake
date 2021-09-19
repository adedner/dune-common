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
    this created ``lib<libname>.so`` or ``lib<libname>.a``.

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

    dune_add_library(<basename> OBJECT
      [SOURCES <sources...>]
      [LINK_LIBRARIES <targets>...]
      [COMPILE_OPTIONS "<flags>;..."]
    )

    Create an object library target ``<basename>`` to collect multiple sources
    to be added to a library target later.

  ``SOURCES``
    The source files from which to build the library.

  ``LINK_LIBRARIES``
    A list of dependency the libraris is explicitly linked against

  ``COMPILE_OPTIONS``
    Any additional compile flags for building the library.


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


#]=======================================================================]
include_guard(GLOBAL)


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


function(dune_add_library_normal _name)
  cmake_parse_arguments(DUNE_LIB
    "NO_EXPORT;STATIC;SHARED;MODULE"
    "COMPILE_FLAGS;COMPILE_OPTIONS;OUTPUT_NAME;EXPORT_NAME"
    "ADD_LIBS;LINK_LIBRARIES;SOURCES" ${ARGN})
  list(APPEND DUNE_LIB_SOURCES ${DUNE_LIB_UNPARSED_ARGUMENTS})
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


function(dune_add_library_object _name)
  cmake_parse_arguments(DUNE_LIB
    ""
    "COMPILE_FLAGS;COMPILE_OPTIONS"
    "ADD_LIBS;LINK_LIBRARIES;SOURCES" ${ARGN})
  list(APPEND DUNE_LIB_SOURCES ${DUNE_LIB_UNPARSED_ARGUMENTS})
  list(APPEND DUNE_LIB_LINK_LIBRARIES ${DUNE_LIB_ADD_LIBS})
  list(APPEND DUNE_LIB_COMPILE_OPTIONS ${DUNE_LIB_COMPILE_FLAGS})

  # create lib
  add_library(${_name} OBJECT ${DUNE_LIB_SOURCES})

  # link with specified libraries from parameter ADD_LIBS
  target_link_libraries(${_name} PRIVATE "${DUNE_LIB_LINK_LIBRARIES}")

  # set target options from COMPILE_FLAGS
  target_compile_options(${_name} PRIVATE "${DUNE_LIB_COMPILE_OPTIONS}")
endfunction(dune_add_library_object)


function(dune_add_library_interface _name)
  cmake_parse_arguments(DUNE_LIB
    "NO_EXPORT"
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
