// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#include "config.h"

#include <type_traits>
#include <string>
#include <cstddef>

#include <dune/common/transpose.hh>
#include <dune/common/fmatrix.hh>
#include <dune/common/diagonalmatrix.hh>
#include <dune/common/dynmatrix.hh>
#include <dune/common/transposedview.hh>
#include <dune/common/test/testsuite.hh>

template <class X, class Y>
bool compare (const X& x, const Y& y)
{
  using T = std::common_type_t<typename Dune::FieldTraits<X>::real_type,typename Dune::FieldTraits<Y>::real_type>;
  return std::abs(x-y) <= std::numeric_limits<T>::epsilon() * std::abs(x+y) * 2 ||
         std::abs(x-y) < std::numeric_limits<T>::min();
}

template <class V1, class V2>
bool compareVector (const V1& vec1, const V2& vec2)
{
  if (vec1.size() != vec2.size())
    return false;
  for (typename V1::size_type i = 0; i < vec1.size(); ++i)
    if (!compare(vec1[i], vec2[i]))
      return false;
  return true;
}

template <class M1, class M2>
bool compareMatrix (const M1& mat1, const M2& mat2)
{
  if (mat1.N() != mat2.N())
    return false;
  if (mat1.M() != mat2.M())
    return false;
  for (typename M1::size_type i = 0; i < mat1.N(); ++i)
    for (typename M1::size_type j = 0; j < mat1.M(); ++j)
      if (!compare(mat1[i][j], mat2[i][j]))
        return false;
  return true;
}


int main()
{
  Dune::TestSuite suite;

  // fill dense matrix with test data
  auto testFillDense = [](auto&& matrix, int init = 0) {
    int k=init;
    for(std::size_t i=0; i<matrix.N(); ++i)
      for(std::size_t j=0; j<matrix.M(); ++j)
        matrix[i][j] = k++;
  };

  auto testFillBlocked = [testFillDense](auto&& matrix, int init = 0) {
    int k=init;
    for(std::size_t i=0; i<matrix.N(); ++i)
      for(std::size_t j=0; j<matrix.M(); ++j)
        testFillDense(matrix[i][j], k++);
  };

  auto testFillVec = [](auto& vec, int init = 0) {
    int k=init;
    for(std::size_t i=0; i<vec.size(); ++i)
      vec[i] = k++;
  };

  { // check element-wise
    auto A = Dune::FieldMatrix<double,3,4>{};
    testFillDense(A);

    auto At = Dune::TransposedView{A};
    bool equal = At.N() == A.M() && At.M() == A.N();
    for (std::size_t i = 0; i < At.N(); ++i)
      for (std::size_t j = 0; j < At.M(); ++j)
        equal = equal && compare(A[j][i], At[i][j]);
    suite.check(equal) << "Test A[i][j] == At[j][i]";
  }

  { // check linear map operations
    auto A = Dune::FieldMatrix<double,3,4>{};
    auto At = Dune::TransposedView{A};
    testFillDense(A);
    auto x = Dune::FieldVector<double,4>{};
    auto y = Dune::FieldVector<double,3>{};
    testFillVec(x,7);
    testFillVec(y,3);

    Dune::FieldVector<double,3> y1,y2;
    Dune::FieldVector<double,4> x1,x2;

    A.mv(x, y1);
    At.mtv(x, y2);
    suite.check(compareVector(y1,y2)) << "Test A.mv and At.mtv";

    A.mtv(y, x1);
    At.mv(y, x2);
    suite.check(compareVector(x1,x2)) << "Test A.mtv and At.mv";
  }

  { // check that twice transposed is identity
    auto A = Dune::FieldMatrix<double,3,4>{};
    testFillDense(A);

    auto At = Dune::transposedView(A);
    auto Att = Dune::transposedView(At);

    suite.check(compareMatrix(A,Att)) << "Test (A^T)^T == A";
  }

  { // check mutability of transposed matrix
    auto A = Dune::FieldMatrix<double,3,4>{};
    auto At = Dune::transposedView(A);
    testFillDense(At);
  }

  { // check transposed of block matrix
    auto A = Dune::FieldMatrix<Dune::FieldMatrix<double,3,2>,3,4>{};
    auto At = Dune::transposedView(A);
    testFillBlocked(At);
  }

  { // check transposed of diagonal matrix
    auto D = Dune::DiagonalMatrix<double,4>{};
    testFillDense(D);
    auto Dt = Dune::transposedView(D);
    suite.check(compareMatrix(D,Dt)) << "Test D^T == D";
  }

  return suite.exit();
}
