// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_STD_IMPL_FWD_LAYOUTS_HH
#define DUNE_COMMON_STD_IMPL_FWD_LAYOUTS_HH

#if ! DUNE_HAVE_CXX_STD_MDSPAN

namespace Dune::Std {

struct layout_left
{
  template <class Extents>
  class mapping;
};

struct layout_right
{
  template <class Extents>
  class mapping;
};

struct layout_stride
{
  template <class Extents>
  class mapping;
};

} // end namespace Dune::Std

#endif // ! DUNE_HAVE_CXX_STD_MDSPAN

#endif // DUNE_COMMON_STD_IMPL_FWD_LAYOUTS_HH
