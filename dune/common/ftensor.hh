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
#include <dune/common/std/mdarray.hh>

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
    : public DenseTensor<FieldTensor<Value,exts...>,
        Std::mdarray<Value,Std::extents<int,exts...>,Std::layout_right,std::array<Value,(exts * ... * 1)>>>
{
  using self_type = FieldTensor;
  using storage_type = Std::mdarray<Value,Std::extents<int,exts...>,Std::layout_right,
    std::array<Value,(exts * ... * 1)>>;
  using base_type = DenseTensor<self_type,storage_type>;

public:
  using extents_type = typename base_type::extents_type;
  using value_type = typename base_type::value_type;

public:
  /// \brief Inherit constructor from DenseTensor
  using base_type::base_type;

  /// \name Additional FieldTensor constructors
  /// @{

  explicit FieldTensor (const value_type& value)
    : base_type{extents_type{}, value}
  {}

  /// @}
};

template <class V, int... exts>
struct FieldTraits< FieldTensor<V,exts...> >
{
  using field_type = typename FieldTraits<V>::field_type;
  using real_type = typename FieldTraits<V>::real_type;
};

} // end namespace Dune

#endif // DUNE_COMMON_FIELDTENSOR_HH
