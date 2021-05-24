// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#include "config.h"

#include <type_traits>
#include <string>
#include <cstddef>

#include <dune/common/transpose.hh>
#include <dune/common/fmatrix.hh>
#include <dune/common/math.hh>
#include <dune/common/diagonalmatrix.hh>
#include <dune/common/dynmatrix.hh>
#include <dune/common/classname.hh>
#include <dune/common/hermitianview.hh>
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
  auto testFillDense = [](auto& matrix, int init = 0) {
    int k=init;
    for(std::size_t i=0; i<matrix.N(); ++i)
      for(std::size_t j=0; j<matrix.M(); ++j)
        matrix[i][j] = k++;
  };

  auto testFillVec = [](auto& vec, int init = 0) {
    int k=init;
    for(std::size_t i=0; i<vec.size(); ++i)
      vec[i] = k++;
  };

  {
    auto A = Dune::FieldMatrix<std::complex<double>,3,4>{};
    testFillDense(A);

    auto At = Dune::hermitianView(A);
    bool equal = At.N() == A.M() && At.M() == A.N();
    for (std::size_t i = 0; i < At.N(); ++i)
      for (std::size_t j = 0; j < At.M(); ++j)
        equal = equal && compare(Dune::conjugateComplex(A[j][i]), At[i][j]);
    suite.check(equal) << "Test A[i][j]' == At[j][i]";
  }

  {
    auto A = Dune::FieldMatrix<std::complex<double>,3,4>{};
    testFillDense(A);
    auto x = Dune::FieldVector<std::complex<double>,4>{};
    testFillVec(x,7);
    Dune::FieldVector<std::complex<double>,3> y1,y2;

    auto Ah = Dune::hermitianView(A);

    A.mv(x, y1);
    Ah.mhv(x, y2);

    suite.check(compareVector(y1,y2)) << "Test A.mv and Ah.mhv";
  }

  {
    auto A = Dune::FieldMatrix<std::complex<double>,3,4>{};
    testFillDense(A);
    auto y = Dune::FieldVector<std::complex<double>,3>{};
    testFillVec(y,3);
    Dune::FieldVector<std::complex<double>,4> x1,x2;

    auto Ah = Dune::hermitianView(A);

    A.mhv(y, x1);
    Ah.mv(y, x2);

    suite.check(compareVector(x1,x2)) << "Test A.mhv and Ah.mv";
  }

  {
    auto A = Dune::FieldMatrix<std::complex<double>,3,4>{};
    testFillDense(A);

    auto Ah = Dune::hermitianView(A);
    auto Ahh = Dune::hermitianView(Ah);

    suite.check(compareMatrix(A,Ahh)) << "Test (A^H)^H == A";
  }

  return suite.exit();
}
