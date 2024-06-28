// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_DENSETENSOR_HH
#define DUNE_COMMON_DENSETENSOR_HH

#include <cassert>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

#include <dune/common/indices.hh>
#include <dune/common/initializerlist.hh>
#include <dune/common/std/type_traits.hh>

namespace Dune {

/**
 * \brief A tensor interface-class providing common functionality.
 *
 * \nosubgrouping
 *
 * The CRTP interface mixin-class for tensors extends the mdarray and mdspan implementations
 * by common functionality for element access and size information and mathematical
 * operations.
 *
 * \tparam Derived  The tensor type derived from this class.
 * \tparam Base     Either mdarray or mdspan base type for storage.
 **/
template <class Derived, class Base>
class DenseTensor
    : public Base
{
  using self_type = DenseTensor;
  using derived_type = Derived;
  using base_type = Base;

  template <class B>
  using const_reference_t = typename B::const_reference;

  static constexpr auto rank_ = base_type::rank();

public:
  using element_type = typename base_type::element_type;
  using value_type = typename base_type::element_type;
  using extents_type = typename base_type::extents_type;
  using size_type = typename base_type::size_type;
  using rank_type = typename base_type::rank_type;
  using index_type = typename base_type::index_type;
  using layout_type = typename base_type::layout_type;
  using mapping_type = typename base_type::mapping_type;
  using reference = typename base_type::reference;
  using const_reference = Std::detected_or_t<reference,const_reference_t,base_type>;

public:
  /// Derive constructors from base class
  using base_type::base_type;

  /// \name Additional DenseTensor constructors
  /// @{

  /// \brief Constructor from a brace-init list of values
  template <class E = extents_type,
    std::enable_if_t<std::is_default_constructible_v<E>, int> = 0>
  constexpr DenseTensor (NestedInitializerList_t<value_type,extents_type::rank()> init)
    : DenseTensor{extents_type{}, init}
  {}

  /// \brief Constructor from an extents and brace-init list of values
  template <class M = mapping_type,
    std::enable_if_t<std::is_constructible_v<M,extents_type>, int> = 0>
  constexpr DenseTensor (const extents_type& e, NestedInitializerList_t<value_type,extents_type::rank()> init)
    : DenseTensor{mapping_type{e}, init}
  {}

  /// \brief Constructor from a layout-mapping and brace-init list of values
  constexpr DenseTensor (const mapping_type& m, NestedInitializerList_t<value_type,extents_type::rank()> init)
    : base_type{m}
  {
    auto it = this->container_data();
    InitializerList<value_type,extents_type>::apply(init,this->extents(),
      [&it](value_type value) { *it++ = value; });
  }

  /// \brief Converting constructor from another DenseTensor
  template <class D, class B,
    std::enable_if_t<std::is_constructible_v<base_type, B>, int> = 0>
  constexpr DenseTensor (const DenseTensor<D,B>& other)
    : base_type{other}
  {}

  /// @}


  /// \name Multi index access
  /// @{

  /// \brief Access specified element at position (i0,i1,...) with mutable access
  template <class... Indices,
    std::enable_if_t<(sizeof...(Indices) == extents_type::rank()), int> = 0,
    std::enable_if_t<(... && std::is_convertible_v<Indices, index_type>), int> = 0>
  constexpr reference operator() (Indices... indices)
  {
    return base_type::operator[](std::array<index_type,extents_type::rank()>{index_type(indices)...});
  }

  /// \brief Access specified element at position (i0,i1,...) with const access
  template <class... Indices,
    std::enable_if_t<(sizeof...(Indices) == extents_type::rank()), int> = 0,
    std::enable_if_t<(... && std::is_convertible_v<Indices, index_type>), int> = 0>
  constexpr const_reference operator() (Indices... indices) const
  {
    return base_type::operator[](std::array<index_type,extents_type::rank()>{index_type(indices)...});
  }

  /**
   * \brief Access specified element at position (i0,i1,...) with mutable access
   * \throws std::out_of_range if the indices are out of the index space `[0,extent_0)x...x[0,extent_{r-1})`.
   */
  template <class... Indices,
    std::enable_if_t<(sizeof...(Indices) == extents_type::rank()), int> = 0,
    std::enable_if_t<(... && std::is_convertible_v<Indices, index_type>), int> = 0>
  constexpr reference at (Indices... indices)
  {
    if (not indexInIndexSpace(indices...))
      throw std::out_of_range("Indices out of index space");
    return base_type::operator[](std::array<index_type,extents_type::rank()>{index_type(indices)...});
  }

  /**
   * \brief Access specified element at position (i0,i1,...) with const access
   * \throws std::out_of_range if the indices are out of the index space `[0,extent_0)x...x[0,extent_{r-1})`.
   */
  template <class... Indices,
    std::enable_if_t<(sizeof...(Indices) == extents_type::rank()), int> = 0,
    std::enable_if_t<(... && std::is_convertible_v<Indices, index_type>), int> = 0>
  constexpr const_reference at (Indices... indices) const
  {
    if (not indexInIndexSpace(indices...))
      throw std::out_of_range("Indices out of index space");
    return base_type::operator[](std::array<index_type,extents_type::rank()>{index_type(indices)...});
  }

  /// \brief Access element at position [{i0,i1,...}]
  template <class Index,
    std::enable_if_t<std::is_convertible_v<const Index&, index_type>, int> = 0>
  constexpr reference operator[] (const std::array<Index,extents_type::rank()>& indices)
  {
    return base_type::operator[](indices);
  }

  /// \brief Access element at position [{i0,i1,...}]
  template <class Index,
    std::enable_if_t<std::is_convertible_v<const Index&, index_type>, int> = 0>
  constexpr const_reference operator[] (const std::array<Index,extents_type::rank()>& indices) const
  {
    return base_type::operator[](indices);
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


  /// \name Size information
  /// @{

  /// \brief Number of rows of a rank-2 tensor
  template <class E = extents_type,
    std::enable_if_t<(E::rank() == 2), int> = 0>
  constexpr index_type rows () const noexcept
  {
    return asBase().extent(0);
  }

  /// \brief Number of columns of a rank-2 tensor
  template <class E = extents_type,
    std::enable_if_t<(E::rank() == 2), int> = 0>
  constexpr index_type cols () const noexcept
  {
    return asBase().extent(1);
  }

  /// @}


  /// \name Conversion to the underlying value if rank is zero
  // @{

  template <class ScalarType, class E = extents_type, class D = derived_type,
    std::enable_if_t<std::is_convertible_v<value_type,ScalarType>, int> = 0,
    std::enable_if_t<(E::rank() == 0), int> = 0>
  constexpr operator ScalarType () const noexcept
  {
    return ScalarType(base_type::operator[](std::array<index_type,0>{}));
  }

  /// @}


  /// \brief Comparison of two DenseTensors for equality
  friend constexpr bool operator== (const DenseTensor& lhs, const DenseTensor& rhs) noexcept
  {
    return static_cast<const base_type&>(lhs) == static_cast<const base_type&>(rhs);
  }

private:
  // Check whether a tuple of indices is in the index space `[0,extent_0)x...x[0,extent_{r-1})`.
  template <class... Indices>
  [[nodiscard]] constexpr bool indexInIndexSpace (Indices... indices) const noexcept
  {
    return unpackIntegerSequence([&](auto... i) {
      return ( (0 <= indices && index_type(indices) < asBase().extent(i)) && ... );
    }, std::make_index_sequence<sizeof...(Indices)>{});
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

  base_type const& asBase () const
  {
    return static_cast<base_type const&>(*this);
  }

  base_type& asBase ()
  {
    return static_cast<base_type&>(*this);
  }
};

// specialization for rank-0 tensor and comparison with scalar
template <class D, class B, class V, class E = typename B::extents_type,
  std::enable_if_t<(E::rank() == 0), int> = 0,
  std::enable_if_t<std::is_convertible_v<V, typename B::value_type>, int> = 0,
  std::enable_if_t<std::is_constructible_v<typename B::value_type, V>, int> = 0>
constexpr bool operator== (const DenseTensor<D,B>& lhs, const V& rhs) noexcept
{
  return lhs() == rhs;
}

// specialization for rank-0 tensor and comparison with scalar
template <class V, class D, class B, class E = typename B::extents_type,
  std::enable_if_t<(E::rank() == 0), int> = 0,
  std::enable_if_t<std::is_convertible_v<V, typename B::value_type>, int> = 0,
  std::enable_if_t<std::is_constructible_v<typename B::value_type, V>, int> = 0>
constexpr bool operator== (const V& lhs, const DenseTensor<D,B>& rhs) noexcept
{
  return lhs == rhs();
}

template <class D, class B>
struct FieldTraits< DenseTensor<D,B> >
{
  using field_type = typename FieldTraits<D>::field_type;
  using real_type = typename FieldTraits<D>::real_type;
};

} // end namespace Dune

#endif // DUNE_COMMON_DENSETENSOR_HH
