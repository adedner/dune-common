// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_DYNAMICTENSOR_HH
#define DUNE_COMMON_DYNAMICTENSOR_HH

#include <type_traits>
#include <utility>
#include <vector>

#include <dune/common/densetensor.hh>
#include <dune/common/ftraits.hh>
#include <dune/common/std/extents.hh>
#include <dune/common/std/mdarray.hh>

namespace Dune {

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
 * \tparam Allocator  An allocator that is used to acquire/release memory and to
 *                    construct/destroy the elements in that memory.
 **/
template <class Value, std::size_t rank, class Allocator = std::allocator<Value>>
class DynamicTensor
    : public DenseTensor<Value,Std::dextents<int,rank>,std::vector<Value,Allocator>>
{
  using self_type = DynamicTensor;
  using base_type = DenseTensor<Value,Std::dextents<int,rank>,std::vector<Value>>;

public:
  using value_type = typename base_type::value_type;
  using extents_type = typename base_type::extents_type;
  using index_type = typename base_type::index_type;
  using mapping_type = typename base_type::mapping_type;

public:
  /// Inherit the constructors from DenseTensor
  using base_type::base_type;

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


template <class V, std::size_t ran>
struct FieldTraits< DynamicTensor<V,ran> >
{
  using field_type = typename FieldTraits<V>::field_type;
  using real_type = typename FieldTraits<V>::real_type;
};

} // end namespace Dune

#endif // DUNE_COMMON_DYNAMICTENSOR_HH
