// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_CONJ_HH
#define DUNE_CONJ_HH

#include <complex>
#include <type_traits>

#include <dune/common/typetraits.hh>

namespace Dune {
namespace Impl {

// default implementation is identity
template <class T>
struct ConjImpl
{
  static T apply (const T& value)
  {
    return value;
  }
};

// specialization for std::complex type
template <class T>
struct ConjImpl<std::complex<T>>
{
  static std::complex<T> apply (const std::complex<T>& value)
  {
    using std::conj;
    return conj(value);
  }
};

} // end namespace Impl

template <class T,
  std::enable_if_t<Dune::IsNumber<T>::value, int> = 0>
auto conj (const T& value)
{
  return Impl::ConjImpl<T>::apply(value);
}

inline float conj (float value) { return value; }
inline double conj (double value) { return value; }
inline long double conj (long double value) { return value; }

} // end namespace Dune

#endif // DUNE_CONJ_HH
