// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_CONCEPTS_LINEARMAP_HH
#define DUNE_CONCEPTS_LINEARMAP_HH

#if __has_include(<concepts>) || DOXYGEN
#include <concepts>

#include <dune/common/concepts/vectorspace.hh>

namespace Dune {
namespace Concept {

template <class L, class X, class Y>
concept LinearMap = VectorSpace<X> && VectorSpace<Y>
  && requires(const L& A, const X& x, Y& y, typename L::field_type alpha)
{
  A.mv(x, y);           // y = A x
  A.umv(x, y);          // y+= A x
  A.mmv(x, y);          // y-= A x
  A.usmv(alpha, x, y);  // y+= alpha A x
};

template <class L, class X, class Y>
concept TransposableLinearMap = LinearMap<L,X,Y>
  && requires(const L& A, const X& x, Y& y, typename L::field_type alpha)
{
  A.mtv(x, y);          // y = A^T x
  A.umtv(x, y);         // y+= A^T x
  A.mmtv(x, y);         // y-= A^T x
  A.usmtv(alpha, x, y); // y+= alpha A^T x
};

template <class L, class X, class Y>
concept HermitianLinearMap = LinearMap<L,X,Y>
  && requires(const L& A, const X& x, Y& y, typename L::field_type alpha)
{
//  A.mhv(x, y);          // y = A^H x (this operation does not exist)
  A.umhv(x, y);         // y+= A^H x
  A.mmhv(x, y);         // y-= A^H x
  A.usmhv(alpha, x, y); // y+= alpha A^H x
};


}} // end namespace Dune::Concept

#endif // __has_include(<concepts>)
#endif // DUNE_CONCEPTS_LINEARMAP_HH
