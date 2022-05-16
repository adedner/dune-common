dune_add_test(SOURCES communicationtest.cc
              MPI_RANKS 1 2 4
              TIMEOUT 300
              CMAKE_GUARD MPI_FOUND
              LABELS quick)
add_dune_mpi_flags(communicationtest)

dune_add_test(SOURCES indexsettest.cc
              LABELS quick)

dune_add_test(SOURCES remoteindicestest.cc
              MPI_RANKS 1 2 4
              TIMEOUT 300
              CMAKE_GUARD HAVE_MPI
              LABELS quick)
add_dune_mpi_flags(remoteindicestest)

dune_add_test(SOURCES selectiontest.cc
              LABELS quick)

dune_add_test(SOURCES syncertest.cc
              MPI_RANKS 1 2 4
              TIMEOUT 300
              CMAKE_GUARD HAVE_MPI
              LABELS quick)
add_dune_mpi_flags(syncertest)

dune_add_test(SOURCES variablesizecommunicatortest.cc
              MPI_RANKS 1 2 4
              TIMEOUT 300
              CMAKE_GUARD HAVE_MPI
              LABELS quick)
add_dune_mpi_flags(variablesizecommunicatortest)

dune_add_test(SOURCES mpidatatest.cc
              MPI_RANKS 2
              TIMEOUT 300
              LABELS quick)
add_dune_mpi_flags(mpidatatest)

dune_add_test(SOURCES mpifuturetest.cc
              CMAKE_GUARD MPI_FOUND
              MPI_RANKS 1 2 4
              TIMEOUT 300
              LABELS quick)
add_dune_mpi_flags(mpifuturetest)

dune_add_test(SOURCES mpipacktest.cc
              MPI_RANKS 2
              TIMEOUT 300
              CMAKE_GUARD HAVE_MPI
              LABELS quick)
add_dune_mpi_flags(mpipacktest)

dune_add_test(SOURCES mpigatherscattertest.cc
              MPI_RANKS 2
              TIMEOUT 300
              CMAKE_GUARD MPI_FOUND
              LABELS quick)
add_dune_mpi_flags(mpigatherscattertest)
