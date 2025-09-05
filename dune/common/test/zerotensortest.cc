// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

#include <iostream>
#include <utility>

#include <dune/common/densetensor.hh>
#include <dune/common/exceptions.hh>
#include <dune/common/fvector.hh>
#include <dune/common/indices.hh>
#include <dune/common/fmatrix.hh>
#include <dune/common/indices.hh>
#include <dune/common/hybridutilities.hh>
#include <dune/common/zerotensor.hh>
#include <dune/common/parallel/mpihelper.hh>
#include <dune/common/test/foreachindex.hh>
#include <dune/common/test/testsuite.hh>

using namespace Dune;


template <class Tensor1, class Tensor2>
void checkEqual(Dune::TestSuite& testSuite, Tensor1 const& a, Tensor2 const& b)
{
  Dune::TestSuite subTestSuite("checkEqual");
  subTestSuite.require(a.extents() == b.extents());
  Dune::forEachIndex(a.extents(), [&](auto index) {
    subTestSuite.check(a[index] == b[index]);
  });
  testSuite.subTest(subTestSuite);
}

template <class Tensor>
void checkEqualValue(Dune::TestSuite& testSuite, Tensor const& a, typename Tensor::value_type const& value)
{
  Dune::TestSuite subTestSuite("checkEqualValue");
  Dune::forEachIndex(a.extents(), [&](auto index) {
    subTestSuite.check(a[index] == value);
  });
  testSuite.subTest(subTestSuite);
}

template <class Tensor>
void checkConstructors(Dune::TestSuite& testSuite)
{
  Dune::TestSuite subTestSuite("checkConstructors");

  // default constructor
  std::cout << "default constructor..." << std::endl;
  Tensor tensor1;
  checkEqualValue(subTestSuite, tensor1, 0.0);

  // copy/move constructors
  std::cout << "copy constructor..." << std::endl;
  Tensor tensor2{tensor1};
  checkEqual(subTestSuite, tensor1,tensor2);

  Tensor tensor3 = tensor1;
  checkEqual(subTestSuite, tensor1,tensor3);

  std::cout << "move constructor..." << std::endl;
  Tensor tensor4{std::move(tensor2)};
  checkEqual(subTestSuite, tensor1,tensor4);

  Tensor tensor5 = std::move(tensor3);
  checkEqual(subTestSuite, tensor1,tensor5);

  // constructor with extents
  std::cout << "constructor with extents..." << std::endl;
  Tensor tensor6(tensor1.extents());
  checkEqual(subTestSuite, tensor1,tensor6);

  Tensor tensor7(tensor1.extents(), 0.0);
  checkEqual(subTestSuite, tensor1,tensor7);

  testSuite.subTest(subTestSuite);
}

template <class Tensor>
void checkArithmetic(Dune::TestSuite& testSuite)
{
  Dune::TestSuite subTestSuite("checkArithmetic");

  Tensor tensor1;
  DenseTensor tensor2(tensor1.extents(), 2.0);
  checkEqualValue(subTestSuite, tensor1, 0.0);
  checkEqualValue(subTestSuite, tensor2, 2.0);

  // plus
  auto tensor3a = tensor1 + tensor2;
  checkEqualValue(subTestSuite, tensor3a, 2.0);
  auto tensor3b = tensor2 + tensor1;
  checkEqualValue(subTestSuite, tensor3b, 2.0);

  // minus
  auto tensor4a = tensor1 - tensor2;
  checkEqualValue(subTestSuite, tensor4a, -2.0);
  auto tensor4b = tensor2 - tensor1;
  checkEqualValue(subTestSuite, tensor4b, 2.0);

  // negation
  auto tensor5 = -tensor1;
  checkEqualValue(subTestSuite, tensor5, 0.0);

  // scaling
  auto tensor6a = tensor1 * 2.0;
  checkEqualValue(subTestSuite, tensor6a, 0.0);

  auto tensor6b = tensor1 / 2.0;
  checkEqualValue(subTestSuite, tensor6b, 0.0);

  auto tensor6c = 2.0 * tensor1;
  checkEqualValue(subTestSuite, tensor6c, 0.0);

  // tensor dot product
  if constexpr (Tensor::extents_type::rank() >= 1) {
    auto tensor7 = tensor1.dot(tensor2);
    checkEqualValue(subTestSuite, tensor7, 0.0);
  }
  if constexpr (Tensor::extents_type::rank() >= 2) {
    auto tensor8 = tensor1.ddot(tensor2);
    checkEqualValue(subTestSuite, tensor8, 0.0);
  }

  auto value9 = tensor1.inner(tensor2);
  subTestSuite.check(value9 == 0.0);

  testSuite.subTest(subTestSuite);
}



int main(int argc, char** argv)
{
  MPIHelper::instance(argc, argv);

  TestSuite testSuite;

  using Tensor0 = Dune::Impl::ZeroTensor<double>;
  using Tensor1 = Dune::Impl::ZeroTensor<double,2>;
  using Tensor2 = Dune::Impl::ZeroTensor<double,2,2>;
  using Tensor3 = Dune::Impl::ZeroTensor<double,2,2,2>;

  checkConstructors<Tensor0>(testSuite);
  checkConstructors<Tensor1>(testSuite);
  checkConstructors<Tensor2>(testSuite);
  checkConstructors<Tensor3>(testSuite);

  checkArithmetic<Tensor0>(testSuite);
  checkArithmetic<Tensor1>(testSuite);
  checkArithmetic<Tensor2>(testSuite);
  checkArithmetic<Tensor3>(testSuite);

  return testSuite.exit();
}
