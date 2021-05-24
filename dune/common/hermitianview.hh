// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_COMMON_HERMITIANVIEW_HH
#define DUNE_COMMON_HERMITIANVIEW_HH

#include <cstddef>

#include <dune/common/ftraits.hh>
#include <dune/common/math.hh>
#include <dune/common/transposedview.hh>

namespace Dune {

//! View of a matrix as its Hermitian.
template<class Matrix>
class HermitianView;


//! Create a wrapper modelling the Hermitian matrix
template<class Matrix>
decltype(auto) hermitianView (const Matrix& matrix)
{
  if constexpr(models<Concept::DenseMatrix,Matrix>())
    return HermitianView<Matrix>(matrix);
  else
    return conjugateComplex(matrix);
}

//! Twice Hermitian of a matrix is the matrix
template<class Matrix>
const Matrix& hermitianView (HermitianView<Matrix> view)
{
  return view.matrix();
}



template<class Matrix>
class HermitianView
{
public:
  using size_type = typename Matrix::size_type;
  using value_type = typename Matrix::value_type;

private:
  struct AccessProxy
  {
    const Matrix& matrix_;
    size_type row_;

    auto operator[] (size_type col) const
    {
      return hermitianView(matrix_[col][row_]);
    }
  };

public:
  explicit HermitianView (const Matrix& matrix)
    : matrix_(matrix)
  {}

  //! Access the wrapped matrix elements A[col][row]
  AccessProxy operator[] (size_type row) const
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
  const Matrix& matrix () const
  {
    return matrix_;
  }

  //===== linear maps

  //! y = A x
  template<class X, class Y>
  void mv (const X& x, Y& y) const
  {
    matrix_.mhv(x,y);
  }

  template<class X, class Y>
  void mtv (const X &x, Y &y) const = delete;

  //! y = A^T x
  template<class X, class Y>
  void mhv (const X &x, Y &y) const
  {
    matrix_.mv(x,y);
  }

  //! y += A x
  template<class X, class Y>
  void umv (const X& x, Y& y) const
  {
    matrix_.umhv(x,y);
  }

  template<class X, class Y>
  void umtv (const X& x, Y& y) const = delete;

  //! y += A^H x
  template<class X, class Y>
  void umhv (const X& x, Y& y) const
  {
    matrix_.umv(x,y);
  }

  //! y -= A x
  template<class X, class Y>
  void mmv (const X& x, Y& y) const
  {
    matrix_.mmhv(x,y);
  }

  template<class X, class Y>
  void mmtv (const X& x, Y& y) const = delete;

  //! y -= A^H x
  template<class X, class Y>
  void mmhv (const X& x, Y& y) const
  {
    matrix_.mmv(x,y);
  }

  //! y += alpha A x
  template<class X, class Y>
  void usmv (typename FieldTraits<Y>::field_type alpha, const X& x, Y& y) const
  {
    matrix_.usmhv(alpha,x,y);
  }

  template<class X, class Y>
  void usmtv (typename FieldTraits<Y>::field_type alpha, const X& x, Y& y) const = delete;

  //! y += alpha A^H x
  template<class X, class Y>
  void usmhv (typename FieldTraits<Y>::field_type alpha, const X& x, Y& y) const
  {
    matrix_.usmv(alpha,x,y);
  }

private:
  const Matrix& matrix_;
};

} // namespace Dune

#endif // DUNE_COMMON_HERMITIANVIEW_HH
