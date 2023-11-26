// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_PARMETIS_HH
#define DUNE_PARMETIS_HH

#if HAVE_PARMETIS

#include <dune/common/parallel/mpi.hh>

#if HAVE_PTSCOTCH_PARMETIS
extern "C" {
  #include <ptscotch.h>
}
#endif

extern "C" {
  #include <parmetis.h>
}

namespace Dune::ParMetis {

#if defined(REALTYPEWIDTH)
  using real_t = ::real_t;
#else
  using real_t = float;
#endif

#if defined(IDXTYPEWIDTH)
  using idx_t = ::idx_t;
#elif HAVE_PTSCOTCH_PARMETIS
  using idx_t = SCOTCH_Num;
#else
  using idx_t = int;
#endif

} // end namespace Dune::ParMetis

#endif // HAVE_PARMETIS
#endif // DUNE_PARMETIS_HH
