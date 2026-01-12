# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

add_subdirectory(common)
add_subdirectory(test)
if(!DUNE_USE_SYSTEM_PYBIND11)
  add_subdirectory(pybind11)
endif()
