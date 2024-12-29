// SPDX-FileCopyrightText: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_COMMON_CONCEPTS_IDENTITY_HH
#define DUNE_COMMON_CONCEPTS_IDENTITY_HH

#include <complex>
#include <concepts>
#include <functional>

namespace Dune::Concept {

//! Identity is the neutral element of type `T` of the operation `Op`
template <class T, class Op>
struct Identity;

// Primary template depends on specializations of the template Identity
template <class T, class Op>
constexpr auto identity (T value, Op) noexcept
  -> decltype(Identity<T,Op>{}(value))
{
  return Identity<T,Op>{}(value);
}

// Overload for floating point types
template <std::floating_point F>
constexpr F identity (F /*value*/, std::plus<>) noexcept
{
  return F(0);
}

template <std::floating_point F>
constexpr F identity (F /*value*/, std::multiplies<>) noexcept
{
  return F(1);
}

// Specialization for complex types
template <std::floating_point F>
struct Identity<std::complex<F>, std::plus<>>
{
  constexpr std::complex<F> operator() (std::complex<F> /*value*/) const noexcept
  {
    return {F(0),F(0)};
  }
};

template <std::floating_point F>
struct Identity<std::complex<F>, std::multiplies<>>
{
  constexpr std::complex<F> operator() (std::complex<F> /*value*/) const noexcept
  {
    return {F(1),F(0)};
  }
};

} // end namespace Dune::Concept

#endif // DUNE_COMMON_CONCEPTS_IDENTITY_HH