// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_FTRAITS_HH
#define DUNE_FTRAITS_HH

/** \file
 * \brief Type traits to determine the type of reals (when working with complex numbers)
 */

#include <complex>
#include <vector>

#include <dune/common/typetraits.hh>

namespace Dune {

#ifndef DOXYGEN

namespace Impl {

  template<class T, bool isNumber>
  struct FieldTraitsDefault {};

  template<class T>
  struct FieldTraitsDefault<T, true>
  {
    typedef T field_type;
    typedef T real_type;
  };
} // namespace Impl

  template<class T>
  struct FieldTraits : public Impl::FieldTraitsDefault<T, Dune::IsNumber<T>::value>
  {};

#else

  /**
     @addtogroup DenseMatVec
     \brief Type traits to retrieve the field and the real type of classes

     Type traits to retrieve the field and the real type of classes
     e.g. that of FieldVector or FieldMatrix
   */
  template<class T>
  struct FieldTraits
  {
    //! export the type representing the field
    typedef T field_type;
    //! export the type representing the real type of the field
    typedef T real_type;
  };

#endif

  template<class T>
  struct FieldTraits<const T>
  {
    typedef typename FieldTraits<T>::field_type field_type;
    typedef typename FieldTraits<T>::real_type real_type;
  };

  template<class T>
  struct FieldTraits< std::complex<T> >
  {
    typedef std::complex<T> field_type;
    typedef T real_type;
  };

  template<class T, unsigned int N>
  struct FieldTraits< T[N] >
  {
    typedef typename FieldTraits<T>::field_type field_type;
    typedef typename FieldTraits<T>::real_type real_type;
  };

  template<class T>
  struct FieldTraits< std::vector<T> >
  {
    typedef typename FieldTraits<T>::field_type field_type;
    typedef typename FieldTraits<T>::real_type real_type;
  };

} // end namespace Dune

#endif // DUNE_FTRAITS_HH
