// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_COMMON_TRANSPOSE_HH
#define DUNE_COMMON_TRANSPOSE_HH

#include <cstddef>
#include <functional>

#include <dune/common/fmatrix.hh>
#include <dune/common/promotiontraits.hh>
#include <dune/common/referencehelper.hh>

namespace Dune {

namespace Impl {

  // Wrapper representing the transposed of a matrix.
  // Creating the wrapper does not compute anything
  // but only serves for tagging the wrapped matrix
  // for transposition. This class will store M by value.
  // To support reference-semantic, it supports using
  // M=std::reference_wrapper<OriginalMatrixType>.
  template<class M>
  class TransposedMatrixWrapper
  {
    using WrappedMatrix = ResolveRef_t<M>;

    const WrappedMatrix& wrappedMatrix() const {
      return resolveRef(matrix_);
    }

  public:

    enum {
      //! The number of rows.
      rows = WrappedMatrix::cols,
      //! The number of columns.
      cols = WrappedMatrix::rows
    };

    using value_type = typename WrappedMatrix::value_type;

    TransposedMatrixWrapper(M&& matrix) : matrix_(std::move(matrix)) {}
    TransposedMatrixWrapper(const M& matrix) : matrix_(matrix) {}

    TransposedMatrixWrapper(const TransposedMatrixWrapper&) = default;
    TransposedMatrixWrapper(TransposedMatrixWrapper&&) = default;
    TransposedMatrixWrapper& operator=(const TransposedMatrixWrapper&) = default;
    TransposedMatrixWrapper& operator=(TransposedMatrixWrapper&&) = default;

    template<class OtherField, int otherRows>
    friend auto operator* (const FieldMatrix<OtherField, otherRows, rows>& matrixA,
                            const TransposedMatrixWrapper& matrixB)
    {
      using ThisField = typename FieldTraits<WrappedMatrix>::field_type;
      using Field = typename PromotionTraits<ThisField, OtherField>::PromotedType;
      FieldMatrix<Field, otherRows, cols> result;
      for (std::size_t j=0; j<otherRows; ++j)
        matrixB.wrappedMatrix().mv(matrixA[j], result[j]);
      return result;
    }

    template<class X, class Y>
    void mv (const X& x, Y& y) const
    {
      wrappedMatrix().mtv(x,y);
    }

    Dune::FieldMatrix<value_type, rows, cols> asDense() const
    {
      Dune::FieldMatrix<value_type, rows, cols> MT;
      for(auto&& [M_i, i] : Dune::sparseRange(wrappedMatrix()))
        for(auto&& [M_ij, j] : Dune::sparseRange(M_i))
          MT[j][i] = M_ij;
      return MT;
    }

    operator Dune::FieldMatrix<value_type, cols, rows> () const
    {
      return asDense();
    }

  private:

    M matrix_;
  };

} // namespace Impl

/**
 * \brief Create a wrapper modelling the transposed matrix
 *
 * Currently the wrapper only implements
 * \code
 * auto c = a*transpose(b);
 * \endcode
 * if a is a FieldMatrix of appropriate size. This is
 * optimal even for sparse b because it only relies on
 * calling b.mv(a[i], c[i]) for the rows of a.
 * Furthermore the wrapper can be converted to a suitable
 * dense FieldMatrix using the \code adDense() \endcode method
 * if the wrapped matrix allows to iterate over its entries
 * and matrix-vector multiplication using \code transpose(b).mv(x,y) \endcode
 * if the wrapped matrix provides the \code b.mtv(x,y) \endcode.
 *
 * The created object stores the passed reference to the wrapped matrix
 * but does not copy the latted.
 */
template<class Matrix,
  typename = std::void_t<typename Matrix::row_type>>
auto transpose(const Matrix& matrix) {
  return Impl::TransposedMatrixWrapper(std::cref(matrix));
}

/**
 * \brief Create a wrapper modelling the transposed matrix
 *
 * Currently the wrapper only implements
 * \code
 * auto c = a*transpose(b);
 * \endcode
 * if a is a FieldMatrix of appropriate size. This is
 * optimal even for sparse b because it only relies on
 * calling b.mv(a[i], c[i]) for the rows of a.
 * Furthermore the wrapper can be converted to a suitable
 * dense FieldMatrix using the \code adDense() \endcode method
 * if the wrapped matrix allows to iterate over its entries
 * and matrix-vector multiplication using \code transpose(b).mv(x,y) \endcode
 * if the wrapped matrix provides the \code b.mtv(x,y) \endcode.
 *
 * This specialization allows to pass a \code std::reference_wrapper \endcode
 * of a matrix to explicitly request, that the latter is stored by
 * reference in the wrapper.
 */
template<class Matrix>
auto transpose(const std::reference_wrapper<Matrix>& matrix) {
  return Impl::TransposedMatrixWrapper(matrix);
}

/**
 * \brief Create a wrapper modelling the transposed matrix
 *
 * Currently the wrapper only implements
 * \code
 * auto c = a*transpose(b);
 * \endcode
 * if a is a FieldMatrix of appropriate size. This is
 * optimal even for sparse b because it only relies on
 * calling b.mv(a[i], c[i]) for the rows of a.
 * Furthermore the wrapper can be converted to a suitable
 * dense FieldMatrix using the \code adDense() \endcode metho
 * if the wrapped matrix allows to iterate over its entries
 * and matrix-vector multiplication using \code transpose(b).mv(x,y) \endcode
 * if the wrapped matrix provides the \code b.mtv(x,y) \endcode.
 *
 * The created object stores a copy of the wrapped matrix
 * and leaves the passed r-value refenrence in moved-from state.
 */
template<class Matrix,
  typename = std::void_t<typename Matrix::row_type>,
  typename = std::enable_if_t<not std::is_reference_v<Matrix>,int>>
  // The previous check is needed to avoid that clang<=7
  // selects this overload for l-value references.
auto transpose(Matrix&& matrix) {
  return Impl::TransposedMatrixWrapper(std::move(matrix));
}

} // namespace Dune

#endif // DUNE_COMMON_TRANSPOSE_HH
