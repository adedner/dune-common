// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_MPFRFIELD_HH
#define DUNE_COMMON_MPFRFIELD_HH

/** \file
 * \brief Wrapper for the GNU MPFR multiprecision floating point library
 */

#include <iostream>
#include <string>
#include <type_traits>

#if HAVE_MPFR || DOXYGEN

#include <mpreal.h>

#include <dune/common/promotiontraits.hh>
#include <dune/common/typetraits.hh>

namespace Dune
{
  /**
   * \ingroup Numbers
   * \brief Number class for high precision floating point number using the MPFR library mpreal implementation
   */
  template< unsigned int precision >
  class MPFRField
    : public mpfr::mpreal
  {
    using Base = mpfr::mpreal;

    static_assert(precision > 0 && precision < std::numeric_limits<mp_prec_t>::max());

  public:
    //! default constructor, initialize to zero.
    MPFRField ()
      : Base(0, mp_prec_t(precision))
    {}

    /**
     * \brief initialize from a string
     * \note this is the only reliable way to initialize with higher precision values
     */
    explicit MPFRField (const char* str)
      : Base(str, mp_prec_t(precision))
    {}

    /**
     * \brief initialize from a string
     * \note this is the only reliable way to initialize with higher precision values
     */
    explicit MPFRField (const std::string& str)
      : Base(str, mp_prec_t(precision))
    {}

    //! initialize from from mpreal value.
    MPFRField (const Base& v)
      : Base(v)
    {}

    //! initialize from a compatible scalar type.
    template <class T,
      std::enable_if_t<std::is_arithmetic_v<T>,int> = 0>
    MPFRField (const T& v)
      : Base(v, mp_prec_t(precision))
    {}

    MPFRField (const MPFRField&) = default;
    MPFRField (MPFRField&&) = default;

    MPFRField& operator= (MPFRField const&) = default;
    MPFRField& operator= (MPFRField&&) = default;

    //! type conversion operators
    operator double () const
    {
      return this->toDouble();
    }
  };

  template <unsigned int precision>
  struct IsNumber<MPFRField<precision>>
    : public std::true_type {};

  template <unsigned int precision1, unsigned int precision2>
  struct PromotionTraits<MPFRField<precision1>, MPFRField<precision2>>
  {
    using PromotedType = MPFRField<(precision1 > precision2 ? precision1 : precision2)>;
  };

  template <unsigned int precision>
  struct PromotionTraits<MPFRField<precision>,MPFRField<precision>>
  {
    using PromotedType = MPFRField<precision>;
  };

  template <unsigned int precision, class T>
  struct PromotionTraits<MPFRField<precision>, T>
  {
    using PromotedType = MPFRField<std::max((unsigned int)(8*sizeof(T)), precision)>;
  };

  template <class T, unsigned int precision>
  struct PromotionTraits<T, MPFRField<precision>>
  {
    using PromotedType = MPFRField<std::max((unsigned int)(8*sizeof(T)), precision)>;
  };
}

namespace std
{
  /// Specialization of numeric_limits for known precision width
  template <unsigned int precision>
  class numeric_limits<Dune::MPFRField<precision>>
      : public numeric_limits<mpfr::mpreal>
  {
    using type = Dune::MPFRField<precision>;

  public:
    inline static type min () { return mpfr::minval(precision); }
    inline static type max () {  return  mpfr::maxval(precision); }
    inline static type lowest () { return -mpfr::maxval(precision); }
    inline static type epsilon () { return  mpfr::machine_epsilon(precision); }

    inline static type round_error ()
    {
      mp_rnd_t r = mpfr::mpreal::get_default_rnd();

      if(r == GMP_RNDN)  return mpfr::mpreal(0.5, precision);
      else               return mpfr::mpreal(1.0, precision);
    }

    inline static int digits () { return int(precision); }
    inline static int digits10 () { return mpfr::bits2digits(precision); }
    inline static int max_digits10 () { return mpfr::bits2digits(precision); }
  };

} // end namespace std

#endif // HAVE_MPFR

#endif // DUNE_COMMON_MPFRFIELD_HH
