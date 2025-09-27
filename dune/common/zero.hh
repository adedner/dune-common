// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_ZERO_HH
#define DUNE_COMMON_ZERO_HH

namespace Dune {

/**
 * \brief The additive identity element for the type `T`.
 *
 * The additive identity `zero` should satisfy `zero + x = x + zero = x` for all
 * `x` of type `T`. It is by default defined as `T(0)`. The template can be
 * specialized for user-defined types, if the construction from a zero number is
 * not possible.
 *
 * \relates zero()
 */
template <class T>
struct AdditiveIdentity
{
  static constexpr T value() noexcept
  {
    return T(0);
  }
};

// specialization for const types
template <class T>
struct AdditiveIdentity<const T> : AdditiveIdentity<T> {};

/// The additive identity element for the type of `x`.
template <class T>
constexpr T zero([[maybe_unused]] const T& x) noexcept
{
  return AdditiveIdentity<T>::value();
}

/// The additive identity element for the type `T`.
template <class T>
constexpr T zero() noexcept
{
  return AdditiveIdentity<T>::value();
}

} // namespace Dune

#endif // DUNE_COMMON_ZERO_HH
