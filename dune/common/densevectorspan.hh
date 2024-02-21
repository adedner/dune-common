// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_DENSEVECTORVIEW_HH
#define DUNE_COMMON_DENSEVECTORVIEW_HH

#include <cstddef>
#include <vector>

#include <dune/common/boundschecking.hh>
#include <dune/common/densevector.hh>
#include <dune/common/ftraits.hh>
#include <dune/common/typetraits.hh>
#include <dune/common/std/mdspan.hh>

namespace Dune {

  /** @addtogroup DenseMatVec
      @{
   */

  /*! \file
   * \brief This file implements a dense vector view with a dynamic size.
   */

  template< class K, std::size_t Extent > class DenseVectorSpan;
  template< class K, std::size_t Extent >
  struct DenseMatVecTraits< DenseVectorSpan< K, Extent > >
  {
    typedef DenseVectorSpan< K, Extent > derived_type;
    typedef K value_type;
    typedef std::size_t size_type;
  };

  template< class K, std::size_t Extent >
  struct FieldTraits< DenseVectorSpan< K, Extent > >
  {
    typedef typename FieldTraits< K >::field_type field_type;
    typedef typename FieldTraits< K >::real_type real_type;
  };

  /** \brief Construct a vector-like span with static or dynamic size.
   *
   * \tparam K is the field type (use float, double, complex, etc)
   * \tparam Extent The static size of the vector or `Std::dynamic_extent` if the size is dynamic
   */
  template< class K, std::size_t Extent = Std::dynamic_extent >
  class DenseVectorSpan
      : private Std::mdspan< K, Std::extents<std::size_t,Extent> >
      , public DenseVector< DenseVectorSpan< K, Extent > >
  {
    using Base = Std::mdspan< K, Std::extents<std::size_t,Extent> >;
    using Interface = DenseVector< DenseVectorSpan< K, Extent > >;

  public:
    // import all constructors from Std::mdspan
    using Base::Base;

    //! Construct a span over a single scalar value
    template <class Scalar,
      std::enable_if_t<Dune::IsNumber<std::decay_t<Scalar>>::value, int> = 0>
    constexpr DenseVectorSpan (Scalar&& v) noexcept
      : Base(&v, 1)
    {}

    //! Construct a span over a container type like `std::vector`, `DynamicVector`, or `FieldVector`
    template <class Vector,
      class P = decltype(std::declval<Vector>().data()),
      class S = decltype(std::declval<Vector>().size()),
      std::enable_if_t<std::is_convertible_v<P,typename Base::data_handle_type>, int> = 0,
      std::enable_if_t<std::is_convertible_v<S,typename Base::index_type>, int> = 0>
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

  template <class K>
  DenseVectorSpan (K*, std::size_t)
    -> DenseVectorSpan<K>;

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

  /** @} end documentation */

} // end namespace

#endif
