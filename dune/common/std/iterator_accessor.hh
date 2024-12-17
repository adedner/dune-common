// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_STD_ITERATOR_ACCESSOR_HH
#define DUNE_COMMON_STD_ITERATOR_ACCESSOR_HH

#include <iterator>

namespace Dune::Std {

/**
 * \brief A type for access to elements of mdspan using iterators as data handle.
 * \ingroup CxxUtilities
 *
 * The `iterator_accessor` class template is an `AccessorPolicy` used by `mdspan`
 * to provide access to elements in an iterator range [begin, end). The iterator
 * thereby acts as a data handle.
 *
 * \tparam Iterator  The iterator type should be conforming with the std iterator concept.
 */
template <class Iterator>
class iterator_accessor
{
public:
  using element_type = typename std::iterator_traits<Iterator>::value_type;
  using data_handle_type = Iterator;
  using reference = typename std::iterator_traits<Iterator>::reference;
  using offset_policy = iterator_accessor;

public:
  /// \brief Default constructor
  constexpr iterator_accessor () noexcept = default;

  /// \brief Return a reference to the i'th element in the data range starting at `p`
  constexpr reference access (data_handle_type p, std::size_t i) const noexcept
  {
    return *std::next(p,i);
  }

  /// \brief Return a data handle to the i'th element in the data range starting at `p`
  constexpr data_handle_type offset (data_handle_type p, std::size_t i) const noexcept
  {
    return std::next(p,i);
  }
};

} // end namespace Dune::Std

#endif // DUNE_COMMON_STD_ITERATOR_ACCESSOR_HH
