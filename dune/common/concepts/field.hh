// SPDX-FileCopyrightText: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_COMMON_CONCEPTS_FIELD_HH
#define DUNE_COMMON_CONCEPTS_FIELD_HH

#include <concepts>
#include <limits>

#include <dune/common/concepts/identity.hh>
#include <dune/common/concepts/inverse.hh>
#include <dune/common/concepts/archetypes/field.hh>

namespace Dune::Concept {

template <class G>
concept AdditiveGroup = requires(G g, std::plus<> op)
{
  { op(g, g) } -> std::convertible_to<G>;
  { g + g } -> std::convertible_to<G>;
  { g - g } -> std::convertible_to<G>;
  { -g }    -> std::convertible_to<G>;

  { identity(g,op) } -> std::convertible_to<G>;
  { inverse(g,op) } -> std::convertible_to<G>;
};

static_assert(AdditiveGroup<Dune::Concept::Archetypes::AdditiveGroup<>>);

template <class G>
concept MultiplicativeGroup = requires(G g, std::multiplies<> op)
{
  { op(g, g) } -> std::convertible_to<G>;
  { g * g } -> std::convertible_to<G>;
  { g / g } -> std::convertible_to<G>;

  { identity(g,op) } -> std::convertible_to<G>;
  { inverse(g,op) } -> std::convertible_to<G>;
};

static_assert(MultiplicativeGroup<Dune::Concept::Archetypes::MultiplicativeGroup<>>);

template <class F>
concept Field = std::regular<F> && AdditiveGroup<F> && MultiplicativeGroup<F>
  && std::numeric_limits<F>::is_specialized;

static_assert(Field<Dune::Concept::Archetypes::Field>);

} // end namespace Dune::Concept

#endif // DUNE_COMMON_CONCEPTS_FIELD_HH
