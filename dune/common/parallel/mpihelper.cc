// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

#include "mpihelper.hh"

namespace Dune {

auto
FakeMPIHelper::getCommunicator() -> MPICommunicator
{
  static MPICommunicator comm;
  return comm;
}

auto
FakeMPIHelper::getLocalCommunicator() -> MPICommunicator
{
  return getCommunicator();
}

[[deprecated("getCollectionCommunication is deprecated. Use getCommunication instead.")]] auto
FakeMPIHelper::getCollectiveCommunication() -> Communication<MPICommunicator>
{
  return Communication<MPICommunicator>(getCommunicator());
}

auto
FakeMPIHelper::getCommunication() -> Communication<MPICommunicator>
{
  return Communication<MPICommunicator>(getCommunicator());
}

FakeMPIHelper&
FakeMPIHelper::instance(int argc, char** argv)
{
  return instance();
}

FakeMPIHelper&
FakeMPIHelper::instance()
{
  static FakeMPIHelper singleton;
  return singleton;
}

int
FakeMPIHelper::rank() const
{
  return 0;
}

int
FakeMPIHelper::size() const
{
  return 1;
}

#if HAVE_MPI

static inline std::atomic<MPIHelper*> mpi_helper_instance_ptr_ = nullptr;

MPICommunicator
MPIHelper::getCommunicator()
{
  return MPI_COMM_WORLD;
}

MPICommunicator
MPIHelper::getLocalCommunicator()
{
  return MPI_COMM_SELF;
}

[[deprecated("getCollectionCommunication is deprecated. Use getCommunication "
             "instead.")]] auto
MPIHelper::getCollectiveCommunication() -> Communication<MPICommunicator>
{
  return Communication<MPICommunicator>(getCommunicator());
}

auto
MPIHelper::getCommunication() -> Communication<MPICommunicator>
{
  return Communication<MPICommunicator>(getCommunicator());
}

MPIHelper&
MPIHelper::instance(int& argc, char**& argv)
{
  // create singleton instance
  static MPIHelper instance(argc, argv);
  static std::once_flag instance_flag;
  std::call_once(instance_flag,
                 [&]() { mpi_helper_instance_ptr_.store(&instance); });
  return instance;
}

MPIHelper&
MPIHelper::instance()
{
  if (auto ptr = mpi_helper_instance_ptr_.load())
    return *ptr;
  DUNE_THROW(InvalidStateException,
             "MPIHelper not initialized! Call MPIHelper::instance(argc, "
             "argv) with arguments first.");
}

int
MPIHelper::rank() const
{
  return rank_;
}

int
MPIHelper::size() const
{
  return size_;
}

MPIHelper::~MPIHelper()
{
  int wasFinalized = -1;
  MPI_Finalized(&wasFinalized);
  if (!wasFinalized && initializedHere_) {
    MPI_Finalize();
    dverb << "Called MPI_Finalize on p=" << rank_ << "!" << std::endl;
  }
}

MPIHelper::MPIHelper(int& argc, char**& argv)
  : initializedHere_(false)
{
  int wasInitialized = -1;
  MPI_Initialized(&wasInitialized);
  if (!wasInitialized) {
    rank_ = -1;
    size_ = -1;
    static int is_initialized = MPI_Init(&argc, &argv);
    prevent_warning(is_initialized);
    initializedHere_ = true;
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
  MPI_Comm_size(MPI_COMM_WORLD, &size_);

  assert(rank_ >= 0);
  assert(size_ >= 1);

  dverb << "Called  MPI_Init on p=" << rank_ << "!" << std::endl;
}

#endif
} // namespace Dune
