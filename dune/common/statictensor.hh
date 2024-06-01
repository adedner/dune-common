// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_STATICTENSOR_HH
#define DUNE_COMMON_STATICTENSOR_HH

#include <array>
#include <type_traits>

#include <dune/common/densetensor.hh>
#include <dune/common/ftraits.hh>
#include <dune/common/initializerlist.hh>
#include <dune/common/std/extents.hh>
#include <dune/common/std/layout_right.hh>
#include <dune/common/std/mdarray.hh>

namespace Dune {

// forward declaration
template <class Value, int... exts>
class StaticTensor;

template <class V, int... exts>
struct FieldTraits< StaticTensor<V,exts...> >
{
  using field_type = typename FieldTraits<V>::field_type;
  using real_type = typename FieldTraits<V>::real_type;
};

namespace Impl {

template <class Value, int... exts>
struct StaticTensorTraits
{
  using element_type = Value;
  using extents_type = Std::extents<int, std::size_t(exts)...>;
  using layout_type = Std::layout_right;
};

template <class Value, int... exts>
struct StaticTensorBase
{
  using Traits = StaticTensorTraits<Value,exts...>;
  using Container = std::array<Value,(exts * ... * 1)>;
  using type = Std::mdarray<Value,typename Traits::extents_type,typename Traits::layout_type,Container>;
};

template <class Value, int... exts>
struct StaticTensorInterface
{
  using Self = StaticTensor<Value,exts...>;
  using Traits = StaticTensorTraits<Value,exts...>;
  using type = DenseTensor<Self,Traits>;
};

} // end namespace Impl


/**
 * \brief A tensor of static extents
 *
 * \nosubgrouping
 *
 * This class is a specialization of `DenseTensor` for purely static extents
 * with fixed right-first layout and a corresponding container of static capacity.
 *
 * \tparam Value    The element type stored in the tensor.
 * \tparam exts...  The individual extents of the dimensions of the tensor.
 **/
template <class Value, int... exts>
class StaticTensor
    : public Impl::StaticTensorBase<Value,exts...>::type
    , public Impl::StaticTensorInterface<Value,exts...>::type
{
  using base_type = typename Impl::StaticTensorBase<Value,exts...>::type;
  using interface_type = typename Impl::StaticTensorInterface<Value,exts...>::type;

  static constexpr std::size_t rank_ = base_type::rank();

public:
  using value_type = Value;
  using extents_type = typename base_type::extents_type;
  using rank_type = typename base_type::rank_type;
  using index_type = typename base_type::index_type;
  using size_type = typename base_type::size_type;
  using mapping_type = typename base_type::mapping_type;
  using layout_type = typename base_type::layout_type;
  using reference = typename base_type::reference;
  using const_reference = typename base_type::const_reference;
  using container_type = typename base_type::container_type;

  /// \name StaticTensor constructors
  /// @{

  /// \brief Default constructor that initializes the tensor to zero
  constexpr StaticTensor () noexcept
    : StaticTensor{extents_type{}}
  {}

  /// \brief Constructor from extents; sets all entries to zero
  constexpr StaticTensor (const extents_type& e) noexcept
    : base_type{mapping_type{e}, container_type{}}
  {}

  /// \brief Constructor that sets all entries to the given `value`
  template <class V,
    std::enable_if_t<std::is_convertible_v<V,value_type>, int> = 0>
  explicit constexpr StaticTensor (const V& value) noexcept
    : base_type{extents_type{}, value_type(value)}
  {}

  /// \brief Constructor from extents and initial value
  template <class V,
    std::enable_if_t<std::is_convertible_v<V,value_type>, int> = 0>
  explicit constexpr StaticTensor (const extents_type& e, const V& value) noexcept
    : base_type{e, value_type(value)}
  {}

  /// \brief Constructor from a brace-init list of values
  constexpr StaticTensor (NestedInitializerList_t<value_type,rank_> init)
    : StaticTensor{extents_type{}, init}
  {}

  /// \brief Constructor from extents and brace-init list of values
  constexpr StaticTensor (const extents_type& e, NestedInitializerList_t<value_type,rank_> init)
    : base_type{mapping_type{e}}
  {
    auto it = this->container_data();
    InitializerList<value_type,extents_type>::apply(init,this->extents(),
      [&it](value_type value) { *it++ = value; });
  }

  /// \brief Converting constructor from mdarray
  constexpr StaticTensor (const base_type& base) noexcept
    : base_type{base}
  {}

  /// \brief Converting constructor from mdarray
  constexpr StaticTensor (base_type&& base) noexcept
    : base_type{std::move(base)}
  {}

  /// @}


  /// \name Assignment operators
  /// @{

  /// \brief Copy assignment operator
  constexpr StaticTensor& operator= (const StaticTensor&) = default;

  /// \brief Copy assignment from a different mdarray
  template <class V, class E, class L, class C,
    std::enable_if_t<std::is_constructible_v<base_type, Std::mdarray<V,E,L,C>>, int> = 0>
  constexpr StaticTensor& operator= (const Std::mdarray<V,E,L,C>& other)
  {
    return *this = base_type(other);
  }

  /// \brief Assignment of a scalar value
  using interface_type::operator=;

  /// @}


  /// \name Multi index access
  /// @{

  /**
   * \brief Nested subscript operator to access either a subdimension or an element.
   *
   * The bracket operator can be nested to successively access more dimensions.
   * Each subscript operator returns an mdspan over the sub-tensor with fixed
   * first index.
   *
   * \b Examples:
   * \code{c++}
     StaticTensor<double,3,3> matrix;
     auto row = matrix[0]; // 0th row
     row[1] = 56;          // the row is an mdspan
     matrix[0][1] = 42;    // element at (0,1)
     matrix[0,1] = 7.0;    // only with c++23
     \endcode
   **/
  using base_type::operator[];
  using interface_type::operator[];

  /**
   * \brief Access an element of the tensor using a variadic list of indices.
   * \b Examples:
   * \code{c++}
     StaticTensor<double,3,3,3> tensor;
     tensor(0,1,2) = 42.0;
     \endcode
   **/
  using interface_type::operator();

  /// @}

  friend constexpr bool operator== (const StaticTensor& lhs, const StaticTensor& rhs) noexcept
  {
    return static_cast<const base_type&>(lhs) == static_cast<const base_type&>(rhs);
  }
};

// specialization for rank-0 tensor and comparison with scalar
template <class V>
constexpr bool operator== (const StaticTensor<V>& lhs, const V& rhs) noexcept
{
  return *lhs.container_data() == rhs;
}

// specialization for rank-0 tensor and comparison with scalar
template <class V>
constexpr bool operator== (const V& lhs, const StaticTensor<V>& rhs) noexcept
{
  return lhs == *rhs.container_data();
}

} // end namespace Dune

#endif // DUNE_COMMON_STATICTENSOR_HH
