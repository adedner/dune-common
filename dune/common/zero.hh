// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_ZERO_HH
#define DUNE_COMMON_ZERO_HH

namespace Dune::Impl {

template <class T>
struct Zero
{
  static constexpr T value() noexcept
  {
    return T(0);
  }
};

template <class T>
constexpr T zero(const T& /*x*/) noexcept
{
  return Zero<T>::value();
}

template <class T>
constexpr T zero() noexcept
{
  return Zero<T>::value();
}

} // namespace Dune::Impl

#endif // DUNE_COMMON_ZERO_HH
