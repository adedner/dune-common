// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_FIELDTENSOR_HH
#define DUNE_COMMON_FIELDTENSOR_HH

#include <array>
#include <type_traits>

#include <dune/common/densetensor.hh>
#include <dune/common/ftraits.hh>
#include <dune/common/initializerlist.hh>
#include <dune/common/std/extents.hh>

namespace Dune {

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
class FieldTensor
    : public DenseTensor<Value,Std::extents<int,exts...>,std::array<Value,(exts * ... * 1)>>
{
  using base_type = DenseTensor<Value,Std::extents<int,exts...>,std::array<Value,(exts * ... * 1)>>;

public:
  using value_type = typename base_type::value_type;
  using extents_type = typename base_type::extents_type;

public:
  /// Inherit constructor from DenseTensor
  using base_type::base_type;

  explicit FieldTensor (const value_type& value)
    : base_type{extents_type{}, value}
  {}

  friend constexpr bool operator== (const FieldTensor& lhs, const FieldTensor& rhs) noexcept
  {
    return static_cast<const base_type&>(lhs) == static_cast<const base_type&>(rhs);
  }
};

// specialization for rank-0 tensor and comparison with scalar
template <class V>
constexpr bool operator== (const FieldTensor<V>& lhs, const V& rhs) noexcept
{
  return *lhs.container_data() == rhs;
}

// specialization for rank-0 tensor and comparison with scalar
template <class V>
constexpr bool operator== (const V& lhs, const FieldTensor<V>& rhs) noexcept
{
  return lhs == *rhs.container_data();
}

template <class V, int... exts>
struct FieldTraits< FieldTensor<V,exts...> >
{
  using field_type = typename FieldTraits<V>::field_type;
  using real_type = typename FieldTraits<V>::real_type;
};

} // end namespace Dune

#endif // DUNE_COMMON_FIELDTENSOR_HH
