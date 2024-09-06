// SPDX-FileCopyrightText: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_COMMON_CONCEPTS_HASHABLE_HH
#define DUNE_COMMON_CONCEPTS_HASHABLE_HH

// check whether c++20 concept can be used
#if __has_include(<version>) && __has_include(<concepts>)
  #include <version>
  #if  __cpp_concepts >= 201907L && __cpp_lib_concepts >= 202002L
    #ifndef DUNE_ENABLE_CONCEPTS
    #define DUNE_ENABLE_CONCEPTS 1
    #endif
  #endif
#endif

#if DUNE_ENABLE_CONCEPTS

#include <concepts>
#include <limits>

#include <dune/common/concepts/identity.hh>
#include <dune/common/concepts/inverse.hh>

namespace Dune::Concept {

template <class G>
concept AdditiveGroup = std::regular<G> &&
requires(G g, std::plus<G> op)
{
  { op(g, g) } -> std::convertible_to<G>;
  { g + g } -> std::convertible_to<G>;
  { g - g } -> std::convertible_to<G>;
  { -g }    -> std::convertible_to<G>;

  { Concept::identity(g,op) } -> std::convertible_to<G>;
  { Concept::inverse(g,op) } -> std::convertible_to<G>;
};

template <class G>
concept MultiplicativeGroup = std::regular<G> &&
requires(G g, std::multiplies<G> op)
{
  { op(g, g) } -> std::convertible_to<G>;
  { g * g } -> std::convertible_to<G>;
  { g / g } -> std::convertible_to<G>;

  { Concept::identity(g,op) } -> std::convertible_to<G>;
  { Concept::inverse(g,op) } -> std::convertible_to<G>;
};

template <class F>
concept Field = AdditiveGroup<F> && MultiplicativeGroup<F>
  && std::numeric_limits<F>::is_specialized;

} // end namespace Dune::Concept

#endif // DUNE_ENABLE_CONCEPTS

#endif // DUNE_COMMON_CONCEPTS_HASHABLE_HH
