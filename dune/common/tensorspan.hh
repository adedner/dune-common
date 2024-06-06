// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_TENSORSPAN_HH
#define DUNE_COMMON_TENSORSPAN_HH

#include <type_traits>

#include <dune/common/ftraits.hh>
#include <dune/common/std/default_accessor.hh>
#include <dune/common/std/extents.hh>
#include <dune/common/std/layout_right.hh>
#include <dune/common/std/mdspan.hh>

namespace Dune {

// forward declaration
template <class Derived, class Traits>
class TensorInterface;

// forward declrataion
template <class Element, class Extents,
          class LayoutPolicy = Std::layout_right,
          class AccessorPolicy = Std::default_accessor<Element>>
class TensorSpan;

template <class Element, class Extents, class Layout, class Accessor>
struct FieldTraits< TensorSpan<Element,Extents,Layout,Accessor> >
{
  using field_type = typename FieldTraits<Element>::field_type;
  using real_type = typename FieldTraits<Element>::real_type;
};

namespace Impl {

template <class Element, class Extents, class Layout>
struct TensorSpanTraits
{
  using element_type = Element;
  using extents_type = Extents;
  using layout_type = Layout;
};

} // end namespace Impl


/**
 * \brief A multi-dimensional span with tensor interface.
 * \nosubgrouping
 *
 * \tparam Element  The element type stored in the tensor.
 * \tparam Extents  The type representing the tensor extents. Must be compatible
 *                  to `Dune::Tensor::Extents`.
 * \tparam Layout   A class providing the index mapping from the multi-dimensional
 *                  index space to a single index usable in the `Container`. See
 *                  the `Dune::Concept::Layout` concept.
 * \tparam Accessor A class that defines types and operations to create a reference
 *                  to a single element stored by a data pointer and index.
 **/
template <class Element, class Extents, class LayoutPolicy, class AccessorPolicy>
class TensorSpan
    : public Std::mdspan<Element,Extents,LayoutPolicy,AccessorPolicy>
    , public TensorInterface<TensorSpan<Element,Extents,LayoutPolicy,AccessorPolicy>,
        Impl::TensorSpanTraits<Element,Extents,LayoutPolicy>>
{
  using self_type = TensorSpan;
  using base_type = Std::mdspan<Element,Extents,LayoutPolicy,AccessorPolicy>;
  using interface_type = TensorInterface<self_type,Impl::TensorSpanTraits<Element,Extents,LayoutPolicy>>;

public:
  using element_type =	Element;
  using extents_type = Extents;
  using layout_type = LayoutPolicy;
  using accessor_type = AccessorPolicy;

  using value_type = typename base_type::value_type;
  using mapping_type = typename base_type::mapping_type;
  using index_type = typename base_type::index_type;
  using size_type = typename base_type::size_type;
  using rank_type = typename base_type::rank_type;
  using data_handle_type = typename base_type::data_handle_type;
  using reference = typename base_type::reference;

public:
  /// \name TensorSpan constructors
  /// @{

  using base_type::base_type;

  /// \brief Converting constructor
  template <class V, class E, class L, class A,
    class MappingType = typename L::template mapping<E>,
    std::enable_if_t<std::is_constructible_v<mapping_type, const MappingType&>, int> = 0,
    std::enable_if_t<std::is_constructible_v<accessor_type, const A&>, int> = 0>
  constexpr TensorSpan (const TensorSpan<V,E,L,A>& other)
    : base_type{other}
  {}

  /// \brief Converting move constructor
  template <class V, class E, class L, class A,
    class MappingType = typename L::template mapping<E>,
    std::enable_if_t<std::is_constructible_v<mapping_type, const MappingType&>, int> = 0,
    std::enable_if_t<std::is_constructible_v<accessor_type, const A&>, int> = 0>
  constexpr TensorSpan (TensorSpan<V,E,L,A>&& tensor)
    : base_type{std::move(tensor)}
  {}

  /// \brief base copy constructor
  constexpr TensorSpan (const base_type& tensor)
    : base_type{tensor}
  {}

  /// \brief base move constructor
  constexpr TensorSpan (base_type&& tensor)
    : base_type{std::move(tensor)}
  {}

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


  /// \brief Access specified element at position (i0,i1,...) with const access
  template <class... Indices,
    std::enable_if_t<(sizeof...(Indices) == extents_type::rank()), int> = 0,
    std::enable_if_t<(... && std::is_convertible_v<Indices, index_type>), int> = 0>
  constexpr reference access (typename interface_type::accessor_type, Indices... indices) const
  {
    return base_type::operator[](std::array<index_type,extents_type::rank()>{index_type(indices)...});
  }
};

// deduction guides
// @{

template <class CArray,
  std::enable_if_t<std::is_array_v<CArray>, int> = 0,
  std::enable_if_t<(std::rank_v<CArray> == 1), int> = 0>
TensorSpan (CArray&)
  -> TensorSpan<std::remove_all_extents_t<CArray>, Std::extents<std::size_t, std::extent_v<CArray,0>>>;

template <class Pointer,
  std::enable_if_t<std::is_pointer_v<std::remove_reference_t<Pointer>>, int> = 0>
TensorSpan (Pointer&&)
  -> TensorSpan<std::remove_pointer_t<std::remove_reference_t<Pointer>>, Std::extents<std::size_t>>;

template <class ElementType, class... II,
  std::enable_if_t<(... && std::is_convertible_v<II,std::size_t>), int> = 0,
  std::enable_if_t<(sizeof...(II) > 0), int> = 0>
TensorSpan (ElementType*, II...)
  -> TensorSpan<ElementType, Std::dextents<std::size_t, sizeof...(II)>>;

template <class ElementType, class SizeType, std::size_t N>
TensorSpan (ElementType*, Std::span<SizeType,N>&)
  -> TensorSpan<ElementType, Std::dextents<std::size_t, N>>;

template <class ElementType, class SizeType, std::size_t N>
TensorSpan (ElementType*, const std::array<SizeType,N>&)
  -> TensorSpan<ElementType, Std::dextents<std::size_t, N>>;

template <class ElementType, class IndexType, std::size_t... exts>
TensorSpan (ElementType*, const Std::extents<IndexType,exts...>&)
  -> TensorSpan<ElementType, Std::extents<IndexType,exts...>>;

template <class ElementType, class Mapping,
  class Extents = typename Mapping::extents_type,
  class Layout = typename Mapping::layout_type>
TensorSpan (ElementType*, const Mapping&)
  -> TensorSpan<ElementType, Extents, Layout>;

template <class Mapping, class Accessor,
  class DataHandle = typename Accessor::data_handle_type,
  class Element = typename Accessor::element_type,
  class Extents = typename Mapping::extents_type,
  class Layout = typename Mapping::layout_type>
TensorSpan (const DataHandle&, const Mapping&, const Accessor&)
  -> TensorSpan<Element, Extents, Layout, Accessor>;

template <class V, class E, class L, class A>
TensorSpan (Std::mdspan<V,E,L,A>)
  -> TensorSpan<V,E,L,A>;

/// @}

} // end namespace Dune

#endif // DUNE_COMMON_TENSORSPAN_HH
