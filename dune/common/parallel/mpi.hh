// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_PARALLEL_MPI_HH
#define DUNE_COMMON_PARALLEL_MPI_HH

#if HAVE_MPI

/* Deactivate cxx bindings for MPI */
#define MPICH_SKIP_MPICXX 1
#define OMPI_SKIP_MPICXX 1
#define MPI_NO_CPPBIND 1
#define MPIPP_H
#define _MPICC_H

#include <mpi.h>

#endif // HAVE_MPI

#endif // DUNE_COMMON_PARALLEL_MPI_HH
