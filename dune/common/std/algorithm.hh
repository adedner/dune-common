// SPDX-FileCopyrightText: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_COMMON_STD_ALGORITHM_HH
#define DUNE_COMMON_STD_ALGORITHM_HH

#include <algorithm>
#include <compare>
#include <type_traits>

namespace Dune::Std {
namespace Impl {

struct CompareThreeWay
{
  template <class T, class U>
  constexpr auto operator() (T&& t, U&& u) const
  {
    return std::forward<T>(t) <=> std::forward<U>(u);
  }
};

}

/**
 * \brief Lexicographically compares two ranges `[first1, last1)` and `[first2, last2)`
 * using three-way comparison and produces a result of the strongest applicable
 * comparison category type.
 *
 * Implementation taken from https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare_three_way
 */
template <class I1, class I2, class Cmp = Impl::CompareThreeWay>
constexpr auto lexicographical_compare_three_way(I1 f1, I1 l1, I2 f2, I2 l2, Cmp comp = {})
    -> decltype(comp(*f1, *f2))
{
  using ret_t = decltype(comp(*f1, *f2));
  static_assert(std::disjunction_v<
                    std::is_same<ret_t, std::strong_ordering>,
                    std::is_same<ret_t, std::weak_ordering>,
                    std::is_same<ret_t, std::partial_ordering>>,
                "The return type must be a comparison category type.");

  bool exhaust1 = (f1 == l1);
  bool exhaust2 = (f2 == l2);
  for (; !exhaust1 && !exhaust2; exhaust1 = (++f1 == l1), exhaust2 = (++f2 == l2))
    if (auto c = comp(*f1, *f2); c != 0)
      return c;

  return !exhaust1 ? std::strong_ordering::greater:
         !exhaust2 ? std::strong_ordering::less:
                     std::strong_ordering::equal;
}

} // end namespace Dune::Std

#endif // DUNE_COMMON_STD_ALGORITHM_HH
