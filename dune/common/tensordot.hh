// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_TENSORDOT_HH
#define DUNE_COMMON_TENSORDOT_HH

#include <array>
#include <cassert>
#include <functional>
#include <span>
#include <type_traits>
#include <utility>

#include <dune/common/integersequence.hh>
#include <dune/common/std/extents.hh>
#include <dune/common/rangeutilities.hh>
#include <dune/common/tensortraits.hh>
#include <dune/common/typetraits.hh>
#include <dune/common/zero.hh>
#include <dune/common/concepts/tensor.hh>
#include <dune/common/std/extents.hh>
#include <dune/common/std/forceinline.hh>

namespace Dune {

// forward declaration
template <class V, std::size_t... exts>
class DenseTensor;

namespace Impl {

// Extract from the given extents only those dimensions specified by the `dims` sequence
template <class E, std::size_t... II>
constexpr auto sliceExtents (const E& extents, std::index_sequence<II...> dims)
{
  return Std::extents<typename E::index_type, E::static_extent(II)...>{extents.extent(II)...};
}

// Concatenate the given extents to define a new index space
template <class I1, std::size_t... exts1, class I2, std::size_t... exts2>
constexpr auto concatExtents (const Std::extents<I1,exts1...>& e1,
                              const Std::extents<I2,exts2...>& e2)
{
  using I = std::common_type_t<I1,I2>;
  return [&]<std::size_t... II, std::size_t... JJ>(std::index_sequence<II...>, std::index_sequence<JJ...>) {
    return Std::extents<I, exts1..., exts2...>{I(e1.extent(II))..., I(e2.extent(JJ))...};
  }(std::make_index_sequence<sizeof...(exts1)>{},std::make_index_sequence<sizeof...(exts2)>{});
}

// Check that the dynamic extents of e1 and e2 corresponding to the positions II... and JJ... are identical
template <class E1, std::size_t... II, class E2, std::size_t... JJ>
constexpr bool checkExtents (const E1& e1, std::index_sequence<II...>, const E2& e2, std::index_sequence<JJ...>)
{
  static_assert(sizeof...(II) == sizeof...(JJ));
  using S = std::common_type_t<typename E1::index_type, typename E2::index_type>;
  return ((S(e1.extent(II)) == S(e2.extent(JJ))) && ...);
}

// Check that the static extents of E1 and E2 corresponding to the positions II... and JJ... are identical or dynamic
template <class E1, class E2, std::size_t... II, std::size_t... JJ>
constexpr bool checkStaticExtents (std::index_sequence<II...>, std::index_sequence<JJ...>)
{
  static_assert(sizeof...(II) == sizeof...(JJ));
  return ((E1::static_extent(II) == E2::static_extent(JJ) ||
           E1::static_extent(II) == std::dynamic_extent ||
           E2::static_extent(JJ) == std::dynamic_extent) && ...);
}

// Check the extents for the special case that we contract over N indices
template <std::size_t N, class E1, class E2>
constexpr bool checkStaticExtents ()
{
  if constexpr (E1::rank() < N || E2::rank() < N)
    return false;
  else {
    using SeqI = typename StaticIntegralRange<std::size_t,E1::rank(),E1::rank()-N>::integer_sequence;
    using SeqJ = std::make_index_sequence<N>;
    return checkStaticExtents<E1,E2>(SeqI{},SeqJ{});
  }
}

/**
 * \brief Perform a recursive nested loop over all indices:
 *
 * \b Example:
 * `C_ijk = A_ilm * B_lmjk`
 *
 * There are three sets of indices to loop over:
 * - {lm}, the indices to sum over, this corresponds to components aSeq={1,2} from A and bSeq={0,1} from B
 * - {i}, the remaining indices from A: aSeqInv={0}
 * - {jk}, the remaining indices from B: bSeqInv={2,3}
 *
 * The algorithm loops over all these indices in the corresponding ranges [0,extent]:
 * \code{.cpp}
for (int l = 0; l < a.extent(aSeq[0]); ++l)
  for (int m = 0; m < a.extent(aSeq[1]); ++m)
    for (int i = 0; i < a.extent(aSeqInv[0]); ++i)
      for (int j = 0; j < b.extent(bSeqInv[0]); ++j)
        for (int k = 0; k < b.extent(bSeqInv[1]); ++k)
          c(i,j,k) = op1(c(i,j,k), op2(a(i,l,m), b(l,m,j,k)))
 * \endcode
 *
 * where `op1` typically means `plus` and `op2` typically means `multiplies`.
 **/
template <std::size_t K = 0,
          class A, class ASeq, class ASeqInv,
          class B, class BSeq, class BSeqInv,
          class C, class BinaryOp1, class BinaryOp2,
          class ATraits = TensorTraits<A>,
          class BTraits = TensorTraits<B>,
          class CTraits = TensorTraits<C>>
constexpr DUNE_FORCE_INLINE
void tensorDotImpl (const A& a, ASeq aSeq, ASeqInv aSeqInv,
                    const B& b, BSeq bSeq, BSeqInv bSeqInv,
                    C& c, BinaryOp1 op1, BinaryOp2 op2,
                    std::array<typename ATraits::index_type,ATraits::rank()> aIndices = {},
                    std::array<typename BTraits::index_type,BTraits::rank()> bIndices = {},
                    std::array<typename CTraits::index_type,CTraits::rank()> cIndices = {})
{
  if constexpr(aSeq.size() > 0 && bSeq.size() > 0) {
    // first, loop over the contraction indices of A and B
    constexpr std::size_t I = head(aSeq);
    constexpr std::size_t J = head(bSeq);
    for (typename ATraits::index_type k = 0; k < ATraits::extent(a,I); ++k) {
      aIndices[I] = k;
      bIndices[J] = k;
      tensorDotImpl<K>(a,tail(aSeq),aSeqInv,b,tail(bSeq),bSeqInv,c,op1,op2,aIndices,bIndices,cIndices);
    }
  }
  else if constexpr(aSeqInv.size() > 0) {
    // second, loop over the remaining indices of tensor A
    constexpr std::size_t I = head(aSeqInv);
    for (typename ATraits::index_type i = 0; i < ATraits::extent(a,I); ++i) {
      aIndices[I] = i;
      cIndices[K] = i;
      tensorDotImpl<K+1>(a,aSeq,tail(aSeqInv),b,bSeq,bSeqInv,c,op1,op2,aIndices,bIndices,cIndices);
    }
  }
  else if constexpr(bSeqInv.size() > 0) {
    // third, loop over the remaining indices of tensor B
    constexpr std::size_t J = head(bSeqInv);
    for (typename BTraits::index_type j = 0; j < BTraits::extent(b,J); ++j) {
      bIndices[J] = j;
      cIndices[K] = j;
      tensorDotImpl<K+1>(a,aSeq,aSeqInv,b,bSeq,tail(bSeqInv),c,op1,op2,aIndices,bIndices,cIndices);
    }
  }
  else {
    // finally, add up the contribution to tensor C, e.g. c[i,j,k] += a[i,l,m] * b[l,m,j,k]
    c[cIndices] = std::invoke(op1, std::move(c[cIndices]), std::invoke(op2, a[aIndices], b[bIndices]));
  }
}

} // end namespace Impl


/**
 * \brief Product of two tensors `A` and `B`, stored in the output tensor `C`.
 *
 * Product of tensors `A` and `B` with index contraction over positions `II...`
 * of `A` and positions `JJ...` of `B`. The output tensor `C` must have a rank
 * corresponding to the number of indices remaining in `A` and `B`.
 *
 * The product might be accumulated to the output tensor using outer binary operation
 * `op1`, e.g., a plus functor, and the inner binary operation `op2`, e.g., a
 * multiplies functor, see also the `std::inner_product` algorithm.
 */
template <Concept::TensorLike A, std::size_t... II,
          Concept::TensorLike B, std::size_t... JJ,
          Concept::TensorLike C,
          class BinaryOp1 = std::plus<>, class BinaryOp2 = std::multiplies<>>
constexpr auto tensordotOut (const A& a, std::index_sequence<II...> aSeq,
                             const B& b, std::index_sequence<JJ...> bSeq,
                             C& c, BinaryOp1 op1 = {}, BinaryOp2 op2 = {})
{
  static_assert(aSeq.size() == bSeq.size());

  using ATraits = TensorTraits<A>;
  using BTraits = TensorTraits<B>;
  using CTraits = TensorTraits<C>;

  // create integer sequences that do not include the contraction indices
  const auto aSeqInv = difference<ATraits::rank()>(aSeq); // {0,1,...A::rank()-1} \ {II...}
  const auto bSeqInv = difference<BTraits::rank()>(bSeq); // {0,1,...B::rank()-1} \ {JJ...}
  static_assert(aSeqInv.size() + bSeqInv.size() == CTraits::rank());

  // the extents of a and the extents of b must be compatible to c
  using EA = typename ATraits::extents_type;
  using EB = typename BTraits::extents_type;
  static_assert(Impl::checkStaticExtents<EA,EB>(aSeq, bSeq));
  assert((Impl::checkExtents(ATraits::extents(a), aSeq, BTraits::extents(b), bSeq)));

  // Objects of `A` and `B` must be different from object `C`
  assert((void*)(&a) != (void*)(&c) && (void*)(&b) != (void*)(&c));
  Impl::tensorDotImpl(a,aSeq,aSeqInv,b,bSeq,bSeqInv,c,std::ref(op1),std::ref(op2));
}


/**
 * \brief Product of two tensors `A` and `B` contracted over the last `N` indices
 * of `A` and the first `N` indices of `B`, stored in the output tensor.
 *
 * Product of tensors `A` and `B` with index contraction over last `N` positions
 * of `A` and first `N` positions of `B`. The output tensor `C` must have a rank
 * corresponding to the number of indices remaining in `A` and `B`.
 *
 * The product might be accumulated to the output tensor using outer binary operation
 * `op1`, e.g., a plus functor, and the inner binary operation `op2`, e.g., a
 * multiplies functor, see also the `std::inner_product` algorithm.
 */
template <std::size_t N,
          Concept::TensorLike A,
          Concept::TensorLike B,
          Concept::TensorLike C,
          class BinaryOp1 = std::plus<>, class BinaryOp2 = std::multiplies<>>
constexpr void tensordotOut (const A& a, const B& b, C& c,
                             std::integral_constant<std::size_t,N> axes = {},
                             BinaryOp1 op1 = {}, BinaryOp2 op2 = {})
{
  using ATraits = TensorTraits<A>;
  using BTraits = TensorTraits<B>;
  using CTraits = TensorTraits<C>;

  using SeqI = typename StaticIntegralRange<std::size_t,ATraits::rank(),ATraits::rank()-N>::integer_sequence;
  using InvSeqI = std::make_index_sequence<ATraits::rank()-N>;
  using SeqJ = std::make_index_sequence<N>;
  using InvSeqJ = typename StaticIntegralRange<std::size_t,BTraits::rank(),N>::integer_sequence;
  static_assert(InvSeqI::size() + InvSeqJ::size() == CTraits::rank());

  // the extents of a and the extents of b must be compatible to c
  using EA = typename ATraits::extents_type;
  using EB = typename BTraits::extents_type;
  static_assert(Impl::checkStaticExtents<EA,EB>(SeqI{}, SeqJ{}));
  assert((Impl::checkExtents(ATraits::extents(a), SeqI{}, BTraits::extents(b), SeqJ{})));

  // Objects of `A` and `B` must be different from object `C`
  assert((void*)(&a) != (void*)(&c) && (void*)(&b) != (void*)(&c));
  Impl::tensorDotImpl(a,SeqI{},InvSeqI{},b,SeqJ{},InvSeqJ{},c,std::ref(op1),std::ref(op2));
}

/**
 * \brief Traits class to determine the extents of the result of a tensordot operation.
 */
struct TensorDotTraits
{
  template <Concept::TensorLike A, std::size_t... II,
            Concept::TensorLike B, std::size_t... JJ,
            class BinaryOp1 = std::plus<>, class BinaryOp2 = std::multiplies<>>
  static constexpr auto extents (const A& a, std::index_sequence<II...> aSeq,
                                 const B& b, std::index_sequence<JJ...> bSeq)
  {
    using ATraits = TensorTraits<A>;
    using BTraits = TensorTraits<B>;

    // the extents(II) of a and the extents(JJ) of b must match
    using EA = typename ATraits::extents_type;
    using EB = typename BTraits::extents_type;
    static_assert(Impl::checkStaticExtents<EA,EB>(aSeq, bSeq));
    assert((Impl::checkExtents(ATraits::extents(a), aSeq, BTraits::extents(b), bSeq)));

    // create integer sequences that do not include the contraction indices
    const auto aSeqInv = difference<ATraits::rank()>(aSeq); // {0,1,...A::rank()-1} \ {II...}
    const auto bSeqInv = difference<BTraits::rank()>(bSeq); // {0,1,...B::rank()-1} \ {JJ...}

    // create result extents by collecting the extents of a and b that are not contracted
    return Impl::concatExtents(
      Impl::sliceExtents(ATraits::extents(a), aSeqInv),
      Impl::sliceExtents(BTraits::extents(b), bSeqInv));
  }

