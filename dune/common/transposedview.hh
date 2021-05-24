// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_COMMON_TRANSPOSEDVIEW_HH
#define DUNE_COMMON_TRANSPOSEDVIEW_HH

#include <cstddef>
#include <type_traits>

#include <dune/common/concept.hh>
#include <dune/common/ftraits.hh>

namespace Dune {
namespace Concept {

// Minimal concept for matrices allowed to be wrapped by TransposedView
struct DenseMatrix
{
  template <class M>
  auto require (const M& matrix) -> decltype((
    requireType<typename M::size_type>(),
    requireType<typename M::value_type>(),
    requireConvertible<typename M::value_type>(matrix[0][0]),
    matrix.N(),
    matrix.M()
  ));
};

} // end namespace Concept


//! View of a matrix as its transposed.
/**
 * The transposed-view of a matrix can be used similarly to the original
 * matrix where in element access the row and column indices are interchanged.
 *
 * Example:
 * ```c++
 * Dune::FieldMatrix<double,3,4> A;
 * TransposedView At{A};
 *
 * // element access
 * assert(At[0][1] == A[1][0]);
 *
 * // linear map
 * At.mv(x,y);
 * A.mtv(y,x);
 * ```
 *
 * See also \ref transposedView function to generate the wrapper.
 **/
template<class Matrix>
class TransposedView;


//! Create a wrapper modelling the transposed matrix
template<class Matrix>
decltype(auto) transposedView (Matrix& matrix)
{
  if constexpr(models<Concept::DenseMatrix,Matrix>())
    return TransposedView<Matrix>(matrix);
  else
    return matrix;
}

//! Twice transpose of a matrix is the matrix
template<class Matrix>
Matrix& transposedView (TransposedView<Matrix> view)
{
  return view.matrix();
}


template<class Matrix>
class TransposedView
{
public:
  using size_type = typename Matrix::size_type;
  using value_type = typename Matrix::value_type;

private:
  struct AccessProxy
  {
    Matrix& matrix_;
    size_type row_;

    decltype(auto) operator[] (size_type col) const
    {
      return transposedView(matrix_[col][row_]);
    }

    decltype(auto) operator[] (size_type col)
    {
      return transposedView(matrix_[col][row_]);
    }
  };

public:
  //! Constructor stores a reference to the wrapped matrix
  explicit TransposedView (Matrix& matrix)
    : matrix_(matrix)
  {}

  //! Access the wrapped matrix elements A[col][row]
  AccessProxy operator[] (size_type row) const
  {
    return {matrix_, row};
  }

  //! Access the wrapped matrix elements A[col][row]
  AccessProxy operator[] (size_type row)
  {
    return {matrix_, row};
  }

  //! Return the number of columns of the wrapped matrix
  size_type N () const
  {
    return matrix_.M();
  }

  //! Return the number of rows of the wrapped matrix
  size_type M () const
  {
    return matrix_.N();
  }

  //! Return the stored matrix
  Matrix& matrix ()
  {
    return matrix_;
  }

  //===== linear maps

  //! y = A x
  template<class X, class Y>
  void mv (const X& x, Y& y) const
  {
    matrix_.mtv(x,y);
  }

  //! y = A^T x
  template<class X, class Y>
  void mtv (const X &x, Y &y) const
  {
    matrix_.mv(x,y);
  }

  template<class X, class Y>
  void mhv (const X &x, Y &y) const = delete;

  //! y += A x
  template<class X, class Y>
  void umv (const X& x, Y& y) const
  {
    matrix_.umtv(x,y);
  }

  //! y += A^T x
  template<class X, class Y>
  void umtv (const X& x, Y& y) const
  {
    matrix_.umv(x,y);
  }

  template<class X, class Y>
  void umhv (const X& x, Y& y) const = delete;

  //! y -= A x
  template<class X, class Y>
  void mmv (const X& x, Y& y) const
  {
    matrix_.mmtv(x,y);
  }

  //! y -= A^T x
  template<class X, class Y>
  void mmtv (const X& x, Y& y) const
  {
    matrix_.mmv(x,y);
  }

  template<class X, class Y>
  void mmhv (const X& x, Y& y) const = delete;

  //! y += alpha A x
  template<class X, class Y>
  void usmv (typename FieldTraits<Y>::field_type alpha, const X& x, Y& y) const
  {
    matrix_.usmtv(alpha,x,y);
  }

  //! y += alpha A^T x
  template<class X, class Y>
  void usmtv (typename FieldTraits<Y>::field_type alpha, const X& x, Y& y) const
  {
    matrix_.usmv(alpha,x,y);
  }

  template<class X, class Y>
  void usmhv (typename FieldTraits<Y>::field_type alpha, const X& x, Y& y) const = delete;

private:
  Matrix& matrix_;
};

} // namespace Dune

#endif // DUNE_COMMON_TRANSPOSEDVIEW_HH
