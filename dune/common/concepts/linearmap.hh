// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_CONCEPTS_LINEARMAP_HH
#define DUNE_CONCEPTS_LINEARMAP_HH

#if __has_include(<concepts>) || DOXYGEN
#include <concepts>

#include <dune/common/ftraits.hh>
#include <dune/common/concepts/vectorspace.hh>

namespace Dune {
namespace Concept {

/**
 * \ingroup CxxConcepts
 * @{
 **/

/// \brief Linear map between vector spaces.
/**
 * A linear map `L:X->Y` from vector space `X` to vector space `Y`
 * is an operator providing matrix-vector multiplication operations.
 *
 * \par Notation:
 * Let `A` be a linear map of type `L`, `x` of type `X` and `y` of type `Y`.
 * Additionally, let `alpha` be a scalar factor of type `field_t<L>`.
 *
 * \par Refinement:
 * - `X` is a model of \ref VectorSpace
 * - `Y` is a model of \ref VectorSpace
 *
 * \par Valid expressions:
 * - `A.mv(x,y)`: Matrix-vector product `y = A x`
 * - `A.umv(x,y)`: Matrix-vector product with update `y+= A x`
 * - `A.mmv(x,y)`: Matrix-vector product with minus-update `y-= A x`
 * - `A.usmv(alpha,x,y)`: Scaled matrix-vector product with update `y+= alpha A x`
 *
 * \hideinitializer
 **/
template <class L, class X, class Y>
concept LinearMap = VectorSpace<X> && VectorSpace<Y>
  && requires(const L& A, const X& x, Y& y, typename Dune::FieldTraits<L>::field_type alpha)
{
  A.mv(x, y);           // y = A x
  A.umv(x, y);          // y+= A x
  A.mmv(x, y);          // y-= A x
  A.usmv(alpha, x, y);  // y+= alpha A x
};


/// \brief Linear map between vector spaces with the transposed operator.
/**
 * A linear map `L^T:X->Y` from vector space `X` to vector space `Y`
 * is an operator providing matrix-vector multiplication operations.
 *
 * \par Notation:
 * Let `A` be a linear map of type `L`, `x` of type `X` and `y` of type `Y`.
 * Additionally, let `alpha` be a scalar factor of type `field_t<L>`.
 *
 * \par Refinement:
 * - `X` is a model of \ref VectorSpace
 * - `Y` is a model of \ref VectorSpace
 *
 * \par Valid expressions:
 * - `A.mtv(x,y)`: Matrix-vector product `y = A^T x`
 * - `A.umtv(x,y)`: Matrix-vector product with update `y+= A^T x`
 * - `A.mmtv(x,y)`: Matrix-vector product with minus-update `y-= A^T x`
 * - `A.usmtv(alpha,x,y)`: Scaled matrix-vector product with update `y+= alpha A^T x`
 *
 * \hideinitializer
 **/
template <class L, class X, class Y>
concept TransposableLinearMap = LinearMap<L,X,Y>
  && requires(const L& A, const X& x, Y& y, typename Dune::FieldTraits<L>::field_type alpha)
{
  A.mtv(x, y);          // y = A^T x
  A.umtv(x, y);         // y+= A^T x
  A.mmtv(x, y);         // y-= A^T x
  A.usmtv(alpha, x, y); // y+= alpha A^T x
};


/// \brief Linear map between vector spaces with the hermitian operator.
/**
 * A linear map `L^H:X->Y` from vector space `X` to vector space `Y`
 * is an operator providing matrix-vector multiplication operations.
 *
 * \par Notation:
 * Let `A` be a linear map of type `L`, `x` of type `X` and `y` of type `Y`.
 * Additionally, let `alpha` be a scalar factor of type `field_t<L>`.
 *
 * \par Refinement:
 * - `X` is a model of \ref VectorSpace
 * - `Y` is a model of \ref VectorSpace
 *
 * \par Valid expressions:
 * - `A.umhv(x,y)`: Matrix-vector product with update `y+= A^H x`
 * - `A.mmhv(x,y)`: Matrix-vector product with minus-update `y-= A^H x`
 * - `A.usmhv(alpha,x,y)`: Scaled matrix-vector product with update `y+= alpha A^H x`
 *
 * \hideinitializer
 **/
template <class L, class X, class Y>
concept HermitianLinearMap = LinearMap<L,X,Y>
  && requires(const L& A, const X& x, Y& y, typename Dune::FieldTraits<L>::field_type alpha)
{
//  A.mhv(x, y);          // y = A^H x (this operation does not exist)
  A.umhv(x, y);         // y+= A^H x
  A.mmhv(x, y);         // y-= A^H x
  A.usmhv(alpha, x, y); // y+= alpha A^H x
};

/** @} */

}} // end namespace Dune::Concept

#endif // __has_include(<concepts>)
#endif // DUNE_CONCEPTS_LINEARMAP_HH
