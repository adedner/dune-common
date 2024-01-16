// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_STD_EXTENTS_HH
#define DUNE_COMMON_STD_EXTENTS_HH

#if __has_include(<mdspan>)
  #include <mdspan>
#endif

#if __cpp_lib_mdspan >= 202207L
  #ifndef DUNE_HAVE_CXX_STD_MDSPAN
  #define DUNE_HAVE_CXX_STD_MDSPAN 1
  #endif
#elif __has_include(<experimental/mdspan>)
  #include <experimental/mdspan>
  #ifndef DUNE_HAVE_CXX_STD_MDSPAN
  #define DUNE_HAVE_CXX_STD_MDSPAN 1
  #endif
#endif

#if !DUNE_HAVE_CXX_STD_MDSPAN
#include <array>
#include <cassert>
#include <limits>
#include <type_traits>
#if __has_include(<version>)
  #include <version>
#endif

#include <dune/common/indices.hh>
#include <dune/common/std/span.hh>
#endif

namespace Dune::Std {

/*
 * The utilities std::extents and std::dextents are part of the <mdspan> header
 * introduced in c++23. It might also be available in Kokkos-mdspan proposal
 * implementation library.
 *
 * The utilities should be deprecated once the minimal compiler version supported
 * by Dune have mdspan support, e.g., clang libc++ >= 17.
 */

#if DUNE_HAVE_CXX_STD_MDSPAN

using std::extents;
using std::dextents;

#else // DUNE_HAVE_CXX_STD_MDSPAN

namespace Impl {

template <class IndexType, std::size_t n>
struct DynamicExtentsArray
{
  using type = std::array<IndexType,n>;
};

template <class IndexType>
struct DynamicExtentsArray<IndexType,0>
{
  // empty type with minimal array-like interface
  struct type {
    IndexType operator[](std::size_t /*i*/) const { return 0; }
  };
};

} // end namespace Impl


/**
 * \brief Multidimensional index space with dynamic and static extents.
 * \ingroup CxxUtilities
 *
 * This class template represents a multidimensional index space of rank equal to `sizeof...(exts)`.
 * Each extent might be specified as a template parameter or as a dynamic parameter in the constructor.
 *
 * \tparam IndexType  An integral type other than `bool`
 * \tparam exts...    Each element of exts is either `Std::dynamic_extent` or a representable
 *                    value of type `IndexType`.
 **/
template <class IndexType, std::size_t... exts>
class extents
{
  template <class, std::size_t...> friend class extents;

  static_assert(std::is_integral_v<IndexType>);

  static inline constexpr std::size_t _rank = sizeof...(exts);
  static inline constexpr std::size_t _rank_dynamic = ((exts == Std::dynamic_extent) + ... + 0);

  using array_type = std::array<std::size_t,_rank>;

public:
  using rank_type = std::size_t;
  using index_type = IndexType;
  using size_type = std::make_unsigned_t<index_type>;

public:
  /// \name Observers
  /// @{

  /// \brief The total number of dimensions
  static constexpr rank_type rank () noexcept { return _rank; }

  /// \brief The number of dimensions with dynamic extent
  static constexpr rank_type rank_dynamic () noexcept { return _rank_dynamic; }

  /// \brief Return the static extent of dimension `r` or `Std::dynamic_extent`
  static constexpr std::size_t static_extent (rank_type r) noexcept
  {
    return _rank == 0 ? 1 : array_type{exts...}[r];
  }

  /// \brief Return the extent of dimension `i`
  constexpr index_type extent (rank_type r) const noexcept
  {
    if constexpr(rank() == 0)
      return 1;
    else if (std::size_t e = static_extent(r); e != Std::dynamic_extent)
      return index_type(e);
    else
      return dynamic_extents_[dynamic_index(r)];
  }

  /// @}

public:
  /// \name extents constructors
  /// @{

  /// \brief The default constructor requires that all exts are not `Std::dynamic_extent`.
  constexpr extents () noexcept = default;

  /// \brief Set all extents by the given integral sequence
  /// [[pre: all static extents correspond to the given value e]]
  template <class... IndexTypes,
    std::enable_if_t<(... && std::is_convertible_v<IndexTypes,index_type>), int> = 0,
    std::enable_if_t<(sizeof...(IndexTypes) == rank() || sizeof...(IndexTypes) == rank_dynamic()), int> = 0>
  constexpr extents (const IndexTypes&... e) noexcept
  {
    init_dynamic_extents<sizeof...(e)>(std::array<index_type,sizeof...(e)>{index_type(e)...});
  }

  /// \brief Set all dynamic extents by the given integral array
  /// [[pre: all static extents correspond to the given values in e]]
  template <class I, std::size_t N,
    std::enable_if_t<std::is_convertible_v<I, index_type>, int> = 0,
    std::enable_if_t<(N == rank()  || N == rank_dynamic()), int> = 0>
  #if __cpp_conditional_explicit >= 201806L
  explicit(N != rank_dynamic())
  #endif
  constexpr extents (const std::array<I,N>& e) noexcept
  {
    init_dynamic_extents<N>(e);
  }

