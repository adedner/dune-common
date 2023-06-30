// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
/* begin dune-common
   put the definitions for config.h specific to
   your project here. Everything above will be
   overwritten
*/

/* begin private */
/* Define to the version of dune-common */
#define DUNE_COMMON_VERSION "${DUNE_COMMON_VERSION}"

/* Define to the major version of dune-common */
#define DUNE_COMMON_VERSION_MAJOR ${DUNE_COMMON_VERSION_MAJOR}

/* Define to the minor version of dune-common */
#define DUNE_COMMON_VERSION_MINOR ${DUNE_COMMON_VERSION_MINOR}

/* Define to the revision of dune-common */
#define DUNE_COMMON_VERSION_REVISION ${DUNE_COMMON_VERSION_REVISION}

/* Standard debug streams with a level below will collapse to doing nothing */
#define DUNE_MINIMAL_DEBUG_LEVEL ${DUNE_MINIMAL_DEBUG_LEVEL}

/* does the standard library provide experimental::is_detected ? */
#cmakedefine DUNE_HAVE_CXX_EXPERIMENTAL_IS_DETECTED 1

/* does the language support lambdas in unevaluated contexts ? */
#cmakedefine DUNE_HAVE_CXX_UNEVALUATED_CONTEXT_LAMBDA 1

/* does the standard library provide identity ? */
#cmakedefine DUNE_HAVE_CXX_STD_IDENTITY 1

/* Deactivate cxx bindings for MPI */
#if defined(HAVE_MPI) && HAVE_MPI
#define MPICH_SKIP_MPICXX 1
#define OMPI_SKIP_MPICXX 1
#define MPI_NO_CPPBIND 1
#define MPIPP_H
#define _MPICC_H
#endif

/* begin private */

/* Name of package */
#define PACKAGE "dune-common"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "@DUNE_MAINTAINER@"

/* Define to the full name of this package. */
#define PACKAGE_NAME "@DUNE_MOD_NAME@"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "@DUNE_MOD_NAME@ @DUNE_MOD_VERSION@"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "@DUNE_MOD_NAME@"

/* Define to the home page for this package. */
#define PACKAGE_URL "@DUNE_MOD_URL@"

/* Define to the version of this package. */
#define PACKAGE_VERSION "@DUNE_MOD_VERSION@"

/* Version number of package */
#define VERSION "@DUNE_MOD_VERSION@"

/* end private */


/* old feature support macros which were tested until 2.9, kept around for one more release */
#define DUNE_HAVE_CXX_EXPERIMENTAL_MAKE_ARRAY 0

/* Define to ENABLE_UMFPACK if the UMFPack library is available */
#define HAVE_UMFPACK HAVE_SUITESPARSE

/* Define to HAVE_SUITESPARSE if the SuiteSparse's AMD library is available */
#define HAVE_SUITESPARSE_AMD HAVE_SUITESPARSE
/* Define to HAVE_SUITESPARSE if the SuiteSparse's BTF library is available */
#define HAVE_SUITESPARSE_BTF HAVE_SUITESPARSE
/* Define to HAVE_SUITESPARSE if the SuiteSparse's CAMD library is available */
#define HAVE_SUITESPARSE_CAMD HAVE_SUITESPARSE
/* Define to HAVE_SUITESPARSE if the SuiteSparse's CCOLAMD library is available */
#define HAVE_SUITESPARSE_CCOLAMD HAVE_SUITESPARSE
/* Define to HAVE_SUITESPARSE if the SuiteSparse's CHOLMOD library is available */
#define HAVE_SUITESPARSE_CHOLMOD HAVE_SUITESPARSE
/* Define to HAVE_SUITESPARSE if the SuiteSparse's COLAMD library is available */
#define HAVE_SUITESPARSE_COLAMD HAVE_SUITESPARSE
/* Define to HAVE_SUITESPARSE if the SuiteSparse's CXSPARSE library is available */
#define HAVE_SUITESPARSE_CXSPARSE HAVE_SUITESPARSE
/* Define to HAVE_SUITESPARSE if the SuiteSparse's KLU library is available */
#define HAVE_SUITESPARSE_KLU HAVE_SUITESPARSE
/* Define to HAVE_SUITESPARSE if the SuiteSparse's LDL library is available */
#define HAVE_SUITESPARSE_LDL HAVE_SUITESPARSE
/* Define to HAVE_SUITESPARSE if the SuiteSparse's RBIO library is available */
#define HAVE_SUITESPARSE_RBIO HAVE_SUITESPARSE
/* Define to HAVE_SUITESPARSE if the SuiteSparse's SPQR library is available
   and if it's version is at least 4.3 */
#define HAVE_SUITESPARSE_SPQR HAVE_SUITESPARSE
/* Define to HAVE_SUITESPARSE if the SuiteSparse's UMFPACK library is available */
#define HAVE_SUITESPARSE_UMFPACK HAVE_SUITESPARSE

/* Define to 1 if the Scotch replacement for METIS is used. */
#cmakedefine HAVE_SCOTCH_METIS 1

/* Define to 1 if the PTScotch replacement for ParMETIS is used. */
#cmakedefine HAVE_PTSCOTCH_PARMETIS 1

/* end dune-common
   Everything below here will be overwritten
*/
