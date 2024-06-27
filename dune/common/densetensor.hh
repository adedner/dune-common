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
#include <dune/common/iteratorfacades.hh>
#include <dune/common/rangeutilities.hh>
#include <dune/common/tensorspan.hh>
#include <dune/common/std/default_accessor.hh>
#include <dune/common/std/extents.hh>
#include <dune/common/std/layout_left.hh>
#include <dune/common/std/layout_right.hh>
#include <dune/common/std/layout_stride.hh>
#include <dune/common/std/type_traits.hh>

namespace Dune {
namespace Impl {

template <class B>
class SubTensorAccess;

template <class A>
class DenseTensorIterator;

} // end namespace Impl


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

private:
  using SubTensorAccess = Impl::SubTensorAccess<base_type>;
  using ConstSubTensorAccess = Impl::SubTensorAccess<const base_type>;

public:
  using iterator = Impl::DenseTensorIterator<SubTensorAccess>;
  using const_iterator = Impl::DenseTensorIterator<ConstSubTensorAccess>;

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

  /**
   * \brief Access specified element or sub-tensor at position [i0] with mutable access.
   *
   * This returns either a reference to the `index`th element if rank == 1,
   * otherwise it returns a sub-Tensorspace with fixed first index.
   *
   * \throws std::domain_error if rank is zero.
   */
  constexpr decltype(auto) operator[] (index_type index) noexcept
  {
    return SubTensorAccess{&asBase()}(index);
  }

  /**
   * \brief Access specified element or sub-tensor at position [i0] with const access.
   *
   * This returns either a const_reference to the `index`th element if rank == 1,
   * otherwise it returns a sub-Tensorspace with fixed first index.
   *
   * \throws std::domain_error if rank is zero.
   */
  constexpr decltype(auto) operator[] (index_type index) const noexcept
  {
    return ConstSubTensorAccess{&asBase()}(index);
  }

  /// \brief Access element at position [{i0,i1,...}]
  template <class Index,
    std::enable_if_t<std::is_convertible_v<const Index&, index_type>, int> = 0>
  constexpr reference operator[] (const std::array<Index,extents_type::rank()>& indices)
  {
    return std::apply([&](auto... ii) -> reference {
      return base_type::operator[](indices); }, indices);
  }

