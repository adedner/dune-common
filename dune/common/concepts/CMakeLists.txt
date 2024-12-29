# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

install(FILES
  container.hh
  field.hh
  hashable.hh
  identity.hh
  inverse.hh
DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dune/common/concepts)
