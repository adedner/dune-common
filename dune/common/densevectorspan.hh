// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_DENSEVECTORVIEW_HH
#define DUNE_COMMON_DENSEVECTORVIEW_HH

#include <array>
#include <concepts>
#include <cstddef>
#include <type_traits>
#include <vector>

#include <dune/common/densevector.hh>
#include <dune/common/ftraits.hh>
#include <dune/common/typetraits.hh>
#include <dune/common/std/mdspan.hh>

namespace Dune {

/** @addtogroup DenseMatVec
  *  @{
  */

/** \brief A vector-like span with static or dynamic size.
  *
  * \tparam K is the field type (use float, double, complex, etc)
  * \tparam Extent The static size of the vector or `Std::dynamic_extent` if the size is dynamic
  */
template <class K, std::size_t Extent = Std::dynamic_extent>
class DenseVectorSpan
    : private Std::mdspan< K, Std::extents<std::size_t,Extent> >
    , public DenseVector< DenseVectorSpan<K, Extent> >
{
  using Base = Std::mdspan< K, Std::extents<std::size_t,Extent> >;
  using Interface = DenseVector< DenseVectorSpan<K, Extent> >;

public:
  // import all constructors from Std::mdspan
  using Base::Base;

  //! Construct a span over a single scalar value
  constexpr DenseVectorSpan (K& v) noexcept
    : Base(&v, 1)
  {}

  //! Construct a span over a container type like `std::vector` or `std::array`
  template <class Vector>
    requires requires(Vector&& v) {
      { v.data() } -> std::convertible_to<typename Base::data_handle_type>;
      { v.size() } -> std::convertible_to<typename Base::index_type>;
    }
  constexpr DenseVectorSpan (Vector&& v) noexcept
    : Base(v.data(), v.size())
  {}

  // make assignment of other vectors possible
  using Interface::operator=;

  // make the size function from Base visible
  using Base::size;

  // import element access operator from Base
  using Base::operator[];
};

// deduction guides

template <class K>
DenseVectorSpan (K*, std::size_t)
  -> DenseVectorSpan<K>;

template <class K, std::size_t N>
DenseVectorSpan (K (&)[N])
  -> DenseVectorSpan<K,N>;

template <class S, std::enable_if_t<Dune::IsNumber<S>::value,int> = 0>
DenseVectorSpan (S&)
  -> DenseVectorSpan<S,1>;

template <class S, std::enable_if_t<Dune::IsNumber<S>::value,int> = 0>
DenseVectorSpan (const S&)
  -> DenseVectorSpan<const S,1>;

template <class K, class A>
DenseVectorSpan (std::vector<K,A>&)
  -> DenseVectorSpan<K>;

template <class K, class A>
DenseVectorSpan (std::vector<K,A> const&)
  -> DenseVectorSpan<const K>;

template <class T, std::size_t N>
DenseVectorSpan (std::array<T,N>&)
  -> DenseVectorSpan<T,N>;

template <class T, std::size_t N>
DenseVectorSpan (std::array<T,N> const&)
  -> DenseVectorSpan<const T,N>;

// forward declaration
template <class T, int n>
class ReservedVector;

template <class T, int n>
DenseVectorSpan (ReservedVector<T,n>&)
  -> DenseVectorSpan<T>;

template <class T, int n>
DenseVectorSpan (ReservedVector<T,n> const&)
  -> DenseVectorSpan<const T>;

// forward declaration
template <class K, class A>
class DynamicVector;

template <class K, class A>
DenseVectorSpan (DynamicVector<K,A>&)
  -> DenseVectorSpan<K>;

template <class K, class A>
DenseVectorSpan (DynamicVector<K,A> const&)
  -> DenseVectorSpan<const K>;

// forward declaration
template <class K, int N>
class FieldVector;

template <class K, int N>
DenseVectorSpan (FieldVector<K,N>&)
  -> DenseVectorSpan<K,std::size_t(N)>;

template <class K, int N>
DenseVectorSpan (FieldVector<K,N> const&)
  -> DenseVectorSpan<const K,std::size_t(N)>;


template <class K, std::size_t Extent>
struct DenseMatVecTraits< DenseVectorSpan<K, Extent> >
{
  using derived_type = DenseVectorSpan<K, Extent>;
  using value_type = K;
  using size_type = std::size_t;
};

template <class K, std::size_t Extent>
struct FieldTraits< DenseVectorSpan<K, Extent> >
{
  using field_type = typename FieldTraits<K>::field_type;
  using real_type = typename FieldTraits<K>::real_type;
};

/** @} end documentation */

} // end namespace Dune

#endif // DUNE_COMMON_DENSEVECTORVIEW_HH
