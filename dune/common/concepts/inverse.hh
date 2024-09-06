// SPDX-FileCopyrightText: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_COMMON_CONCEPTS_INVERSE_HH
#define DUNE_COMMON_CONCEPTS_INVERSE_HH

#include <cassert>
#include <complex>
#include <concepts>
#include <functional>

namespace Dune::Concept {

//! Zero is the neutral element of the addition
template <class T, class Op>
struct Inverse;

// Primary template depends on specializations of the template Zero
template <class T, class Op>
constexpr auto inverse (T value, Op) noexcept
  -> decltype(Inverse<T,Op>{}(value))
{
  return Inverse<T,Op>{}(value);
}

// Overload for floating point types
template <std::floating_point F>
constexpr F inverse (F value, std::plus<F>) noexcept
{
  return -value;
}

template <std::floating_point F>
constexpr F inverse (F value, std::multiplies<F>) noexcept
{
  assert(value != F(0));
  return F(1) / value;
}

// Specialization for complex types
template <std::floating_point F>
struct Inverse<std::complex<F>, std::plus<F>>
{
  constexpr std::complex<F> operator() (std::complex<F> value) const noexcept
  {
    return -value;
  }
};

template <std::floating_point F>
struct Inverse<std::complex<F>, std::multiplies<F>>
{
  constexpr std::complex<F> operator() (std::complex<F> value) const noexcept
  {
    assert(value != std::complex<F>(0,0));
    return F(1) / value;
  }
};

} // end namespace Dune::Concept

#endif // DUNE_COMMON_CONCEPTS_INVERSE_HH