// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_ZEROTENSOR_HH
#define DUNE_COMMON_ZEROTENSOR_HH

#include <cstddef>
#include <type_traits>

#include <dune/common/densetensormixin.hh>
#include <dune/common/ftraits.hh>
#include <dune/common/indices.hh>
#include <dune/common/zero.hh>
#include <dune/common/std/mdspan.hh>

namespace Dune::Impl {

template <class Element, std::size_t... exts>
struct ZeroTensorStorageType
{
  static_assert(std::is_const_v<Element>, "ZeroTensor can only be instantiated with const element type");

  /// Accessor that always returns zero
  struct ZeroTensorAccessor
  {
  public:
    using element_type = Element;
    using data_handle_type = std::nullptr_t;
    using reference = element_type;
    using offset_policy = ZeroTensorAccessor;

  public:
    /// \brief Return a reference to the i'th element in the data range starting at `p`
    static constexpr element_type access ([[maybe_unused]] data_handle_type p, [[maybe_unused]] std::size_t i) noexcept
    {
      return zero<element_type>();
    }

    /// \brief Return a data handle to the i'th element in the data range starting at `p`
    static constexpr data_handle_type offset (data_handle_type p, [[maybe_unused]] std::size_t i) noexcept
    {
      return p;
    }
  };

  /// Layout that maps all indices to zero
  struct ZeroTensorLayout
  {
    template <class Extents>
    class mapping
    {
      template <class> friend class mapping;

    public:
      using extents_type = Extents;
      using size_type = typename extents_type::size_type;
      using rank_type = typename extents_type::rank_type;
      using index_type = typename extents_type::index_type;
      using layout_type = ZeroTensorLayout;

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

      /// \brief Copy-assignment for the mapping
      constexpr mapping& operator= (const mapping&) noexcept = default;

      constexpr const extents_type& extents () const noexcept { return extents_; }
      constexpr index_type required_span_size () const noexcept { return 0; }

      template <class... Indices>
      constexpr index_type operator() (Indices... ii) const noexcept { return 0; }

      static constexpr bool is_always_unique () noexcept { return extents_type::rank() == 0; }
      static constexpr bool is_always_exhaustive () noexcept { return true; }
      static constexpr bool is_always_strided () noexcept { return false; }

      static constexpr bool is_unique () noexcept { return extents_type::rank() == 0; }
      static constexpr bool is_exhaustive () noexcept { return true; }
      static constexpr bool is_strided () noexcept { return false; }

      template <class E = extents_type,
        std::enable_if_t<(E::rank() > 0), int> = 0>
      constexpr index_type stride (rank_type i) const noexcept { return 0; }

      template <class OtherExtents,
        std::enable_if_t<(Extents::rank() == OtherExtents::rank()), int> = 0>
      friend constexpr bool operator== (const mapping& a, const mapping<OtherExtents>& b) noexcept
      {
        return a.extents_ == b.extents_;
      }

    private:
      DUNE_NO_UNIQUE_ADDRESS extents_type extents_;
    };
  };

