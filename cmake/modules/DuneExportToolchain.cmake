# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

#[=======================================================================[.rst:
DuneExportToolchain
-------------------

Export the current toolchain configuration into a file.

.. cmake:command:: dune_export_toolchain

  The cmake toolchain file configures the system, architecture and compiler. This
  function exports this configuration such that it can be used in other builds
  that require a compatible toolchain.

  .. code-block:: cmake

    dune_export_toolchain(<filename>
      [LANGUAGES {C,CXX,FORTRAN,ASM...}]
      [CONFIGS {DEBUG,RELEASE,RELWITHDEBINFO,MINSIZEREL}]
    )

    Export a set of toolchain variables into the file ``<filename>``. This file
    can be imported in another project by ``-DCMAKE_TOOLCHAIN_FILE`` flag.

  ``LANGUAGES``
    (optional) A list of languages for which to extract toolchain variables.
    Default is ``C CXX``.

  ``CONFIGS``
    (optional) A list of build configurations which to extract the flags from.
    Default is ``DEBUG RELEASE``.

#]=======================================================================]
include_guard(GLOBAL)


# Public interface for exporting a ŧoolchain file
function(dune_export_toolchain _filename)
  cmake_parse_arguments(ARG "" "" "LANGUAGES;CONFIGS" ${ARGN})

  if(NOT ARG_LANGUAGES)
    set(ARG_LANGUAGES "C" "CXX")
  endif()

  if(NOT ARG_CONFIGS)
    set(ARG_CONFIGS "DEBUG" "RELEASE")
  endif()

  set(_toolchain_required_vars
    "SYSTEM_NAME"
    "SYSTEM_PROCESSOR"
    "SYSTEM_VERSION"
    "LINKER")

  set(_toolchain_optional_vars
    "SYSROOT"
    "STAGING_PREFIX")

  foreach(_type "EXE" "MODULE" "SHARED" "STATIC")
    list(APPEND _toolchain_required_vars
      "${_type}_LINKER_FLAGS")
    foreach(_config ${ARG_CONFIGS})
      list(APPEND _toolchain_required_vars
        "${_type}_LINKER_FLAGS_${_config}")
    endforeach(_config)
  endforeach(_type)

  foreach(_lang ${ARG_LANGUAGES})
    list(APPEND _toolchain_required_vars
      "${_lang}_COMPILER"
      "${_lang}_COMPILER_ID"
      "${_lang}_COMPILER_VERSION"
      "${_lang}_FLAGS")
    list(APPEND _toolchain_optional_vars
      "${_lang}_COMPILER_TARGET"
      "${_lang}_COMPILER_EXTERNAL_TOOLCHAIN")
    foreach(_config ${ARG_CONFIGS})
      list(APPEND _toolchain_required_vars
        "${_lang}_FLAGS_${_config}")
    endforeach(_config)
  endforeach(_lang)

  set(_output "# Toolchain file exported automatically")
  foreach(_var ${_toolchain_required_vars})
    set(_output "${_output}\nset(CMAKE_${_var} \"${CMAKE_${_var}}\")")
  endforeach(_var)

  foreach(_var ${_toolchain_optional_vars})
    if(CMAKE_${_var})
      set(_output "${_output}\nset(CMAKE_${_var} \"${CMAKE_${_var}}\")")
    endif()
  endforeach(_var)

  file(WRITE ${_filename} ${_output})
  message(STATUS "Toolchain file has been written to: ${_filename}")
endfunction(dune_export_toolchain)
