# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

#[=======================================================================[.rst:
DuneParseArguments
------------------

Parse the arguments and extract options, one-valued args, multi-valued args,
and selections.

.. cmake:command:: dune_parse_arguments

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

    dune_parse_arguments(prefix options onevalued multivalued selections [args...])


#]=======================================================================]
include_guard(GLOBAL)

macro(dune_parse_arguments _prefix _options _onevalued _multivalued _selections)
  set(options ${_options})
  foreach(_s ${_selections})
    string(FIND "${_s}" ":" _pos REVERSE)
    math(EXPR _tailpos "${_pos} + 1")
    string(SUBSTRING "${_s}" ${_tailpos} -1 _tail)
    string(REPLACE "," ";" _list  "${_tail}")
    foreach(_item ${_list})
      list(APPEND options ${_item})
    endforeach(_item)
  endforeach(_s)

  cmake_parse_arguments(${_prefix} "${options}" "${_onevalued}" "${_multivalued}" ${ARGN})

  foreach(_s ${_selections})
    string(FIND "${_s}" ":" _headlength)
    string(SUBSTRING "${_s}" 0 ${_headlength} _head)
    string(FIND "${_s}" ":" _pos REVERSE)
    math(EXPR _defaultpos "${_headlength} + 1")
    math(EXPR _defaultlength "${_pos} - ${_defaultpos}")
    string(SUBSTRING "${_s}" ${_defaultpos} ${_defaultlength} _default)
    math(EXPR _first "${_pos} + 1")
    string(SUBSTRING "${_s}" ${_first} -1 _tail)
    string(REPLACE "," ";" _list  "${_tail}")

    set(${_prefix}_${_head} ${_default})
    foreach(_item ${_list})
      if(${_prefix}_${_item})
        set(${_prefix}_${_head} ${_item})
      endif()
    endforeach(_item)
  endforeach(_s)
endmacro(dune_parse_arguments)
