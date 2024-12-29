// SPDX-FileCopyrightText: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_COMMON_CONCEPTS_ARCHETYPES_FIELD_HH
#define DUNE_COMMON_CONCEPTS_ARCHETYPES_FIELD_HH

#include <concepts>
#include <functional>
#include <limits>
#include <type_traits>

namespace Dune::Concept::Archetypes {

struct Swappable
{
  friend void swap (Swappable&, Swappable&);
};

struct Movable
  : public Swappable
{
  Movable (Movable&&);
  Movable& operator= (Movable&&);

  Movable (const Movable&) = delete;
  Movable& operator= (const Movable&) = delete;
};

static_assert(std::movable<Movable>);
static_assert(not std::copyable<Movable>);

struct Copyable
  : public Movable
{
  using Movable::Movable;
  Copyable (const Copyable&);
  Copyable& operator= (const Copyable&);
  Copyable& operator= (Copyable&&) = default;
};

static_assert(std::copyable<Copyable>);


struct SemiRegular
  : public Copyable
{
  using Copyable::Copyable;
  SemiRegular ();
  SemiRegular& operator= (const SemiRegular&) = default;
};

static_assert(std::semiregular<SemiRegular>);


struct EqualityComparable
{
  bool operator== (const EqualityComparable&) const;
  bool operator!= (const EqualityComparable&) const;
};

static_assert(std::equality_comparable<EqualityComparable>);


struct Regular
  : public SemiRegular
  , public EqualityComparable
{
  Regular () = default;
  Regular (const Regular&) = default;
  Regular (Regular&&) = default;
  Regular& operator= (const Regular&) = default;
  Regular& operator= (Regular&&) = default;
};

static_assert(std::regular<Regular>);


template <class Derived = void>
struct AdditiveGroup
{
  using return_type = std::conditional_t<std::is_void_v<Derived>, AdditiveGroup, Derived>;
  return_type operator+ (const AdditiveGroup&);
  return_type operator- (const AdditiveGroup&);
  return_type operator- ();
};

template <class Derived>
typename AdditiveGroup<Derived>::return_type
identity (const AdditiveGroup<Derived>&, std::plus<>);

template <class Derived>
typename AdditiveGroup<Derived>::return_type
inverse (const AdditiveGroup<Derived>&, std::plus<>);


template <class Derived = void>
struct MultiplicativeGroup
{
  using return_type = std::conditional_t<std::is_void_v<Derived>, MultiplicativeGroup, Derived>;
  return_type operator* (const MultiplicativeGroup&);
  return_type operator/ (const MultiplicativeGroup&);
};

template <class Derived>
typename MultiplicativeGroup<Derived>::return_type
identity (const MultiplicativeGroup<Derived>&, std::multiplies<>);

template <class Derived>
typename MultiplicativeGroup<Derived>::return_type
inverse (const MultiplicativeGroup<Derived>&, std::multiplies<>);


struct Field
  : public AdditiveGroup<Field>
  , public MultiplicativeGroup<Field>
  , public Regular
{
  Field () = default;
  Field (const Field&) = default;
  Field (Field&&) = default;

  Field& operator= (const Field&) = default;
  Field& operator= (Field&&) = default;
};

} // end namespace Dune::Concept::Archetypes

template <>
class std::numeric_limits<Dune::Concept::Archetypes::Field>
  : public std::numeric_limits<double>
{
public:
  static constexpr bool is_specialized = true;
};

#endif // DUNE_COMMON_CONCEPTS_ARCHETYPES_FIELD_HH