  template <std::size_t N,
            Concept::TensorLike A,
            Concept::TensorLike B,
            class BinaryOp1 = std::plus<>, class BinaryOp2 = std::multiplies<>>
  static constexpr auto extents (const A& a, const B& b,
                                std::integral_constant<std::size_t,N> axes = {})
  {
    using ATraits = TensorTraits<A>;
    using SeqI = typename StaticIntegralRange<std::size_t,ATraits::rank(),ATraits::rank()-N>::integer_sequence;
    using SeqJ = std::make_index_sequence<N>;
    return extents(a, SeqI{}, b, SeqJ{});
  }

  template <Concept::TensorLike A, Concept::TensorLike B,
            class BinaryOp1 = std::plus<>, class BinaryOp2 = std::multiplies<>>
  static constexpr auto zero (const A& a, const B& b, BinaryOp1 op1 = {}, BinaryOp2 op2 = {})
  {
    using ATraits = TensorTraits<A>;
    using BTraits = TensorTraits<B>;

    using VA = decltype(a[std::array<typename ATraits::index_type,ATraits::rank()>{}]);
    using VB = decltype(b[std::array<typename BTraits::index_type,BTraits::rank()>{}]);
    using V2 = std::invoke_result_t<BinaryOp2,VA,VB>;
    using V1 = std::invoke_result_t<BinaryOp1,V2,V2>;
    return zero<V1>();
  }
};

/**
 * \brief Product of two tensors, contracted over indices `II...` of tensor `A` and `JJ...` of tensor `B`.
 *
 * Product of tensors `A` and `B` with index contraction over positions `II...`
 * of `A` and positions `JJ...` of `B`. And output tensor is constructed with
 * rank corresponding to the number of indices remaining in `A` and `B`.
 *
 * The product might be accumulated to the output tensor using outer binary operation
 * `op1`, e.g., a plus functor, and the inner binary operation `op2`, e.g., a
 * multiplies functor, see also the `std::inner_product` algorithm.
 *
 * \b Examples:
 * - outer product of 2-tensors: `c(i,j,k,l) = a(i,j) * b(k,l)`
 *     is `tensordot(a,index_sequence<>{},b,index_sequence<>{})`
 * - matrix-matrix product: `c(i,j) = a(i,k) * b(k,j)`
 *     is `tensordot(a,index_sequence<1>{},b,index_sequence<0>{})`
 * - inner product of 2-tensors: `c = a(i,j) * b(i,j)`
 *     is `tensordot(a,index_sequence<0,1>{},b,index_sequence<0,1>{})`
 */
template <Concept::TensorLike A, std::size_t... II,
          Concept::TensorLike B, std::size_t... JJ,
          class BinaryOp1 = std::plus<>, class BinaryOp2 = std::multiplies<>>
constexpr auto tensordot (const A& a, std::index_sequence<II...> aSeq,
                          const B& b, std::index_sequence<JJ...> bSeq,
                          BinaryOp1 op1 = {}, BinaryOp2 op2 = {})
{
  using ATraits = TensorTraits<A>;
  using BTraits = TensorTraits<B>;

  // create integer sequences that do not include the contraction indices
  const auto aSeqInv = difference<ATraits::rank()>(aSeq); // {0,1,...A::rank()-1} \ {II...}
  const auto bSeqInv = difference<BTraits::rank()>(bSeq); // {0,1,...B::rank()-1} \ {JJ...}

  auto c = DenseTensor{
    TensorDotTraits::zero(a,b,std::ref(op1),std::ref(op2)),
    TensorDotTraits::extents(a, aSeq, b, bSeq)};
  Impl::tensorDotImpl(a,aSeq,aSeqInv,b,bSeq,bSeqInv,c,std::ref(op1),std::ref(op2));
  return c;
}


/**
 * \brief Product of two tensors `A` and `B` contracted over the last `N` indices
 * of `A` and the first `N` indices of `B`
 *
 * Sum over the last `N` axes of `a` and the first `N` axes of `b`. Returns a tensor of
 * rank `A::rank()-N + B::rank()-N`.
 *
 * \b Examples:
 * - outer product of 2-tensors: `c(i,j,k,l) = a(i,j) * b(k,l)` is `tensordot<0>(a,b)`
 * - matrix-matrix product: `c(i,j) = a(i,k) * b(k,j)` is `tensordot<1>(a,b)`
 * - inner product of 2-tensors: `c = a(i,j) * b(i,j)` is `tensordot<2>(a,b)`
 **/
template <std::size_t N,
          Concept::TensorLike A,
          Concept::TensorLike B,
          class BinaryOp1 = std::plus<>, class BinaryOp2 = std::multiplies<>>
constexpr auto tensordot (const A& a, const B& b,
                          std::integral_constant<std::size_t,N> axes = {},
                          BinaryOp1 op1 = {}, BinaryOp2 op2 = {})
{
  using ATraits = TensorTraits<A>;
  using SeqI = typename StaticIntegralRange<std::size_t,ATraits::rank(),ATraits::rank()-N>::integer_sequence;
  using SeqJ = std::make_index_sequence<N>;
  return tensordot(a,SeqI{},b,SeqJ{},op1,op2);
}

} // end namespace Dune

#endif // DUNE_COMMON_TENSORDOT_HH
