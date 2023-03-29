# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

# check whether the user wants to overload compile flags upon calling make
#
# Provides the following macros:
#
#   finalize_compiler_script()   : needs to be called at the end of the cmake macros, e.g. in finalize_dune_project
#
# Those two macro calls are hooked into dune_project/finalize_dune_project.
#
# .. cmake_variable:: ALLOW_CXXFLAGS_OVERWRITE
#
#    Setting this option will allow you to overload preprocessor definitions from
#    the command line, as it was possible naturally with the autotools build system.
#    This feature only works with a :code:`Unix Makefiles` based generator. You can
#    use it as:
#
#    :code:`make CXXFLAGS="your flags" GRIDTYPE="grid type"`
#
#    :code:`GRIDTYPE` can be anything defined in :code:`config.h` via the :ref:`dune_define_gridtype` macro from dune-grid.
#    Furthermore any CPP variable of the form :code:`-DVAR=VALUE` can be overloaded on the command line.
#
#    .. note::
#       If you don't know what this is or what it's good for, don't use it.
#
include_guard(GLOBAL)

option(ALLOW_CXXFLAGS_OVERWRITE OFF)
option(ALLOW_CFLAGS_OVERWRITE OFF)
option(ALLOW_CXXFLAGS_OVERWRITE_FORCE OFF)

set(WRITE_CXXFLAGS_COMPILER_SCRIPT FALSE)
# write scripts if CMAKE_GENERATOR is Makefiles or if FORCE is on
if(${CMAKE_GENERATOR} MATCHES ".*Unix Makefiles.*" OR ALLOW_CXXFLAGS_OVERWRITE_FORCE)
  set(WRITE_CXXFLAGS_COMPILER_SCRIPT TRUE)
endif()

set(CXX_COMPILER_SCRIPT "${CMAKE_BINARY_DIR}/CXX_compiler.sh" )
set(C_COMPILER_SCRIPT "${CMAKE_BINARY_DIR}/C_compiler.sh" )

macro(find_extended_unix_commands)
  include(FindUnixCommands)
  set(FLAGSNAMES "ALLOW_CXXFLAGS_OVERWRITE and/or ALLOW_CFLAGS_OVERWRITE")
  find_program (GREP_PROGRAM grep)
  if(NOT GREP_PROGRAM)
    message( SEND_ERROR "grep not found, please disable ${FLAGSNAMES}")
  endif()
  find_program (SED_PROGRAM  sed)
  if(NOT SED_PROGRAM)
    message( SEND_ERROR "sed not found, please disable ${FLAGSNAMES}")
  endif()
  find_program (CUT_PROGRAM  cut)
  if(NOT CUT_PROGRAM)
    message( SEND_ERROR "cut not found, please disable ${FLAGSNAMES}")
  endif()
  find_program (ENV_PROGRAM  env)
  if(NOT ENV_PROGRAM)
    message( SEND_ERROR "env not found, please disable ${FLAGSNAMES}")
  endif()
  find_program (ECHO_PROGRAM echo)
  if(NOT ECHO_PROGRAM)
    message( SEND_ERROR "echo not found, please disable ${FLAGSNAMES}")
  endif()
  find_program (CHMOD_PROGRAM chmod)
  if(NOT CHMOD_PROGRAM)
    message( SEND_ERROR "chmod not found, please disable ${FLAGSNAMES}")
  endif()
  mark_as_advanced(GREP_PROGRAM)
  mark_as_advanced(SED_PROGRAM)
  mark_as_advanced(CUT_PROGRAM)
  mark_as_advanced(ENV_PROGRAM)
  mark_as_advanced(ECHO_PROGRAM)
  mark_as_advanced(CHMOD_PROGRAM)
endmacro(find_extended_unix_commands)

# generate a compiler script used as launcher
macro(finalize_compiler_script)
  if(WRITE_CXXFLAGS_COMPILER_SCRIPT)
    find_extended_unix_commands()
    # check CXX compiler
    set(COMPILERS)
    if(ALLOW_CXXFLAGS_OVERWRITE)
      list(APPEND COMPILERS "CXX")
    endif()
    # check C compiler
    if(ALLOW_CFLAGS_OVERWRITE)
      list(APPEND COMPILERS "C")
    endif()

    dune_module_path(MODULE dune-common RESULT scriptdir SCRIPT_DIR)

    # for the found compilers for flag overloading generate compiler script
    foreach(COMP ${COMPILERS})
      configure_file(${SCRIPT_DIR}/compiler.sh.in
      ${${COMP}_COMPILER_SCRIPT} @ONLY)
      set(CMAKE_${COMP}_COMPILER_LAUNCHER ${${COMP}_COMPILER_SCRIPT} CACHE STRING "")
      message("-- Generating ${COMP} compiler script for ${COMP}FLAGS overloading on command line")
    endforeach()
    unset(COMPILER)
    unset(COMPILER_SCRIPT)
  endif()
endmacro()
