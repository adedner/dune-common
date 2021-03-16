// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_CONCEPTS_VECTORSPACE_HH
#define DUNE_CONCEPTS_VECTORSPACE_HH

#if __has_include(<concepts>) || DOXYGEN
#include <concepts>
#include <cmath>

#include <dune/common/ftraits.hh>
#include <dune/common/typetraits.hh>

namespace Dune {
namespace Concept {

/**
 * \ingroup CxxConcepts
 * @{
 **/

template <class K>
concept Scalar = Dune::IsNumber<K>::value;

/// \brief A VectorSpace over a field is a collection of vectors with arithmetic operations.
/**
 * We define a vector-space as a type `X` that allows unary and binary arithmetic operations,
 * like `+` and `-`, as well as compound assignment operations `+=` and `-=`. Additionally, vectors
 * can be scaled by a scalar factor of the associated field type.
 *
 * \par Associated types:
 * - `typename Dune::FieldTraits<X>::field_type`: a model of \ref Scalar
 *
 * \par Notation:
 * - `x,y`: objects of type `X`.
 * - `alpha`: scalar object of type `F = Dune::FieldTraits<X>::field_type`.
 *
 * \par Valid expressions:
 * - `x.axpy(alpha, y)`: The operation `x += alpha y`.
 * - `x + y`, `x += y`: Addition of vectors.
 * - `x - y`, `x -= y`: Subtraction of vectors.
 * - `-x`: Negation. Is equivalent to `-1 * x`.
 * - `x *= alpha`: Multiplication with scalar.
 * - `x /= alpha`: Division by scalar. Is equivalent to multiplication with the
 *   inverse `1.0/alpha` (if `F` is a floating-point or complex type).
 *
 * \noop The following expression should be valid but are not implemented by many types:
 * \noop - `x * alpha`, `alpha * x`, `x *= alpha`: Multiplication with scalar.
 * \noop - `x / alpha`, `x /= alpha`: Division by scalar. Is equivalent to multiplication with the
 * \noop   inverse `1.0/alpha` (if `F` is a floating-point or complex type).
 *
 * \par Models:
 * - \ref FieldVector
 * - \ref FieldMatrix
 * - \ref DynamicVector
 * - \ref DynamicMatrix
 *
 * \hideinitializer
 **/
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

  // x * alpha;
  // x / alpha;
  // alpha * x;

