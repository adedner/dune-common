# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

add_subdirectory(dune)

configure_file(MANIFEST.in MANIFEST.in)
configure_file(pyproject.toml.in pyproject.toml)

dune_python_configure_bindings(
  PATH "."
  CMAKE_METADATA_FLAGS DUNE_OPTS_FILE HAVE_MPI CMAKE_BUILD_TYPE CMAKE_CXX_COMPILER CMAKE_CXX_FLAGS CMAKE_C_COMPILER CMAKE_C_FLAGS CMAKE_PREFIX_PATH CMAKE_CXX_COMPILER_LAUNCHER
)
