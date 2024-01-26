// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_STD_LAYOUT_LEFT_HH
#define DUNE_COMMON_STD_LAYOUT_LEFT_HH

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
#include <type_traits>

#include <dune/common/indices.hh>
#include <dune/common/std/impl/fwd_layouts.hh>
#endif

namespace Dune::Std {

/*
 * The utility std::layout_left is part of the <mdspan> header introduced
 * in c++23. It might also be available in Kokkos-mdspan proposal implementation
 * library.
 *
 * The utilities should be deprecated once the minimal compiler version supported
 * by Dune have mdspan support, e.g., clang libc++ >= 17.
 */

#if DUNE_HAVE_CXX_STD_MDSPAN

using std::layout_left;

#else // DUNE_HAVE_CXX_STD_MDSPAN

/**
 * \brief A layout mapping where the leftmost extent has stride 1.
 * \ingroup CxxUtilities
 *
 * For two-dimensional tensors this corresponds to column-major indexing.
 **/
template <class Extents>
class layout_left::mapping
{
  template <class> friend class mapping;

public:
  using extents_type = Extents;
  using size_type = typename extents_type::size_type;
  using rank_type = typename extents_type::rank_type;
  using index_type = typename extents_type::index_type;
  using layout_type = layout_left;

  /// \brief The default construction is possible for default constructible extents
  constexpr mapping () noexcept = default;

  /// \brief Copy constructor for the mapping
  constexpr mapping (const mapping&) noexcept = default;

  /// \brief Construct the mapping with from given extents
  constexpr mapping (const extents_type& e) noexcept
    : extents_(e)
  {}

  /// \brief Construct the mapping from another mapping with a different extents type
  template <class OtherExtents,
    std::enable_if_t<std::is_constructible_v<extents_type, OtherExtents>, int> = 0>
  #if __cpp_conditional_explicit >= 201806L
  explicit(!std::is_convertible_v<OtherExtents, extents_type>)
  #endif
  constexpr mapping (const mapping<OtherExtents>& m) noexcept
    : extents_(m.extents())
  {}

  /// \brief Construct the mapping from a layout_right
  template <class OtherExtents, class E = extents_type,
    std::enable_if_t<(E::rank() <= 1), int> = 0,
    std::enable_if_t<std::is_constructible_v<extents_type, OtherExtents>, int> = 0>
  #if __cpp_conditional_explicit >= 201806L
  explicit(!std::is_convertible_v<OtherExtents, extents_type>)
  #endif
  constexpr mapping (const layout_right::mapping<OtherExtents>& m) noexcept
    : extents_(m.extents())
  {}

  /// \brief Construct the mapping from a layout_stride
  template <class OtherExtents,
    std::enable_if_t<std::is_constructible_v<extents_type, OtherExtents>, int> = 0>
  #if __cpp_conditional_explicit >= 201806L
  explicit(extents_type::rank() > 0)
  #endif
  constexpr mapping (const layout_stride::mapping<OtherExtents>& m)
    : extents_(m.extents())
  {
#ifndef NDEBUG
    if constexpr(extents_type::rank() > 0) {
      for (rank_type r = 0; r < extents_type::rank(); ++r)
        assert(m.strides(r) == m.extents()._fwd_product(r));
    }
#endif
  }

  /// \brief Copy-assignment for the mapping
  constexpr mapping& operator= (const mapping&) noexcept = default;

  constexpr const extents_type& extents () const noexcept { return extents_; }
  constexpr index_type required_span_size () const noexcept { return extents_._product(); }

  /// \brief Compute the offset i0 + E(0)*(i1 + E(1)*(i2 + E(2)*i3))
  template <class... Indices,
    std::enable_if_t<(sizeof...(Indices) == extents_type::rank()), int> = 0,
    std::enable_if_t<(... && std::is_convertible_v<Indices, index_type>), int> = 0,
    std::enable_if_t<(... && std::is_nothrow_constructible_v<Indices, index_type>), int> = 0>
  constexpr index_type operator() (Indices... ii) const noexcept
  {
    const std::array indices{index_type(std::move(ii))...};
    index_type value = indices.back();
    for (rank_type r = 1; r < extents_type::rank(); ++r) {
      const rank_type j = extents_type::rank()-r;
      value = indices[j-1] + extents_.extent(j-1) * value;
    }
    return value;
  }

  /// \brief The default offset for rank-0 tensors is 0
  constexpr index_type operator() () const noexcept
  {
    return 0;
  }

  static constexpr bool is_always_unique () noexcept { return true; }
  static constexpr bool is_always_exhaustive () noexcept { return true; }
  static constexpr bool is_always_strided () noexcept { return true; }

  static constexpr bool is_unique () noexcept { return true; }
  static constexpr bool is_exhaustive () noexcept { return true; }
  static constexpr bool is_strided () noexcept { return true; }

  /// \brief The stride is the product of the extents `E(0)*E(1)*...*E(i-1)`
  template <class E = extents_type,
    std::enable_if_t<(E::rank() > 0), int> = 0>
  constexpr index_type stride (rank_type i) const noexcept
  {
    // [[expects: i < extents_type::rank()]]
    return extents()._fwd_product(i);
  }

  template <class OtherExtents,
    std::enable_if_t<(Extents::rank() == OtherExtents::rank()), int> = 0>
  friend constexpr bool operator== (const mapping& a, const mapping<OtherExtents>& b) noexcept
  {
    return a.extents_ == b.extents_;
  }

private:
  [[no_unique_address]] extents_type extents_;
};

#endif  // DUNE_HAVE_CXX_STD_MDSPAN

} // end namespace Dune::Std

#endif // DUNE_COMMON_STD_LAYOUT_LEFT_HH