  x.axpy(alpha, y); // x += alpha y
};


// Normed spaces

/// \brief Traits class represent norm functors
/**
 * Boolean traits class where norm concept-maps can be registered.
 *
 * \par Models:
 * - \ref OneNorm
 * - \ref TwoNorm
 * - \ref FrobeniusNorm
 * - \ref InfinityNorm
 * - \ref InfinityNormReal
 **/
template <class N>
struct IsNorm : std::false_type {};

struct OneNorm
{
  template <class X>
  auto operator()(const X& x) const -> decltype(x.one_norm()) { return x.one_norm(); }
};
#ifndef DOXYGEN
template <> struct IsNorm<OneNorm> : std::true_type {};
#endif

struct TwoNorm
{
  template <class X>
  auto operator()(const X& x) const -> decltype(x.two_norm()) { return x.two_norm(); }
};
#ifndef DOXYGEN
template <> struct IsNorm<TwoNorm> : std::true_type {};
#endif

struct FrobeniusNorm
{
  template <class X>
  auto operator()(const X& x) const -> decltype(x.frobenius_norm()) { return x.frobenius_norm(); }
};
#ifndef DOXYGEN
template <> struct IsNorm<FrobeniusNorm> : std::true_type {};
#endif

struct InfinityNorm
{
  template <class X>
  auto operator()(const X& x) const -> decltype(x.infinity_norm()) { return x.infinity_norm(); }
};
#ifndef DOXYGEN
template <> struct IsNorm<InfinityNorm> : std::true_type {};
#endif

struct InfinityNormReal
{
  template <class X>
  auto operator()(const X& x) const -> decltype(x.infinity_norm_real()) { return x.infinity_norm_real(); }
};
#ifndef DOXYGEN
template <> struct IsNorm<InfinityNormReal> : std::true_type {};
#endif


/// \brief A Norm is function applied to elements of a vector space.
/**
 * Typically, norm functions are implemented in terms of member functions. Those can be checked
 * using one of the functors \ref OneNorm, \ref TwoNorm, \ref FrobeniusNorm, \ref InfinityNorm,
 * \ref InfinityNormReal. See its definition for details. The norm `N` must be registered in
 * the type-trait class \ref IsNorm.
 *
 * \par Associated types:
 * - `typename Dune::FieldTraits<X>::field_type`: a model of \ref Scalar
 *
 * \par Notation:
 * - `norm`: an instance of the norm functor `N`.
 * - `x`: object of type `X`.
 * - `a`: scalar of field type.
 *
 * \par Valid expressions:
 * - `norm(x)`: The norm function evaluated with argument `x`.
 *
 * \par Invariants:
 * - Satisfies the triangle inequality: `norm(x + y) <= norm(x) + norm(y)`
 * - Is absolute homogeneous: `norm(a * x) == abs(a) * norm(x)`
 * - Positive definiteness: `if norm(x) == 0 then x == 0` (zero vector)
 *
 * \hideinitializer
 **/
template <class N, class X,
          class R = typename Dune::FieldTraits<X>::real_type>
concept Norm = requires(N norm, const X& x)
{
  requires IsNorm<N>::value;
  requires Scalar<R>; // maybe std::arithmetic
  { norm(x) } -> std::convertible_to<R>;
};

/// \brief A BanachSpace is a VectorSpace with norm.
template <class X, class N = TwoNorm,
          class F = typename Dune::FieldTraits<X>::field_type,
          class R = typename Dune::FieldTraits<X>::real_type>
concept BanachSpace = VectorSpace<X,F> && Norm<N,X,R>;


// Inner-product spaces

/// \brief Traits class represent inner-product functors
/**
 * Boolean traits class where inner-product concept-maps can be registered.
 *
 * \par Models:
 * - \ref Dot
 * - \ref TDot
 **/
template <class I>
struct IsInnerProduct : std::false_type {};

struct Dot
{
  template <class X, class Y>
  auto operator()(const X& x, const Y& y) const -> decltype(x.dot(y)) { return x.dot(y); }
};
#ifndef DOXYGEN
template <> struct IsInnerProduct<Dot> : std::true_type {};
#endif

struct TDot
{
  template <class X, class Y>
  auto operator()(const X& x, const Y& y) const -> decltype(x*y) { return x*y; }
};
#ifndef DOXYGEN
template <> struct IsInnerProduct<TDot> : std::true_type {};
#endif


/// \brief An InnerProduct is a function applied to elements of a vector space.
/**
 * Typically, inner-product functions are implemented in terms of member functions of the vectors.
 * Those can be checked using one of the functors \ref Dot, \ref TDot. See its definition for
 * details. The inner-product `I` must be registered in the type-traits \ref IsInnerProduct.
 *
 * \par Associated types:
 * - `typename Dune::FieldTraits<X>::field_type`: a model of \ref Scalar
 *
 * \par Notation:
 * - `inner`: an instance of the norm functor `I`.
 * - `x,y`: object of type `X`.
 * - `a`: scalar of the field type.
 *
 * \par Valid expressions:
 * - `inner(x,y)`: The inner-product function evaluated with arguments `x` and `y`.
 *
 * \par Invariants:
 * - Linearity in the first argument: `inner(a * x, y) == a * inner(x, y)` and
 *   `inner(x + y, z) == inner(x, z) + inner(y, z)`
 * - Conjugate symmetry or Hermitian symmetry: `inner(x, y) == conj(inner(y, x))`
 * - Positive definiteness: `inner(x, x) > 0 if x != 0`
 *
 * \hideinitializer
 **/
template <class I, class X,
          class F = typename Dune::FieldTraits<X>::field_type>
concept InnerProduct = requires(I inner, const X& x, const X& y)
{
  requires IsInnerProduct<I>::value;
  requires Scalar<F>;
  { inner(x,y) } -> std::convertible_to<F>;
};


/// \brief Traits class that transforms an inner-product into a norm.
template <class I>
struct InducedNorm;

#ifndef DOXYGEN

template <>
struct InducedNorm<Dot>
{
  using type = TwoNorm;
};

template <class I>
using InducedNorm_t = typename InducedNorm<I>::type;

#endif // !DOXYGEN

/// \brief A HilbertSpace is a VectorSpace with inner-product and an induced norm.
template <class X, class I = Dot,
          class F = typename Dune::FieldTraits<X>::field_type,
          class R = typename Dune::FieldTraits<X>::real_type>
concept HilbertSpace = BanachSpace<X,InducedNorm_t<I>,F,R> && InnerProduct<I,X,F>;

/** @} */

}} // end namespace Dune::Concept

#endif // __has_include(<concepts>)
#endif // DUNE_CONCEPTS_VECTORSPACE_HH
