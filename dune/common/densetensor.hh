// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_DENSETENSOR_HH
#define DUNE_COMMON_DENSETENSOR_HH

#include <array>
#include <cassert>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include <dune/common/indices.hh>
#include <dune/common/initializerlist.hh>
#include <dune/common/tensorinterface.hh>
#include <dune/common/tensorspan.hh>
#include <dune/common/std/default_accessor.hh>
#include <dune/common/std/extents.hh>
#include <dune/common/std/layout_right.hh>
#include <dune/common/std/mdarray.hh>
#include <dune/common/std/mdspan.hh>
#include <dune/common/std/type_traits.hh>

namespace Dune {

namespace Impl {

template <class Element, class Extents>
struct DenseTensorTraits
{
  using element_type = Element;
  using extents_type = Extents;
  using layout_type = Std::layout_right;
};

} // end namespace Impl


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
template <class Element, class Extents, class Container>
class DenseTensor
    : public Std::mdarray<Element,Extents,Std::layout_right,Container>
    , public TensorInterface<DenseTensor<Element,Extents,Container>,
        Impl::DenseTensorTraits<Element,Extents>>
{
  using self_type = DenseTensor;
  using base_type = Std::mdarray<Element,Extents,Std::layout_right,Container>;
  using interface_type = TensorInterface<self_type, Impl::DenseTensorTraits<Element,Extents>>;

public:
  using element_type = typename base_type::element_type;
  using extents_type = typename base_type::extents_type;
  using layout_type = typename base_type::layout_type;
  using container_type = typename base_type::container_type;
  using value_type = typename base_type::value_type;
  using mapping_type = typename base_type::mapping_type;
  using index_type = typename base_type::index_type;
  using size_type = typename base_type::size_type;
  using rank_type = typename base_type::rank_type;
  using mdspan_type = typename base_type::mdspan_type;
  using const_mdspan_type = typename base_type::const_mdspan_type;
  using pointer = typename base_type::pointer;
  using reference = typename base_type::reference;
  using const_pointer = typename base_type::const_pointer;
  using const_reference = typename base_type::const_reference;

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
  template <class V, class E, class C,
    std::enable_if_t<std::is_constructible_v<base_type, typename DenseTensor<V,E,C>::base_type>, int> = 0>
  constexpr DenseTensor (const DenseTensor<V,E,C>& other)
    : base_type{other}
  {}

  /// @}


  /// \name Assignment operators
  /// @{

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


  /// \brief Access specified element at position (i0,i1,...) with mutable access
  template <class... Indices,
    std::enable_if_t<(sizeof...(Indices) == extents_type::rank()), int> = 0,
    std::enable_if_t<(... && std::is_convertible_v<Indices, index_type>), int> = 0>
  constexpr reference access (typename interface_type::accessor_type, Indices... indices)
  {
    return base_type::operator[](std::array<index_type,extents_type::rank()>{index_type(indices)...});
  }

  /// \brief Access specified element at position (i0,i1,...) with const access
  template <class... Indices,
    std::enable_if_t<(sizeof...(Indices) == extents_type::rank()), int> = 0,
    std::enable_if_t<(... && std::is_convertible_v<Indices, index_type>), int> = 0>
  constexpr const_reference access (typename interface_type::accessor_type, Indices... indices) const
  {
    return base_type::operator[](std::array<index_type,extents_type::rank()>{index_type(indices)...});
  }
};

} // end namespace Dune

#endif // DUNE_COMMON_DENSETENSOR_HH
