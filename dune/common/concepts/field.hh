// SPDX-FileCopyrightText: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_COMMON_CONCEPTS_FIELD_HH
#define DUNE_COMMON_CONCEPTS_FIELD_HH

#include <concepts>
#include <functional>
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


/**
 * \brief The `Field` concept is satisfied by all regular types where addition (+),
 * subtraction (-), multiplication (*), and division (/) are defined and behave
 * as the corresponding operations in real numbers.
 *
 * A field is a set of numbers with two operations `+` and `*`, and corresponding
 * inverse operations `-` and `/`. Associated to these operations are identity
 * element, the zero `0` and one `1` for addition and multiplication, respectively.
 * In order to distinguish these numbers, they must be equality comparable. To
 * work with these numbers we additionally require that they are semiregular, i.e.,
 * default constructible, copyable and movable.
 *
 * \b Examples:
 * - floating-point types like `double`, and `float`
 * - complex number types, e.g., `std::complex<double>`
 */
template <class F>
concept Field = std::regular<F> && AdditiveGroup<F> && MultiplicativeGroup<F>;

static_assert(Field<Dune::Concept::Archetypes::Field>);

} // end namespace Dune::Concept

#endif // DUNE_COMMON_CONCEPTS_FIELD_HH
