// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_REGULAR_OPTIONAL_HH
#define DUNE_COMMON_REGULAR_OPTIONAL_HH

#include <optional>
#include <type_traits>

#include <dune/common/copyableoptional.hh>

namespace Dune {

/**
 * \brief A regular type wrapper that provides copy/move assignment
 * operations for types that are only copy/move constructible and a default
 * constructor for types that are not itself default constructible.
 *
 * The class is an extension of the CopyableOptional, with the addition
 * of a default constructor.
 *
 * \tparam Type  The type to be wrapped and equipped with copy operations.
 *
 * \b Requirements: `Type` must be an object type that is copy-constructible
 **/
template <class Type>
class RegularOptional
    : public CopyableOptional<Type>
{
  using Base = CopyableOptional<Type>;

public:

  using Base::Base;

  /**
   * \brief Implementation of a default constructor, if the `Type` is itself
   * not default constructible. The internal optional then contains `std::nullopt`
   **/
  template <class T = Type,
    std::enable_if_t<not std::is_default_constructible_v<T>, int> = 0>
  constexpr RegularOptional () noexcept
    : Base{typename Base::derived_constructor{}, std::nullopt}
  {}

  /**
   * \brief Implementation of a default constructor, if the `Type` is itself
   * default constructible. The internal optional contains a value-initialized `Type`.
   **/
  template <class T = Type,
    std::enable_if_t<std::is_default_constructible_v<T>, int> = 0>
  constexpr RegularOptional ()
        noexcept(std::is_nothrow_default_constructible_v<T>)
    : Base{}
  {}
};

} // end namespace Dune

#endif // DUNE_COMMON_REGULAR_OPTIONAL_HH
