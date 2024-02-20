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

namespace Dune {

  /** @addtogroup DenseMatVec
      @{
   */

  /*! \file
   * \brief This file implements a dense vector view with a dynamic size.
   */

  template< class K > class DenseVectorView;
  template< class K >
  struct DenseMatVecTraits< DenseVectorView< K > >
  {
    typedef DenseVectorView< K > derived_type;
    typedef K value_type;
    typedef std::size_t size_type;
  };

  template< class K >
  struct FieldTraits< DenseVectorView< K > >
  {
    typedef typename FieldTraits< K >::field_type field_type;
    typedef typename FieldTraits< K >::real_type real_type;
  };

  /** \brief Construct a vector-like view with a dynamic size.
   *
   * \tparam K is the field type (use float, double, complex, etc)
   */
  template< class K >
  class DenseVectorView : public DenseVector< DenseVectorView< K > >
  {
    using Base = DenseVector< DenseVectorView< K > >;

  public:
    using size_type = typename Base::size_type ;
    using value_type = typename Base::value_type;
    using pointer = value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;

    constexpr DenseVectorView (pointer p, size_type s) noexcept
      : pointer_(p)
      , size_(s)
    {}

    template <class A, class _K = K, std::enable_if_t<not std::is_const_v<_K>,int> = 0>
    constexpr DenseVectorView (std::vector<K,A>& v) noexcept
      : DenseVectorView(v.data(), v.size())
    {}

    template <class A, class _K = K, std::enable_if_t<std::is_const_v<_K>,int> = 0>
    constexpr DenseVectorView (std::vector<K,A> const& v) noexcept
      : DenseVectorView(v.data(), v.size())
    {}

    constexpr size_type size () const { return size_; }

    constexpr K& operator[] (size_type i) noexcept
    {
      DUNE_ASSERT_BOUNDS(i < size_);
      return pointer_[i];
    }

    constexpr const K& operator[] (size_type i) const noexcept
    {
      DUNE_ASSERT_BOUNDS(i < size_);
      return pointer_[i];
    }

  private:
    pointer pointer_;
    size_type size_;
  };

  template <class K>
  DenseVectorView (K*, std::size_t)
    -> DenseVectorView<K>;

  template <class K, class A>
  DenseVectorView (std::vector<K,A>&)
    -> DenseVectorView<K>;

  template <class K, class A>
  DenseVectorView (std::vector<K,A> const&)
    -> DenseVectorView<const K>;

  /** @} end documentation */

} // end namespace

#endif