  /// \brief Access element at position [{i0,i1,...}]
  template <class Index,
    std::enable_if_t<std::is_convertible_v<const Index&, index_type>, int> = 0>
  constexpr const_reference operator[] (const std::array<Index,extents_type::rank()>& indices) const
  {
    return std::apply([&](auto... ii) -> const_reference {
      return base_type::operator[](indices); }, indices);
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


  /// \name Iterators
  /// @{

  constexpr iterator begin () noexcept { return iterator(0, &asBase()); }
  constexpr const_iterator begin () const noexcept { return const_iterator(0, &asBase()); }
  constexpr const_iterator cbegin () const noexcept { return const_iterator(0, &asBase()); }

  constexpr iterator end () noexcept { return iterator(asBase().extent(0), &asBase()); }
  constexpr const_iterator end () const noexcept { return const_iterator(asBase().extent(0), &asBase()); }
  constexpr const_iterator cend () const noexcept { return const_iterator(asBase().extent(0), &asBase()); }

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

namespace Impl {

template <class B, class S>
class SubTensorAccessImpl;

// specialization for rank == 1
template <class B, std::size_t i0>
class SubTensorAccessImpl<B, std::index_sequence<i0>>
{
public:
  using base_type = B;
  using index_type = typename base_type::index_type;

public:
  constexpr SubTensorAccessImpl () = default;
  constexpr SubTensorAccessImpl (base_type* base) noexcept
    : base_(base)
  {}

  constexpr decltype(auto) operator() (index_type index) const
  {
    return (*base_)[index];
  }

private:
  base_type* base_ = nullptr;
};

// specialization for rank >= 2
template <class B, std::size_t i0, std::size_t i1, std::size_t... ii>
class SubTensorAccessImpl<B, std::index_sequence<i0,i1,ii...>>
{
private:
  template <class D>
  using HasContainerData = decltype(std::declval<D>().container_data());

  template <class D>
  using HasAccessorAndDataHandle = decltype(std::declval<D>().accessor(), std::declval<D>().data_handle());

public:
  using base_type = B;
  using extents_type = typename base_type::extents_type;
  using layout_type = typename base_type::layout_type;
  using mapping_type = typename base_type::mapping_type;
  using index_type = typename base_type::index_type;

public:
  constexpr SubTensorAccessImpl () = default;
  constexpr SubTensorAccessImpl (base_type* base) noexcept
    : base_(base)
  {}

  static constexpr auto extents (const extents_type& originalExtents)
  {
    using E = Std::extents<index_type, extents_type::static_extent(i1),extents_type::static_extent(ii)...>;
    return E{originalExtents.extent(i1),originalExtents.extent(ii)...};
  }

  static constexpr auto mapping (Std::layout_right, const mapping_type& originalMapping)
  {
    return Std::layout_right::mapping{extents(originalMapping.extents())};
  }

  static constexpr auto mapping (Std::layout_left, const mapping_type& originalMapping)
  {
    return Std::layout_stride::mapping{extents(originalMapping.extents()),
      std::array{originalMapping.stride(i1), originalMapping.stride(ii)...} };
  }

  static constexpr index_type offset (const mapping_type& originalMapping, index_type index)
  {
    return originalMapping(index, 0, (void(ii),0)...);
  }

  constexpr auto operator() (index_type index) const
  {
    assert(0 <= index && index < base_->extent(0));
    auto m = mapping(layout_type{}, base_->mapping());
    if constexpr (Std::is_detected_v<HasContainerData,B>) {
      auto* data_handle = base_->container_data() + offset(base_->mapping(),index);
      return TensorSpan{data_handle, std::move(m)};
    }
    else if constexpr (Std::is_detected_v<HasAccessorAndDataHandle,B>) {
      auto data_handle = base_->accessor().offset(base_->data_handle(), offset(base_->mapping(),index));
      using accessor_type = typename base_type::accessor_type::offset_policy;
      return TensorSpan{data_handle, std::move(m), accessor_type{}};
    }
    else {
      throw std::runtime_error("NotImplemented: Tensor must be derived from mdarray or mdspan.");
      return 0;
    }
  }

private:
  base_type* base_ = nullptr;
};

template <class B>
class SubTensorAccess
    : public SubTensorAccessImpl<B, std::make_index_sequence<B::rank()>>
{
  using self_type = SubTensorAccess;
  using base_type = SubTensorAccessImpl<B, std::make_index_sequence<B::rank()>>;

public:
  using base_type::base_type;
  using base_type::operator();
};


template <class A>
struct DenseTensorIteratorTraits
{
  using accessor_type = A;
  using index_type = typename A::index_type;
  using reference = decltype(std::declval<accessor_type>()(std::declval<index_type>()));
  using value_type = std::decay_t<reference>;
  using pointer = ProxyArrowResult<reference>;
  using facade_type = IteratorFacade<DenseTensorIterator<A>, std::random_access_iterator_tag,
    value_type, reference, pointer>;
};


template <class A>
class DenseTensorIterator
    : public DenseTensorIteratorTraits<A>::facade_type
{
  using accessor_type = A;
  using traits_type = DenseTensorIteratorTraits<A>;
  using facade_type = typename traits_type::facade_type;

public:
  using reference = typename facade_type::reference;
  using index_type = typename traits_type::index_type;

public:
  constexpr DenseTensorIterator () = default;

  template <class... Args,
    std::enable_if_t<std::is_constructible_v<accessor_type,Args...>, int> = 0>
  constexpr explicit DenseTensorIterator (index_type pos, Args&&... args) noexcept
    : pos_(pos)
    , access_(std::forward<Args>(args)...)
  {}

  constexpr index_type index () const noexcept { return pos_; }
  constexpr reference operator* () const { return access_(pos_); }

private:
  constexpr index_type& baseIterator() noexcept { return pos_; }
  constexpr const index_type& baseIterator() const noexcept { return pos_; }

private:
  index_type pos_ = 0;
  accessor_type access_ = {};

  friend IteratorFacadeAccess;
};

} // end namespace Impl

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