  /// \brief Set all dynamic extents by the given integral array
  /// [[pre: all static extents correspond to the given values in e]]
  template <class I, std::size_t N,
    std::enable_if_t<std::is_convertible_v<I, index_type>, int> = 0,
    std::enable_if_t<(N == rank()  || N == rank_dynamic()), int> = 0>
  #if __cpp_conditional_explicit >= 201806L
  explicit(N != rank_dynamic())
  #endif
  constexpr extents (Std::span<I,N> e) noexcept
  {
    init_dynamic_extents<N>(e);
  }

  template <class I, std::size_t... e,
    std::enable_if_t<(sizeof...(e) == rank()), int> = 0,
    std::enable_if_t<((e == Std::dynamic_extent || exts == Std::dynamic_extent || e == exts) &&...), int> = 0>
  #if __cpp_conditional_explicit >= 201806L
  explicit(
    (( (exts != Std::dynamic_extent) && (e == Std::dynamic_extent)) || ... ) ||
    (std::numeric_limits<index_type>::max() < std::numeric_limits<I>::max()))
  #endif
  constexpr extents (const extents<I,e...>& other) noexcept
  {
    init_dynamic_extents<sizeof...(e)>(as_array(other));
  }

  /// @}


  /// \brief Compare two extents by their rank and all individual extents
  template <class OtherIndexType, std::size_t... otherExts>
  friend constexpr bool operator== (const extents& a, const extents<OtherIndexType, otherExts...>& b) noexcept
  {
    if (a.rank() != b.rank())
      return false;
    using I = std::common_type_t<index_type, OtherIndexType>;
    for (rank_type i = 0; i < rank(); ++i)
      if (I(a.extent(i)) != I(b.extent(i)))
        return false;
    return true;
  }


#ifndef DOXYGEN

  // ---------------------------
  // Some implementation details

  // The product of all extents
  constexpr size_type _product () const noexcept
  {
    return _fwd_product(rank());
  }

  // The product of all extents up to r-1
  constexpr size_type _fwd_product (rank_type r) const noexcept
  {
    // [[pre: r <= rank()]]
    size_type prod = 1;
    for (rank_type i = 0; i < r; ++i)
      prod *= extent(i);
    return prod;
  }

  // The product of all extents down to r+1
  constexpr size_type _rev_product (rank_type r) const noexcept
  {
    // [[pre: r < rank()]]
    size_type prod = 1;
    for (rank_type i = r+1; i < rank(); ++i)
      prod *= extent(i);
    return prod;
  }

#endif

private:
  // Compute the index within dynamic_extents corresponding to the total index `i` in extents.
  // This corresponds to the j'th occurrence of `Std::dynamic_extent` within `exts...`.
  static constexpr rank_type dynamic_index (rank_type i) noexcept
  {
    // [[pre: i <= rank()]];
    rank_type j = 0;
    for (rank_type r = 0; r < i; ++r)
      if (static_extent(r) == Std::dynamic_extent)
        ++j;
    return j;
  }

  // A representation of all extents as an array
  template <class OtherIndexType, std::size_t... otherExts>
  static constexpr std::array<index_type,sizeof...(otherExts)>
  as_array (const Std::extents<OtherIndexType,otherExts...>& e) noexcept
  {
    return unpackIntegerSequence([&](auto... ii) {
      return std::array<index_type,sizeof...(otherExts)>{index_type(e.extent(ii))...}; },
      std::make_index_sequence<sizeof...(otherExts)>{});
  }

  // Copy only the dynamic extents from the container `e` into the `dynamic_extents_` storage
  template <std::size_t N, class Container>
  constexpr void init_dynamic_extents (const Container& e) noexcept
  {
    if constexpr(rank_dynamic() > 0) {
      if constexpr(N == rank_dynamic()) {
        assert(e.size() == rank_dynamic());
        for (rank_type i = 0; i < rank_dynamic(); ++i)
          dynamic_extents_[i] = e[i];
      } else {
        assert(e.size() == rank());
        for (rank_type i = 0, j = 0; i < rank(); ++i) {
          if (static_extent(i) == Std::dynamic_extent)
            dynamic_extents_[j++] = e[i];
        }
      }
    }
  }

private:
  using dynamic_extents_type = typename Impl::DynamicExtentsArray<index_type,rank_dynamic()>::type;
  [[no_unique_address]] dynamic_extents_type dynamic_extents_;
};


namespace Impl {

template <class IndexType, class Seq>
struct DExtentsImpl;

template <class IndexType, std::size_t... I>
struct DExtentsImpl<IndexType, std::integer_sequence<std::size_t,I...>>
{
  template <std::size_t>
  using dynamic = std::integral_constant<std::size_t,Std::dynamic_extent>;
  using type = Std::extents<IndexType, dynamic<I>::value...>;
};

} // end namespace Impl


/**
 * \brief Alias of `extents` of given rank `R` and purely dynamic extents.
 * \ingroup CxxUtilities
 **/
template <class IndexType, std::size_t R>
using dextents = typename Impl::DExtentsImpl<IndexType, std::make_integer_sequence<std::size_t,R>>::type;

#endif // DUNE_HAVE_CXX_STD_MDSPAN

} // end namespace Dune::Std

#endif // DUNE_COMMON_STD_EXTENTS_HH
