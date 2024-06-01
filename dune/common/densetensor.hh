// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_DENSETENSOR_HH
#define DUNE_COMMON_DENSETENSOR_HH

#include <type_traits>

#include <dune/common/tensorspan.hh>
#include <dune/common/std/default_accessor.hh>
#include <dune/common/std/extents.hh>
#include <dune/common/std/layout_left.hh>
#include <dune/common/std/layout_right.hh>
#include <dune/common/std/mdspan.hh>
#include <dune/common/std/type_traits.hh>

namespace Dune {

/**
 * \brief A tensor interface-class providing common functionality.
 *
 * \nosubgrouping
 *
 * The CRTP interface class for tensors extends the mdarray and mdspan implementations
 * by common functionality for element access and size information and mathematical
 * operations.
 *
 * \tparam Derived  The tensor type derived from this class.
 * \tparam Traits   Type containing the type aliases of a tensor, see TensorTraits.
 **/
template <class Derived, class Traits>
class DenseTensor
{
  using self_type = DenseTensor;
  using derived_type = Derived;

public:
  using element_type = typename Traits::element_type;
  using value_type = std::remove_cv_t<element_type>;
  using extents_type = typename Traits::extents_type;
  using size_type = typename extents_type::size_type;
  using rank_type = typename extents_type::rank_type;
  using index_type = typename extents_type::index_type;
  using layout_type = typename Traits::layout_type;
  using reference = element_type&;
  using const_reference = const element_type&;

public:

  /// \name Multi index access
  /// @{

  /// \brief Access specified element at position (i0,i1,...) with mutable access
  template <class... Indices,
    std::enable_if_t<(sizeof...(Indices) == extents_type::rank()), int> = 0,
    std::enable_if_t<(... && std::is_convertible_v<Indices, index_type>), int> = 0>
  constexpr decltype(auto) operator() (Indices... indices)
  {
    return asDerived()[std::array<index_type,sizeof...(Indices)>{index_type(indices)...}];
  }

  /// \brief Access specified element at position (i0,i1,...) with const access
  template <class... Indices,
    std::enable_if_t<(sizeof...(Indices) == extents_type::rank()), int> = 0,
    std::enable_if_t<(... && std::is_convertible_v<Indices, index_type>), int> = 0>
  constexpr decltype(auto) operator() (Indices... indices) const
  {
    return asDerived()[std::array<index_type,sizeof...(Indices)>{index_type(indices)...}];
  }

  /// \brief Access specified element at position [i0][i1]... with mutable access
  template <class E = extents_type, std::enable_if_t<(E::rank() >= 2), int> = 0>
  constexpr auto operator[] (index_type index) noexcept
  {
    return access(asDerived(), index, std::make_index_sequence<extents_type::rank()>{});
  }

  /// \brief Access specified element at position [i0][i1]... with const access
  template <class E = extents_type, std::enable_if_t<(E::rank() >= 2), int> = 0>
  constexpr auto operator[] (index_type index) const noexcept
  {
    return access(asDerived(), index, std::make_index_sequence<extents_type::rank()>{});
  }

  /// @}


  /// \name Observers
  /// @{

  /// \brief Number of rows of a rank-2 tensor
  template <class E = extents_type,
    std::enable_if_t<(E::rank() == 2), int> = 0>
  constexpr index_type rows () const noexcept
  {
    return asDerived().extent(0);
  }

  /// \brief Number of columns of a rank-2 tensor
  template <class E = extents_type,
    std::enable_if_t<(E::rank() == 2), int> = 0>
  constexpr index_type cols () const noexcept
  {
    return asDerived().extent(1);
  }

  /**
   * \brief Return true when (i0,i1,...) is in pattern.
   * This is always true for dense tensors.
   **/
  template <class... Indices,
    std::enable_if_t<(sizeof...(Indices) == extents_type::rank()), int> = 0,
    std::enable_if_t<(... && std::is_convertible_v<Indices, index_type>), int> = 0>
  constexpr bool exists ([[maybe_unused]] Indices... indices) const noexcept
  {
    return true;
  }

  /// @}


  /// \name Conversion to the underlying value if rank is zero
  // @{

  // specialization for mdarray
  template <class ScalarType, class E = extents_type, class D = derived_type,
    std::enable_if_t<std::is_convertible_v<value_type,ScalarType>, int> = 0,
    std::enable_if_t<(E::rank() == 0), int> = 0,
    decltype(std::declval<D>().container_data(), bool{}) = true >
  constexpr operator ScalarType () const noexcept
  {
    return ScalarType(*asDerived().container_data());
  }

  // specialization for mdspan
  template <class ScalarType, class E = extents_type, class D = derived_type,
    std::enable_if_t<std::is_convertible_v<value_type,ScalarType>, int> = 0,
    std::enable_if_t<(E::rank() == 0), int> = 0,
    decltype(std::declval<D>().accessor(), bool{}) = true,
    decltype(std::declval<D>().data_handle(), bool{}) = true >
  constexpr operator ScalarType () const noexcept
  {
    return ScalarType(asDerived().accessor().access(asDerived().data_handle(),0));
  }

  /// @}

private:
  template <class D>
  using HasContainerData = decltype(std::declval<D>().container_data());

  template <class D>
  static constexpr bool is_mdarray = Std::is_detected_v<HasContainerData,D>;

  template <class D>
  using HasAccessorAndDataHandle = decltype(std::declval<D>().accessor(), std::declval<D>().data_handle());

  template <class D>
  static constexpr bool is_mdspan = Std::is_detected_v<HasAccessorAndDataHandle,D>;

private:
  // obtain a sub-mdspan with fixed first index
  template <class L = layout_type, class D, std::size_t i0, std::size_t... ii,
    std::enable_if_t<std::is_same_v<L, Std::layout_right>, int> = 0>
  static constexpr auto access (D&& derived, index_type index, std::index_sequence<i0,ii...>)
  {
    using E = Std::extents<index_type, extents_type::static_extent(ii)...>;
    if constexpr (is_mdarray<D>) {
      using A = Std::default_accessor<value_type>;
      using V = std::conditional_t<std::is_const_v<std::remove_reference_t<D>>, const value_type, value_type>;
      return TensorSpan<V,E,L,A>(
        derived.container_data() + derived.mapping().stride(0)*index,
        derived.extent(ii)...);
    }
    else if constexpr (is_mdspan<D>) {
      using A = typename std::decay_t<decltype(derived.accessor())>::offset_policy;
      using V = typename A::element_type;
      return TensorSpan<V,E,L,A>(
        derived.accessor().offset(derived.data_handle(), derived.mapping().stride(0)*index),
        derived.extent(ii)...);
    }
    else {
      DUNE_THROW(Dune::NotImplemented, "SubTensor access not implemented for unknown tensor type. Must be derived from mdarray or mdspan.");
      return 0;
    }
  }

  template <class L = layout_type, class D, std::size_t i0, std::size_t... ii,
    std::enable_if_t<not std::is_same_v<L, Std::layout_right>, int> = 0>
  static constexpr auto access (D&& derived, index_type index, std::index_sequence<i0,ii...>)
  {
    DUNE_THROW(Dune::NotImplemented, "SubTensor access not implemented for layouts oher than layout_right.");
    return 0;
  }

private:

  derived_type const& asDerived () const
  {
    return static_cast<derived_type const&>(*this);
  }

  derived_type& asDerived ()
  {
    return static_cast<derived_type&>(*this);
  }
};

} // end namespace Dune

#endif // DUNE_COMMON_DENSETENSOR_HH
