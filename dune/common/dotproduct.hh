// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_DOTPRODUCT_HH
#define DUNE_DOTPRODUCT_HH

#include <type_traits>
#include <dune/common/conj.hh>
#include <dune/common/typetraits.hh>
#include <dune/common/std/type_traits.hh>

namespace Dune {
namespace Impl {

template <class A, class B, bool with_conj = true>
struct DotImpl
{
private:
  template <class A_, class B_>
  static auto applyImpl (const A_& a, const B_& b, std::true_type, Dune::PriorityTag<3>)
    -> decltype(a.dot(b))
  {
    return a.dot(b);
  }

  template <class A_, class B_>
  static auto applyImpl (const A_& a, const B_& b, std::false_type, Dune::PriorityTag<2>)
    -> decltype(a.dotT(b))
  {
    return a.dotT(b);
  }

  template <class A_, class B_, class WithConj>
  static auto applyImpl (const A_& a, const B_& b, WithConj, Dune::PriorityTag<1>)
    -> decltype(a*b)
  {
    using Dune::conj;
    if constexpr(WithConj::value && Dune::IsNumber<A_>::value)
      return conj(a) * b;
    else
      return a * b;
  }

public:
  static auto apply (const A& a, const B& b)
  {
    return applyImpl(a,b,std::bool_constant<with_conj>{},Dune::PriorityTag<5>{});
  }
};

} // end namespace Impl

//! Computes the dot product for various vector types according to Petsc's VecDot function
/**
 * dot(a,b) := \f$a^H \cdot b \f$
 **/
template <class A, class B>
auto dot (const A& a, const B& b) -> decltype(Impl::DotImpl<A,B>::apply(a,b))
{
  return Impl::DotImpl<A,B>::apply(a,b);
}

//! Computes the dot product for various vector types according to Petsc's VecTDot function
/**
 * dotT(a,b) := \f$a^T \cdot b \f$
 **/
template <class A, class B>
auto dotT (const A& a, const B& b) -> decltype(Impl::DotImpl<A,B,false>::apply(a,b))
{
  return Impl::DotImpl<A,B,false>::apply(a,b);
}

} // end namespace Dune

#endif // DUNE_DOTPRODUCT_HH
