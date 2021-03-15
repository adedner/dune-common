#if HAVE_CONFIG_H
#include <config.h>
#endif

#if __has_include(<concepts>)

#include <array>
#include <vector>

#include <dune/common/fmatrix.hh>
#include <dune/common/fvector.hh>
#include <dune/common/diagonalmatrix.hh>
#include <dune/common/dynmatrix.hh>
#include <dune/common/dynvector.hh>
#include <dune/common/concepts/matrix.hh>
#include <dune/common/concepts/linearmap.hh>
#include <dune/common/concepts/vectorspace.hh>

namespace Dune::Concept {

template<class K, int n>
struct IsRowMajor<Dune::DiagonalMatrix<K,n>> : std::true_type {};

template<class K, int n>
struct IsColMajor<Dune::DiagonalMatrix<K,n>> : std::true_type {};

}

int main(int argc, char** argv)
{
  Dune::TestSuite test;

  static_assert(Dune::Concept::Matrix<Dune::FieldMatrix<double,2,2>>);
  static_assert(Dune::Concept::TraversableMatrix<Dune::FieldMatrix<double,2,2>>);
  static_assert(Dune::Concept::VectorSpace<Dune::FieldMatrix<double,2,2>>);
  static_assert(Dune::Concept::LinearMap<Dune::FieldMatrix<double,2,2>, Dune::FieldVector<double,2>, Dune::FieldVector<double,2>>);
  static_assert(Dune::Concept::TransposableLinearMap<Dune::FieldMatrix<double,2,2>, Dune::FieldVector<double,2>, Dune::FieldVector<double,2>>);
  static_assert(Dune::Concept::HermitianLinearMap<Dune::FieldMatrix<double,2,2>, Dune::FieldVector<double,2>, Dune::FieldVector<double,2>>);

  static_assert(Dune::Concept::Matrix<Dune::DynamicMatrix<double>>);
  static_assert(Dune::Concept::TraversableMatrix<Dune::DynamicMatrix<double>>);
  // static_assert(Dune::Concept::VectorSpace<Dune::DynamicMatrix<double>>); // missing + and -

  static_assert(Dune::Concept::Matrix<Dune::DiagonalMatrix<double,2>>);
  // static_assert(Dune::Concept::TraversableMatrix<Dune::DiagonalMatrix<double,2>>); // row not traversable
  // static_assert(Dune::Concept::SparseMatrix<Dune::DiagonalMatrix<double,2>>); // missing nonzeroes()

}

#else
int main()
{
  return 77;
}
#endif
