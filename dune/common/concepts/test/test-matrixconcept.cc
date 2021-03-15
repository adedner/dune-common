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
#include <dune/common/transpose.hh>
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
  using Mat1 = Dune::FieldMatrix<double,2,2>;
  using Vec1 = Dune::FieldVector<double,2>;
  static_assert(Dune::Concept::Matrix<Mat1>);
  static_assert(Dune::Concept::TraversableMatrix<Mat1>);
  static_assert(Dune::Concept::VectorSpace<Mat1>);
  static_assert(Dune::Concept::LinearMap<Mat1, Vec1, Vec1>);
  static_assert(Dune::Concept::TransposableLinearMap<Mat1, Vec1, Vec1>);
  static_assert(Dune::Concept::HermitianLinearMap<Mat1, Vec1, Vec1>);

  using Mat2 = Dune::DynamicMatrix<double>;
  using Vec2 = Dune::DynamicVector<double>;
  static_assert(Dune::Concept::Matrix<Mat2>);
  static_assert(Dune::Concept::TraversableMatrix<Mat2>);
  // static_assert(Dune::Concept::VectorSpace<Mat2>); // missing + and -
  static_assert(Dune::Concept::LinearMap<Mat2, Vec2, Vec2>);
  static_assert(Dune::Concept::TransposableLinearMap<Mat2, Vec2, Vec2>);
  static_assert(Dune::Concept::HermitianLinearMap<Mat2, Vec2, Vec2>);

  using Mat3 = Dune::DiagonalMatrix<double,2>;
  using Vec3 = Dune::FieldVector<double,2>;
  static_assert(Dune::Concept::Matrix<Mat3>);
  static_assert(Dune::Concept::MutableMatrix<Mat3>);
  // static_assert(Dune::Concept::TraversableMatrix<Mat3>); // row not traversable
  // static_assert(Dune::Concept::SparseMatrix<Mat3>); // missing nonzeroes()
  // static_assert(Dune::Concept::DiagonalMatrix<Mat3>); // missing nonzeroes()
  // static_assert(Dune::Concept::MutableDiagonalMatrix<Mat3>); // missing nonzeroes()
  static_assert(Dune::Concept::LinearMap<Mat3, Vec3, Vec3>);
  static_assert(Dune::Concept::TransposableLinearMap<Mat3, Vec3, Vec3>);
  static_assert(Dune::Concept::HermitianLinearMap<Mat3, Vec3, Vec3>);

  using Mat4 = decltype(transpose(std::declval<Mat1>()));
  // static_assert(Dune::Concept::Matrix<Mat4>); // no collection or matrix concept is fulfilled.
}

#else
int main()
{
  return 77;
}
#endif
