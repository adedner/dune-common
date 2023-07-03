# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

#[=======================================================================[.rst:
DuneTargetForwardProperties
---------------------------

Extract all (interface) target properties and apply them manually to the
given output target.

.. cmake:command:: dune_target_forward_properties

  The command ``dune_parse_arguments()`` is similar to the cmake_parse_arguments
  macros, but adds one more option to parse the function arguments: selections.
  a selection is a list of possible values where the output is the value itself
  if it is given as an option. An example is the "scope" in the ``add_library``
  command. It can be either PUBLIC, PRIVATE, or INTERFACE and we want to get this
  scope as a variable to forward to other cmake functions.

  A selection is given with a key, a default value and possible values in the form
  "key:default:value1,values2...". Note the "," instead of a ";" in the list of
  possible values. Multiple selections can be separated by semicolon.

  .. code-block:: cmake

    dune_target_forward_properties(<target> <scope> <other>)


    Extract all interface target properties from ``<other>`` annd apply them
    using ``target_xxx_yyy(<target> <scope> _property_)`` to the given ``<target>``.


#]=======================================================================]
include_guard(GLOBAL)

function(dune_target_forward_properties _target _scope _other)
  # List of public compile definitions requirements for a library.
  get_target_property(defs ${_other} INTERFACE_COMPILE_DEFINITIONS)
  if(defs)
    target_compile_definitions(${_target} ${_scope} "${defs}")
  endif()

  # List of public compile features requirements for a library.
  get_target_property(features ${_other} INTERFACE_COMPILE_FEATURES)
  if(features)
    target_compile_features(${_target} ${_scope} "${features}")
  endif()

  # List of public compile options requirements for a library.
  get_target_property(opts ${_other} INTERFACE_COMPILE_OPTIONS)
  if(opts)
    target_compile_options(${_target} ${_scope} "${opts}")
  endif()

  # List of public include directories requirements for a library.
  get_target_property(incs ${_other} INTERFACE_INCLUDE_DIRECTORIES)
  if(incs)
    target_include_directories(${_target} ${_scope} "${incs}")
  endif()

  # List of public link directories requirements for a library.
  get_target_property(linkdirs ${_other} INTERFACE_LINK_DIRECTORIES)
  if(linkdirs)
    target_link_directories(${_target} ${_scope} "${linkdirs}")
  endif()

  # List public interface libraries for a library.
  get_target_property(libs ${_other} INTERFACE_LINK_LIBRARIES)
  if(libs)
    target_link_libraries(${_target} ${_scope} "${libs}")
  endif()

  # List of public link options requirements for a library.
  get_target_property(linkopts ${_other} INTERFACE_LINK_OPTIONS)
  if(linkopts)
    target_link_options(${_target} ${_scope} "${linkopts}")
  endif()
endfunction(dune_target_forward_properties)



function(dune_global_properties_from_target _all)
  cmake_parse_arguments(ARG "APPEND;VERBOSE" "" "" ${ARGN})

  # List of public compile definitions requirements for a library.
  get_target_property(defs ${_all} INTERFACE_COMPILE_DEFINITIONS)
  if(defs)
    add_compile_definitions("${defs}")
    if(ARG_VERBOSE)
      message("(global) compile definitions: ${defs}")
    endif()
  endif()

  # List of public compile features requirements for a library.
  get_target_property(features ${_all} INTERFACE_COMPILE_FEATURES)
  if(features)
    if(ARG_VERBOSE)
      message(WARNING "(global) compile features: [not supported]")
    endif()
  endif()

  # List of public compile options requirements for a library.
  get_target_property(opts ${_all} INTERFACE_COMPILE_OPTIONS)
  if(opts)
    add_compile_options("${opts}")
    if(ARG_VERBOSE)
      message("(global) compile options: ${opts}")
    endif()
  endif()

  # List of public include directories requirements for a library.
  get_target_property(incs ${_all} INTERFACE_INCLUDE_DIRECTORIES)
  if(incs)
    if(ARG_APPEND)
      include_directories(AFTER "${incs}")
    else()
      include_directories(BEFORE "${incs}")
    endif()
    if(ARG_VERBOSE)
      message("(global) include directories: ${incs}")
    endif()
  endif()

  # List of public link directories requirements for a library.
  get_target_property(linkdirs ${_all} INTERFACE_LINK_DIRECTORIES)
  if(linkdirs)
    if(ARG_APPEND)
      link_directories(AFTER "${linkdirs}")
    else()
      link_directories(BEFORE "${linkdirs}")
    endif()
    if(ARG_VERBOSE)
      message("(global) link directories: ${linkdirs}")
    endif()
  endif()

  # List public interface libraries for a library.
  get_target_property(libs ${_all} INTERFACE_LINK_LIBRARIES)
  if(libs)
    link_libraries("${libs}")
    if(ARG_VERBOSE)
      message("(global) link libraries: ${libs}")
    endif()
  endif()

  # List of public link options requirements for a library.
  get_target_property(linkopts ${_all} INTERFACE_LINK_OPTIONS)
  if(linkopts)
    link_libraries("${linkopts}")
    if(ARG_VERBOSE)
      message("(global) link options: ${linkopts}")
    endif()
  endif()
endfunction(dune_global_properties_from_target)
