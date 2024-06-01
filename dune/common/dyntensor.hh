// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_DYNAMICTENSOR_HH
#define DUNE_COMMON_DYNAMICTENSOR_HH

#include <type_traits>

#include <dune/common/densetensor.hh>
#include <dune/common/ftraits.hh>
#include <dune/common/initializerlist.hh>
#include <dune/common/std/extents.hh>
#include <dune/common/std/layout_right.hh>
#include <dune/common/std/mdarray.hh>

namespace Dune {

// forward declaration
template <class Value, std::size_t rank>
class DynamicTensor;

template <class V, std::size_t ran>
struct FieldTraits< DynamicTensor<V,ran> >
{
  using field_type = typename FieldTraits<V>::field_type;
  using real_type = typename FieldTraits<V>::real_type;
};

namespace Impl {

template <class Value, std::size_t rank>
struct DynamicTensorTraits
{
  using element_type = Value;
  using extents_type = Std::dextents<int, rank>;
  using layout_type = Std::layout_right;
};

template <class Value, std::size_t rank>
struct DynamicTensorBase
{
  using Traits = DynamicTensorTraits<Value,rank>;
  using Container = std::vector<Value>;
  using type = Std::mdarray<Value,typename Traits::extents_type,typename Traits::layout_type,Container>;
};

template <class Value, std::size_t rank>
struct DynamicTensorInterface
{
  using Self = DynamicTensor<Value,rank>;
  using Traits = DynamicTensorTraits<Value,rank>;
  using type = DenseTensor<Self,Traits>;
};

} // end namespace Impl


/**
 * \brief A tensor of dynamic extents
 *
 * \nosubgrouping
 *
 * This class is a specialization of `DenseTensor` for purely dynamic extents
 * with fixed right-first layout and a corresponding container of dynamic capacity.
 *
 * \tparam Value  The element type stored in the tensor
 * \tparam rank   The tensorial rank, must be a non-negative integer.
 **/
template <class Value, std::size_t rank>
class DynamicTensor
    : public Impl::DynamicTensorBase<Value,rank>::type
    , public Impl::DynamicTensorInterface<Value,rank>::type
{
  using self_type = DynamicTensor;
  using base_type = typename Impl::DynamicTensorBase<Value,rank>::type;
  using interface_type = typename Impl::DynamicTensorInterface<Value,rank>::type;

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

  /// \name Additional DynamicTensor constructors
  /// @{

  constexpr DynamicTensor () = default;

  /// \brief Construct from the individual extents; sets all entries to zero
  template <class... IndexTypes,
    std::enable_if_t<(sizeof...(IndexTypes) == rank), int> = 0,
    std::enable_if_t<(... && std::is_convertible_v<IndexTypes,index_type>), int> = 0,
    std::enable_if_t<std::is_constructible_v<extents_type,IndexTypes...>, int> = 0>
  explicit constexpr DynamicTensor (IndexTypes... exts)
    : base_type{extents_type(std::move(exts)...), value_type(0)}
  {}

  /// \brief Constructor from extents; sets all entries to zero
  constexpr DynamicTensor (const extents_type& e)
    : base_type{e, value_type(0)}
  {}

  /// \brief Construct from the individual extents and an initial value
  template <class... IndexTypes, class V,
    std::enable_if_t<(sizeof...(IndexTypes) == rank), int> = 0,
    std::enable_if_t<(... && std::is_convertible_v<IndexTypes,index_type>), int> = 0,
    std::enable_if_t<std::is_constructible_v<extents_type,IndexTypes...>, int> = 0,
    std::enable_if_t<std::is_convertible_v<V,value_type>, int> = 0>
  explicit constexpr DynamicTensor (IndexTypes... exts, const V& value)
    : base_type{extents_type(std::move(exts)...), value}
  {}

  /// \brief Constructor from extents and initial value
  template <class V,
    std::enable_if_t<std::is_convertible_v<V,value_type>, int> = 0>
  explicit constexpr DynamicTensor (const extents_type& e, const V& value)
    : base_type{e, value}
  {}

  /// \brief Constructor from extents and brace-init list of values
  constexpr DynamicTensor (const extents_type& e, NestedInitializerList_t<value_type,extents_type::rank()> init)
    : base_type{mapping_type{e}}
  {
    auto it = this->container_data();
    InitializerList<value_type,extents_type>::apply(init,this->extents(),
      [&it](value_type value) { *it++ = value; });
  }

  /// \brief Converting constructor from mdarray
  constexpr DynamicTensor (const base_type& base)
    : base_type{base}
  {}

  /// \brief Converting constructor from mdarray
  constexpr DynamicTensor (base_type&& base)
    : base_type{std::move(base)}
  {}

  /// @}


  /// \name Assignment operators
  /// @{

  /// \brief Copy assignment operator
  constexpr self_type& operator= (const self_type&) = default;

  /// \brief Copy assignment from a different mdarray
  template <class V, class E, class L, class C,
    std::enable_if_t<std::is_constructible_v<base_type, Std::mdarray<V,E,L,C>>, int> = 0>
  constexpr self_type& operator= (const Std::mdarray<V,E,L,C>& other)
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
     DynamicTensor<double,2> matrix(3,3);
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
     DynamicTensor<double,3> tensor(3,3,3);
     tensor(0,1,2) = 42.0;
     \endcode
   **/
  using interface_type::operator();

  /// @}


  /// \name Modifiers
  /// @{

  /// \brief Change the extents of the tensor and resize the underlying container with given default value
  template <class V,
    std::enable_if_t<std::is_convertible_v<V,value_type>, int> = 0>
  void resize (const extents_type& e, const V& value)
  {
    auto container = std::move(*this).extract_container();
    auto m = mapping_type{e};
    container.resize(m.required_span_size(), value);
    static_cast<base_type&>(*this) = base_type{m, std::move(container)};
  }

  /// \brief Change the extents of the tensor and resize the underlying container
  void resize (const extents_type& e)
  {
    resize(e, value_type(0));
  }

  /// \brief Change the extents of the tensor by the given individual extents
  template <class... IndexTypes, class V,
    std::enable_if_t<(sizeof...(IndexTypes) == rank), int> = 0,
    std::enable_if_t<(... && std::is_convertible_v<IndexTypes,index_type>), int> = 0,
    std::enable_if_t<std::is_constructible_v<extents_type,IndexTypes...>, int> = 0,
    std::enable_if_t<std::is_convertible_v<V,value_type>, int> = 0>
  void resize (IndexTypes... exts, const V& v)
  {
    resize(extents_type{exts...}, v);
  }

  /// \brief Change the extents of the tensor by the given individual extents
  template <class... IndexTypes,
    std::enable_if_t<(sizeof...(IndexTypes) == rank), int> = 0,
    std::enable_if_t<(... && std::is_convertible_v<IndexTypes,index_type>), int> = 0,
    std::enable_if_t<std::is_constructible_v<extents_type,IndexTypes...>, int> = 0>
  void resize (IndexTypes... exts)
  {
    resize(extents_type{exts...}, value_type(0));
  }

  /// @}

  friend constexpr bool operator== (const DynamicTensor& lhs, const DynamicTensor& rhs) noexcept
  {
    return static_cast<const base_type&>(lhs) == static_cast<const base_type&>(rhs);
  }
};

// specialization for rank-0 tensor and comparison with scalar
template <class V>
constexpr bool operator== (const DynamicTensor<V,0>& lhs, const V& rhs) noexcept
{
  return *lhs.container_data() == rhs;
}

// specialization for rank-0 tensor and comparison with scalar
template <class V>
constexpr bool operator== (const V& lhs, const DynamicTensor<V,0>& rhs) noexcept
{
  return lhs == *rhs.container_data();
}


} // end namespace Dune

#endif // DUNE_COMMON_DYNAMICTENSOR_HH
