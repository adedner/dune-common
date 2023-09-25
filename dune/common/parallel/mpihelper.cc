// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

#include <config.h>

#include "mpihelper.hh"

#if HAVE_MPI
#include <cassert>
#include <dune/common/stdstreams.hh>
#include <mpi.h>
#endif

#if HAVE_MPI
#include <mutex>
#include <atomic>
#include <cassert>
#include <mpi.h>
#endif

#if HAVE_MPI
#include <dune/common/parallel/mpicommunication.hh>
#include <dune/common/stdstreams.hh>
#endif

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

[[deprecated("getCollectionCommunication is deprecated. Use getCommunication "
             "instead.")]] auto
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

static inline std::atomic<MPIHelper*> mpiHelperInstancePtr_ = nullptr;

auto
MPIHelper::getCommunicator() -> MPICommunicator
{
  return MPI_COMM_WORLD;
}

auto
MPIHelper::getLocalCommunicator() -> MPICommunicator
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
  // create singleton instance & record its address for later use
  static MPIHelper instance(argc, argv);
  static std::once_flag instanceFlag;
  std::call_once(instanceFlag, [&]() {
    mpiHelperInstancePtr_.store(&instance, std::memory_order_release);
  });
  return instance;
}

MPIHelper&
MPIHelper::instance()
{
  // if pointer is written in another thread we may need to wait until this
  // threads sees the value
  MPIHelper* ptr = mpiHelperInstancePtr_.load(std::memory_order_acquire);
  if (ptr)
    return *ptr;
  unsigned int count = 0;
  do {
#if defined(__x86_64__) || defined(__i386__)
    __asm__ __volatile__("pause");
#endif
    ptr = mpiHelperInstancePtr_.load(std::memory_order_relaxed);
    if (++count == ~(unsigned int){ 0 })
      std::cerr << "MPIHelper seems to be not initialized! Ensure to call "
                   "MPIHelper::instance(argc, argv) with arguments first."
                << std::endl;
  } while (!ptr);
  return *mpiHelperInstancePtr_.load(std::memory_order_acquire);
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
