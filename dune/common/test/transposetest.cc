// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#include "config.h"

#include <type_traits>
#include <string>
#include <cstddef>

#include <dune/common/transpose.hh>
#include <dune/common/fmatrix.hh>
#include <dune/common/diagonalmatrix.hh>
#include <dune/common/classname.hh>
#include <dune/common/test/testsuite.hh>



// check A*transpose(B)
template<class A, class B, class BT>
auto checkAxBT(const A& a, const B&b, const BT&bt)
{
  Dune::TestSuite suite(std::string{"Check transpose with A="} + Dune::className<A>() + " and B=" + Dune::className<B>() + " and BT=" + Dune::className<BT>());

  // compute abt
  auto abt = a * bt;

  // check result type
  using FieldA = typename Dune::FieldTraits<A>::field_type;
  using FieldB = typename Dune::FieldTraits<B>::field_type;
  using Field = typename Dune::PromotionTraits<FieldA, FieldB>::PromotedType;
  using ABT = Dune::FieldMatrix<Field,A::rows, B::rows>;
  suite.check(std::is_same<decltype(abt), ABT>())
    << "Result type of A*transpose(B) should be " << Dune::className<ABT>() << " but is " << Dune::className<decltype(abt)>();

  // manually compute result value
  auto abt_check = ABT{};
  for(std::size_t i=0; i<A::rows; ++i)
    for(std::size_t j=0; j<B::rows; ++j)
      for(auto&& [b_jk, k] : Dune::sparseRange(b[j]))
        abt_check[i][j] += a[i][k]*b_jk;

  // check result value
  bool equal = true;
  for(std::size_t i=0; i<A::rows; ++i)
    for(std::size_t j=0; j<B::rows; ++j)
      equal = equal and (abt_check[i][j] == abt[i][j]);
  suite.check(equal)
    << "Result of A*transpose(B) should be \n" << abt_check << " but is \n" << abt;

  return suite;
}

template<class A, class B>
void checkTranspose(Dune::TestSuite& suite, A a, B b_original)
{
  // Check with reference capture
  {
    auto b = b_original;
    auto bt = transpose(b);
    suite.subTest(checkAxBT(a,b,bt));
    // Check if bt was captured by reference
    b *= 2;
    suite.subTest(checkAxBT(a,b,bt));
  }

  // Check with value capture
  {
    auto b = b_original;
    auto bt = transpose(std::move(b));
    suite.subTest(checkAxBT(a,b_original,bt));
    // Check if bt was captured by value
    b = b_original;
    b *= 2;
    suite.subTest(checkAxBT(a,b_original,bt));
  }
}


int main()
{
  Dune::TestSuite suite;
  std::size_t seed=0;

  // fill dense matrix with test data
  auto testFillDense = [k=seed](auto& matrix) mutable {
    for(std::size_t i=0; i<matrix.N(); ++i)
      for(std::size_t j=0; j<matrix.M(); ++j)
        matrix[i][j] = k++;
  };

  {
    auto a = Dune::FieldMatrix<double,3,4>{};
    auto b = Dune::FieldMatrix<double,7,4>{};
    testFillDense(a);
    testFillDense(b);
    checkTranspose(suite,a,b);
  }

  {
    auto a = Dune::FieldMatrix<double,1,2>{};
    auto b = Dune::FieldMatrix<double,3,2>{};
    testFillDense(a);
    testFillDense(b);
    checkTranspose(suite,a,b);
  }

  {
    auto a = Dune::FieldMatrix<double,1,2>{};
    auto b = Dune::FieldMatrix<double,1,2>{};
    testFillDense(a);
    testFillDense(b);
    checkTranspose(suite,a,b);
  }

  {
    auto a = Dune::FieldMatrix<double,3,4>{};
    auto b = Dune::DiagonalMatrix<double,4>{};
    testFillDense(a);
    b = {0, 1, 2, 3};
    checkTranspose(suite,a,b);
  }

  return suite.exit();
}
