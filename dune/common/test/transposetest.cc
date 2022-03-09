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

// Helper function to convert a matrix to a FieldMatrix
// using only the mv method.
template<class M>
auto toDenseMatrix(const M& m){
  using K = typename M::field_type;
  Dune::FieldMatrix<K, M::rows, M::cols> result;
  for (int j=0; j<M::cols; j++) {
    Dune::FieldVector<K,M::cols> idColumn(0);
    idColumn[j] = 1;
    Dune::FieldVector<K,M::rows> column;
    m.mv(idColumn,column);
    for (int k=0; k<M::rows; k++)
      result[k][j] = column[k];
  }
  return result;
}


// check A*transpose(B)
template<class A, class B, class BT>
auto checkAxBT(const A& a, const B&b, const BT&bt)
{
  Dune::TestSuite suite(std::string{"Check A*transpose(B) with A="} + Dune::className<A>() + " and B=" + Dune::className<B>() + " and BT=" + Dune::className<BT>());

  // compute abt
  auto abt = a * bt;

  // check result type
  using FieldA = typename Dune::FieldTraits<A>::field_type;
  using FieldB = typename Dune::FieldTraits<B>::field_type;
  using Field = typename Dune::PromotionTraits<FieldA, FieldB>::PromotedType;
  using ABT = Dune::FieldMatrix<Field,A::rows, B::rows>;
  suite.check(std::is_same<decltype(abt), ABT>())
    << "Result type of A*transpose(B) should be " << Dune::className<ABT>() << " but is " << Dune::className<decltype(abt)>();

  auto a_dense = toDenseMatrix(a);
  auto b_dense = toDenseMatrix(b);

  // manually compute result value
  auto abt_check = ABT{};
  for(std::size_t i=0; i<A::rows; ++i)
    for(std::size_t j=0; j<B::rows; ++j)
      for(std::size_t k=0; k<B::cols; ++k)
        abt_check[i][j] += a_dense[i][k]*b_dense[j][k];

  // check result value
  bool equal = true;
  for(std::size_t i=0; i<A::rows; ++i)
    for(std::size_t j=0; j<B::rows; ++j)
      equal = equal and (abt_check[i][j] == abt[i][j]);
  suite.check(equal)
    << "Result of A*transpose(B) should be \n" << abt_check << " but is \n" << abt;

  return suite;
}

template<class A, class AT, class B>
auto checkATxB(const A& a, const AT& at, const B& b)
{
  Dune::TestSuite suite(std::string{"Check transpose(A)*B with AT="} + Dune::className<AT>() + " and B=" + Dune::className<B>() + " and B=" + Dune::className<B>());

  // compute atb
  auto atb = at * b;

  // check result type
  using FieldA = typename Dune::FieldTraits<A>::field_type;
  using FieldB = typename Dune::FieldTraits<B>::field_type;
  using Field = typename Dune::PromotionTraits<FieldA, FieldB>::PromotedType;
  using ATB = Dune::FieldMatrix<Field,A::cols, B::cols>;
  suite.check(std::is_same<decltype(atb), ATB>())
    << "Result type of transpose(A)*B should be " << Dune::className<ATB>() << " but is " << Dune::className<decltype(atb)>();

  auto a_dense = toDenseMatrix(a);
  auto b_dense = toDenseMatrix(b);

  // manually compute result value
  auto atb_check = ATB{};
  for(std::size_t i=0; i<A::cols; ++i)
    for(std::size_t j=0; j<B::cols; ++j)
      for(std::size_t k=0; k<B::rows; ++k)
        atb_check[i][j] += a_dense[k][i]*b_dense[k][j];

  // check result value
  bool equal = true;
  for(std::size_t i=0; i<A::cols; ++i)
    for(std::size_t j=0; j<B::cols; ++j)
      equal = equal and (atb_check[i][j] == atb[i][j]);
  suite.check(equal)
    << "Result of transpose(A)*B should be \n" << atb_check << " but is \n" << atb;

  return suite;
}

template<class A, class B, class C>
void checkTranspose(Dune::TestSuite& suite, A a, B b_original, C c)
{
  // Check A*BT with reference capture
  {
    auto b = b_original;
    auto bt = transpose(b);
    suite.subTest(checkAxBT(a,b,bt));
    // Check if bt was captured by reference
    b *= 2;
    suite.subTest(checkAxBT(a,b,bt));
  }

  // Check A*BT with reference capture by std::ref
  {
    auto b = b_original;
    auto bt = transpose(std::ref(b));
    suite.subTest(checkAxBT(a,b,bt));
    // Check if bt was captured by reference
    b *= 2;
    suite.subTest(checkAxBT(a,b,bt));
  }

  // Check A*BT with value capture
  {
    auto b = b_original;
    auto bt = transpose(std::move(b));
    suite.subTest(checkAxBT(a,b_original,bt));
    // Check if bt was captured by value
    b = b_original;
    b *= 2;
    suite.subTest(checkAxBT(a,b_original,bt));
  }

  // Check BT*C with reference capture
  {
    auto b = b_original;
    auto bt = transpose(b);
    suite.subTest(checkATxB(b,bt,c));
    // Check if bt was captured by reference
    b *= 2;
    suite.subTest(checkATxB(b,bt,c));
  }

  // Check conversion to dense matrix
  {
    auto b = b_original;
    auto bt = transpose(b_original).asDense();
    suite.subTest(checkAxBT(a,b,bt));
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
    auto c = Dune::FieldMatrix<double,7,2>{};
    testFillDense(a);
    testFillDense(b);
    testFillDense(c);
    checkTranspose(suite,a,b,c);
  }

  {
    auto a = Dune::FieldMatrix<double,1,2>{};
    auto b = Dune::FieldMatrix<double,3,2>{};
    auto c = Dune::FieldMatrix<double,3,4>{};
    testFillDense(a);
    testFillDense(b);
    testFillDense(c);
    checkTranspose(suite,a,b,c);
  }

  {
    auto a = Dune::FieldMatrix<double,1,2>{};
    auto b = Dune::FieldMatrix<double,1,2>{};
    auto c = Dune::FieldMatrix<double,1,5>{};
    testFillDense(a);
    testFillDense(b);
    testFillDense(c);
    checkTranspose(suite,a,b,c);
  }

  {
    auto a = Dune::FieldMatrix<double,3,4>{};
    auto b = Dune::DiagonalMatrix<double,4>{};
    auto c = Dune::FieldMatrix<double,4,7>{};
    testFillDense(a);
    b = {0, 1, 2, 3};
    testFillDense(c);
    checkTranspose(suite,a,b,c);
  }

  return suite.exit();
}
