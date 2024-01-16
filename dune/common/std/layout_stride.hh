// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_STD_LAYOUT_STRIDE_HH
#define DUNE_COMMON_STD_LAYOUT_STRIDE_HH

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
#include <dune/common/std/layout_right.hh>
#endif

namespace Dune::Std {

/*
 * The utility std::layout_stride is part of the <mdspan> header introduced
 * in c++23. It might also be available in Kokkos-mdspan proposal implementation
 * library.
 *
 * The utilities should be deprecated once the minimal compiler version supported
 * by Dune have mdspan support, e.g., clang libc++ >= 18 (not yet available in 17).
 */

#if DUNE_HAVE_CXX_STD_MDSPAN

using std::layout_stride;

#else // DUNE_HAVE_CXX_STD_MDSPAN

/**
 * \brief A layout mapping where the strides are user-defined.
 * \ingroup CxxUtilities
 **/
struct layout_stride
{
  template <class Extents>
  class mapping
  {
    template <class> friend class mapping;
    static constexpr auto _rank = Extents::rank();

  public:
    using extents_type = Extents;
    using index_type = typename extents_type::index_type;
    using size_type = typename extents_type::size_type;
    using rank_type = typename extents_type::rank_type;
    using result_type = index_type;
    using layout_type = layout_stride;
    using strides_type = std::array<index_type,_rank>;

    /// \brief The default construction initializes the strides from layout_right
    constexpr mapping () noexcept
      : mapping(Std::layout_right::template mapping<extents_type>{})
    {}

    /// \brief Construct the mapping from given extents and strides
    constexpr mapping (const extents_type& e, const strides_type& s) noexcept
      : extents_(e)
      , strides_(s)
    {}

    /// \brief Construct the mapping from given extents and strides
    template <class OtherIndexType,
      std::enable_if_t<std::is_convertible_v<OtherIndexType, index_type>, int> = 0>
    constexpr mapping (const extents_type& e, const std::array<OtherIndexType,_rank>& s) noexcept
      : extents_(e)
      , strides_{}
    {
      for (rank_type r = 0; r < _rank; ++r)
        strides_[r] = s[r];
    }

    /// \brief Construct the mapping from another mapping with different extents
    template <class E>
    constexpr mapping (const mapping<E>& m) noexcept
      : extents_(m.extents_)
      , strides_(m.strides_)
    {}

    /// \brief Construct the mapping from another mapping with different extents
    template <class E>
    constexpr mapping (mapping<E>&& m) noexcept
      : extents_(std::move(m.extents_))
      , strides_(std::move(m.strides_))
    {}

    /// \brief Construct the mapping from another mapping with different extents and different strides
    template <class M,
      std::enable_if_t<(M::extents_type::rank() == extents_type::rank()), int> = 0,
      decltype(std::declval<M>().extents(), bool{}) = true,
      decltype(std::declval<M>().stride(std::declval<rank_type>()), bool{}) = true>
    constexpr mapping (const M& m) noexcept
      : extents_(m.extents())
      , strides_{}
    {
      for (rank_type r = 0; r < _rank; ++r)
        strides_[r] = m.stride(r);
    }

    constexpr const extents_type& extents () const noexcept { return extents_; }

    /// \brief Return the sum `1 + (E(0)-1)*S(0) + (E(1)-1)*S(1) + ...`
    constexpr std::size_t required_span_size () const noexcept
    {
      return _size(extents_,strides_);
    }

    /// \brief Compute the offset by folding with index-array with the strides array
    template <class... Indices,
      std::enable_if_t<(sizeof...(Indices) == _rank), int> = 0>
    constexpr index_type operator() (Indices... ii) const noexcept
    {
      return unpackIntegerSequence([&](auto... r) {
        return ((static_cast<index_type>(ii)*strides_[r]) + ... + 0); },
        std::make_index_sequence<_rank>{});
    }

    /// \brief The default offset for rank-0 tensors is 0
    constexpr index_type operator() () const noexcept
    {
      return 0;
    }

    static constexpr bool is_always_unique () noexcept { return true; }
    static constexpr bool is_always_exhaustive () noexcept { return false; }
    static constexpr bool is_always_strided () noexcept { return true; }

    static constexpr bool is_unique () noexcept { return true; }
    static constexpr bool is_strided () noexcept { return true; }

    constexpr bool is_exhaustive () const noexcept
    {
      // Actually this could be improved. A strided layout can still be exhaustive.
      // This test is more complicated to implement, though. See §24.7.3.4.7.4 line (5.2)
      // in the C++ standard document N4971
      return extents_type::rank() == 0 || (required_span_size() > 0 && required_span_size() == extents()._product());
    }

    /// \brief Get the array of all strides
    constexpr const strides_type& strides () const noexcept
    {
      return strides_;
    }

    /// \brief Get the single stride `i`
    template <class E = extents_type,
      std::enable_if_t<(E::rank() > 0), int> = 0>
    constexpr index_type stride (rank_type i) const noexcept
    {
      return strides_[i];
    }

    template <class OtherExtents,
      std::enable_if_t<(OtherExtents::rank() == extents_type::rank()), int> = 0>
    friend constexpr bool operator== (const mapping& a, const mapping<OtherExtents>& b) noexcept
    {
      if (_offset(b))
        return false;
      if constexpr(extents_type::rank() == 0)
        return true;
      return a.extents_ == b.extents_ && a.strides_ == b.strides_;
    }

  private:
    template <class E, class S>
    static constexpr index_type _size (const E& extents, const S& strides) noexcept
    {
      if constexpr (E::rank() == 0)
        return 1;
      else {
        if (extents._product() == 0)
          return 0;
        else {
          index_type result = 1;
          for (rank_type r = 0; r < E::rank(); ++r)
            result += (extents.extent(r)-1) * strides[r];
          return result;
        }
      }
    }

    template <class M>
    static constexpr size_type _offset (const M& m) noexcept
    {
      if constexpr (M::extents_type::rank() == 0)
        return m();
      else {
        if (m.required_span_size() == 0)
          return 0;
        else {
          return unpackIntegerSequence([&](auto... r) {
            return m((r,0)...); }, // map the index tuple (0,0...)
            std::make_index_sequence<M::extents_type::rank()>{});
        }
      }
    }

  private:
    [[no_unique_address]] extents_type extents_;
    strides_type strides_;
  };
};

#endif // DUNE_HAVE_CXX_STD_MDSPAN

} // end namespace Dune::Std

#endif // DUNE_COMMON_STD_LAYOUT_STRIDE_HH
