// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_CONCEPTS_VECTORSPACE_HH
#define DUNE_CONCEPTS_VECTORSPACE_HH

#if __has_include(<concepts>)
#include <concepts>
#include <cmath>

#include <dune/common/ftraits.hh>
#include <dune/common/typetraits.hh>

namespace Dune {
namespace Concept {

template <class K>
concept Scalar = Dune::IsNumber<K>::value;

template <class X, class F = typename Dune::FieldTraits<X>::field_type>
concept VectorSpace = requires(X& x, const X& y, F alpha)
{
  requires Scalar<F>;
  x += y;
  x -= y;
  x *= alpha;
  x /= alpha;

  x + y;
  x - y;
  -x;

  x.axpy(alpha, y); // x += alpha y
};


// Normed spaces

struct OneNorm
{
  template <class X>
  auto operator()(const X& x) const -> decltype(x.one_norm()) { return x.one_norm(); }
};

struct TwoNorm
{
  template <class X>
  auto operator()(const X& x) const -> decltype(x.two_norm()) { return x.two_norm(); }
};

struct FrobeniusNorm
{
  template <class X>
  auto operator()(const X& x) const -> decltype(x.frobenius_norm()) { return x.frobenius_norm(); }
};

struct InfinityNorm
{
  template <class X>
  auto operator()(const X& x) const -> decltype(x.infinity_norm()) { return x.infinity_norm(); }
};

struct InfinityNormReal
{
  template <class X>
  auto operator()(const X& x) const -> decltype(x.infinity_norm_real()) { return x.infinity_norm_real(); }
};

template <class N, class X,
          class R = typename Dune::FieldTraits<X>::real_type>
concept NormedSpace = requires(N norm, const X& x)
{
  requires Scalar<R>; // maybe std::arithmetic
  { norm(x) } -> std::convertible_to<R>;
};

template <class N, class X,
          class F = typename Dune::FieldTraits<X>::field_type,
          class R = typename Dune::FieldTraits<X>::real_type>
concept BanachSpace = VectorSpace<X,F> && NormedSpace<N,X,R>;


// Inner-product spaces

struct Dot
{
  template <class X, class Y>
  auto operator()(const X& x, const Y& y) const -> decltype(x.dot(y)) { return x.dot(y); }
};

struct TDot
{
  template <class X, class Y>
  auto operator()(const X& x, const Y& y) const -> decltype(x*y) { return x*y; }
};

template <class I, class X,
          class F = typename Dune::FieldTraits<X>::field_type>
concept InnerProductSpace = requires(I inner, const X& x, const X& y)
{
  requires Scalar<F>;
  { inner(x,y) } -> std::convertible_to<F>;
};


template <class I>
struct InducedNorm;

template <>
struct InducedNorm<Dot>
{
  using type = TwoNorm;
};

template <class I>
using InducedNorm_t = typename InducedNorm<I>::type;

template <class I, class X,
          class F = typename Dune::FieldTraits<X>::field_type,
          class R = typename Dune::FieldTraits<X>::real_type>
concept HilbertSpace = BanachSpace<InducedNorm_t<I>,X,F,R> && InnerProductSpace<I,X,F>;

}} // end namespace Dune::Concept

#endif // __has_include(<concepts>)
#endif // DUNE_CONCEPTS_VECTORSPACE_HH