  using extents_type = Std::extents<std::size_t,exts...>;
  using type = Std::mdspan<Element,extents_type,ZeroTensorLayout,ZeroTensorAccessor>;
};


template <class T, std::size_t... extents>
class ZeroTensor
    : public DenseTensorMixin<ZeroTensor<T,extents...>,
        typename ZeroTensorStorageType<std::add_const_t<T>,extents...>::type>
{
  using self_type = ZeroTensor;
  using storage_type = typename ZeroTensorStorageType<std::add_const_t<T>,extents...>::type;
  using base_type = DenseTensorMixin<self_type, storage_type>;

public:
  using element_type =	typename storage_type::element_type;
  using value_type = typename base_type::value_type;
  using field_type = typename Dune::FieldTraits<value_type>::field_type;
  using extents_type = typename storage_type::extents_type;
  using index_type = typename storage_type::index_type;
  using layout_type = typename storage_type::layout_type;
  using accessor_type = typename storage_type::accessor_type;
  using mapping_type = typename storage_type::mapping_type;

public:
  /// \name ZeroTensor constructors
  /// @{

  /// \brief Default constructor, only available if extents are default constructible
  constexpr ZeroTensor () requires (std::default_initializable<extents_type>)
    : ZeroTensor(extents_type())
  {}

  /// \brief Construct from the dynamic extents given as variadic list
  template <class... IndexTypes,
    class E = extents_type, class M = mapping_type, class A = accessor_type,
    std::enable_if_t<(sizeof...(IndexTypes) == E::rank() || sizeof...(IndexTypes) == E::rank_dynamic()), int> = 0,
    std::enable_if_t<(... && std::is_convertible_v<IndexTypes, index_type>), int> = 0,
    std::enable_if_t<(... && std::is_nothrow_constructible_v<index_type, IndexTypes>), int> = 0,
    std::enable_if_t<std::is_constructible_v<M, E>, int> = 0,
    std::enable_if_t<std::is_default_constructible_v<A>, int> = 0>
  explicit constexpr ZeroTensor (IndexTypes... exts)
    : ZeroTensor(extents_type(std::move(exts)...))
  {}

  /// \brief Construct from the dynamic extents given as an array
  template <class IndexType, std::size_t N,
    std::enable_if_t<std::is_convertible_v<const IndexType&, index_type>, int> = 0,
    std::enable_if_t<std::is_nothrow_constructible_v<index_type,const IndexType&>, int> = 0,
    std::enable_if_t<(N == extents_type::rank_dynamic() || N == extents_type::rank()), int> = 0>
  #if __cpp_conditional_explicit >= 201806L
  explicit(N != extents_type::rank_dynamic())
  #endif
  constexpr ZeroTensor (std::span<IndexType,N> exts)
    : ZeroTensor(extents_type(std::move(exts)))
  {}

  /// \brief Construct from the pointer to the data of the tensor and its extents
  constexpr ZeroTensor (extents_type e, const element_type& v = zero<element_type>())
    : base_type(nullptr, std::move(e))
  {
    assert(v == zero<element_type>());
  }

  /// \brief Converting constructor
  template <class OtherElementType, std::size_t... OtherExtends>
  constexpr ZeroTensor (const ZeroTensor<OtherElementType,OtherExtends...>& other) noexcept
    : ZeroTensor(extents_type(other.extents()))
  {}

  /// @}


  /// \name Elementwise operations
  // @{

  // updating arithmetic operations

  template <class OtherTensor>
  constexpr ZeroTensor& operator= (const OtherTensor&) = delete;

  template <class OtherTensor>
  constexpr ZeroTensor& operator= (OtherTensor&&) = delete;

  template <class OtherTensor>
  constexpr ZeroTensor& operator+= (const OtherTensor&) = delete;

  template <class OtherTensor>
  constexpr ZeroTensor& operator-= (const OtherTensor&) = delete;

  template <class OtherScalar>
  constexpr ZeroTensor& operator*= (const OtherScalar&) = delete;

  template <class OtherScalar>
  constexpr ZeroTensor& operator/= (const OtherScalar&) = delete;

  template <class OtherScalar, class OtherTensor>
  constexpr ZeroTensor& axpy (const OtherScalar&, const OtherTensor&) = delete;

  template <class OtherScalar, class OtherTensor>
  constexpr ZeroTensor& aypx (const OtherScalar&, const OtherTensor&) = delete;

  // unary and binary arithmetic operations

  constexpr ZeroTensor const& operator- () const { return *this; }

  template <class OtherTensor>
  friend constexpr OtherTensor const& operator+ (const ZeroTensor&, const OtherTensor& rhs) { return rhs; }

  template <class OtherTensor>
  friend constexpr OtherTensor const& operator+ (const OtherTensor& lhs, const ZeroTensor&) { return lhs; }

  template <class OtherTensor>
  friend constexpr OtherTensor operator- (const ZeroTensor&, const OtherTensor& rhs) { return -rhs; }

  template <class OtherTensor>
  friend constexpr OtherTensor const& operator- (const OtherTensor& lhs, const ZeroTensor&) { return lhs; }

  template <class OtherScalar>
  friend constexpr ZeroTensor const& operator* (const ZeroTensor& lhs, const OtherScalar&) { return lhs; }

  template <class OtherScalar>
  friend constexpr ZeroTensor const& operator* (const OtherScalar&, const ZeroTensor& rhs) { return rhs; }

  template <class OtherScalar>
  friend constexpr ZeroTensor const& operator/ (const ZeroTensor& lhs, const OtherScalar&) { return lhs; }

  /// @}


  /// \name tensor dot-products
  // @{

  template <Concept::TensorLike Tensor>
    requires (extents_type::rank() >= 1 && TensorTraits<Tensor>::rank() >= 1 &&
      Impl::checkStaticExtents<1, extents_type, typename TensorTraits<Tensor>::extents_type>())
  constexpr Concept::Tensor auto dot (const Tensor& tensor) const
  {
    return Dune::Impl::ZeroTensor{
      TensorDotTraits::extents<1>(*this, tensor),
      TensorDotTraits::zero(*this,tensor,std::plus<>{},DotProduct{})};
  }

  /// \brief Returns the Hermitian tensor product with contraction over two indices `conj(A_{ijk}) B_{jkl}`
  template <Concept::TensorLike Tensor>
    requires (extents_type::rank() >= 2 && TensorTraits<Tensor>::rank() >= 2 &&
      Impl::checkStaticExtents<1, extents_type, typename TensorTraits<Tensor>::extents_type>())
  constexpr Concept::Tensor auto ddot (const Tensor& tensor) const
  {
    return Dune::Impl::ZeroTensor{
      TensorDotTraits::extents<2>(*this, tensor),
      TensorDotTraits::zero(*this,tensor,std::plus<>{},DotProduct{})};
  }

  /// \brief y = A x
  template <Concept::VectorLike VectorIn, Concept::VectorLike VectorOut>
  constexpr void mv (const VectorIn& x, VectorOut& y) const
      requires (extents_type::rank() == 2)
  {
    y = 0;
  }

  /// \brief y = A^T x
  template <Concept::VectorLike VectorIn, Concept::VectorLike VectorOut>
  constexpr void mtv (const VectorIn& x, VectorOut& y) const
      requires (extents_type::rank() == 2)
  {
    y = 0;
  }

  /// \brief y = A^H x
  template <Concept::VectorLike VectorIn, Concept::VectorLike VectorOut>
  constexpr void mhv (const VectorIn& x, VectorOut& y) const
      requires (extents_type::rank() == 2)
  {
    y = 0;
  }

  /// \brief y += A x
  template <Concept::VectorLike VectorIn, Concept::VectorLike VectorOut>
  constexpr void umv (const VectorIn& x, VectorOut& y) const
      requires (extents_type::rank() == 2)
  {}

  /// \brief y -= A x
  template <Concept::VectorLike VectorIn, Concept::VectorLike VectorOut>
  constexpr void mmv (const VectorIn& x, VectorOut& y) const
      requires (extents_type::rank() == 2)
  {}

  /// \brief y += A^T x
  template <Concept::VectorLike VectorIn, Concept::VectorLike VectorOut>
  constexpr void umtv (const VectorIn& x, VectorOut& y) const
      requires (extents_type::rank() == 2)
  {}

  /// \brief y -= A^T x
  template <Concept::VectorLike VectorIn, Concept::VectorLike VectorOut>
  constexpr void mmtv (const VectorIn& x, VectorOut& y) const
      requires (extents_type::rank() == 2)
  {}

  /// \brief y += A^H x
  template <Concept::VectorLike VectorIn, Concept::VectorLike VectorOut>
  constexpr void umhv (const VectorIn& x, VectorOut& y) const
      requires (extents_type::rank() == 2)
  {}

  /// \brief y -= A^H x
  template <Concept::VectorLike VectorIn, Concept::VectorLike VectorOut>
  constexpr void mmhv (const VectorIn& x, VectorOut& y) const
      requires (extents_type::rank() == 2)
  {}

  /// \brief y += alpha A x
  template <Concept::VectorLike VectorIn, Concept::VectorLike VectorOut>
  constexpr void usmv (const typename FieldTraits<VectorOut>::field_type& alpha,
                       const VectorIn& x, VectorOut& y) const
      requires (extents_type::rank() == 2)
  {}

  /// \brief y += alpha A^T x
  template <Concept::VectorLike VectorIn, Concept::VectorLike VectorOut>
  constexpr void usmtv (const typename FieldTraits<VectorOut>::field_type& alpha,
                        const VectorIn& x, VectorOut& y) const
      requires (extents_type::rank() == 2)
  {}

  /// \brief y += alpha A^H x
  template <Concept::VectorLike VectorIn, Concept::VectorLike VectorOut>
  constexpr void usmhv (const typename FieldTraits<VectorOut>::field_type& alpha,
                        const VectorIn& x, VectorOut& y) const
      requires (extents_type::rank() == 2)
  {}

  /// @}


  /// \name tensor norms and inner products
  // @{

  /// \brief Returns the Hermitian tensor inner product with contraction over all indices `conj(A_{ij}) B_{ij}`
  template <Concept::TensorLikeWithRank<extents_type::rank()> Tensor>
    requires (Impl::checkStaticExtents<extents_type::rank(), extents_type, typename TensorTraits<Tensor>::extents_type>())
  constexpr Concept::Number auto inner (const Tensor& tensor) const
  {
    using Result = decltype(Dune::Impl::ZeroTensor{
      TensorDotTraits::extents<extents_type::rank()>(*this, tensor),
      TensorDotTraits::zero(*this,tensor,std::plus<>{},DotProduct{})});
    using F = typename FieldTraits<Result>::field_type;
    return zero<F>();
  }

  /// \brief Square of Frobenius norm
  typename FieldTraits<value_type>::real_type frobenius_norm2 () const
  {
    using R = typename FieldTraits<value_type>::real_type;
    return zero<R>();
  }

  /// \brief Frobenius norm
  typename FieldTraits<value_type>::real_type frobenius_norm () const
  {
    using R = typename FieldTraits<value_type>::real_type;
    return zero<R>();
  }

  /// \brief Square of 2-norm
  typename FieldTraits<value_type>::real_type two_norm2 () const
      requires (extents_type::rank() == 1)
  {
    using R = typename FieldTraits<value_type>::real_type;
    return zero<R>();
  }

  /// \brief 2-norm
  typename FieldTraits<value_type>::real_type two_norm () const
      requires (extents_type::rank() == 1)
  {
    using R = typename FieldTraits<value_type>::real_type;
    return zero<R>();
  }

  /// \brief Infinity norm
  typename FieldTraits<value_type>::real_type infinity_norm () const
  {
    using R = typename FieldTraits<value_type>::real_type;
    return zero<R>();
  }

  /// @}
};

template <class IndexType, std::size_t... exts, class ElementType>
ZeroTensor (Dune::Std::extents<IndexType,exts...>, ElementType)
  -> ZeroTensor<ElementType, exts...>;

} // namespace Dune::Impl

template <class T, std::size_t... extents>
struct Dune::FieldTraits<Dune::Impl::ZeroTensor<T,extents...>>
{
  using field_type = typename Dune::FieldTraits<T>::field_type;
  using real_type = typename Dune::FieldTraits<T>::real_type;
};

#endif // DUNE_COMMON_ZEROTENSOR_HH
