// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_CONCEPTS_MATRIX_HH
#define DUNE_CONCEPTS_MATRIX_HH

#if __has_include(<concepts>) || DOXYGEN
#include <concepts>

#include <dune/common/concepts/collection.hh>
#include <dune/common/concepts/vector.hh>

namespace Dune {
namespace Concept {

/**
 * \ingroup CxxConcepts
 * @{
 **/

/// \brief A Matrix is a collection with direct element acccess.
/**
 * The concept models matrix-like collections, providing indexed element access.
 *
 * \par Refinement of:
 * - \ref AlgebraicMatrix
 *
 * \par Notation:
 * - `m`: a matrix of type `M`.
 * - `i,j`: indices of `size_type`.
 *
 * \par Valid expressions:
 * - `m[i][j]`: Access the (i,j)'th element of the matrix where `i` is the row index
 *   and `j` the column index. `[[pre: 0 <= i < m.N() &&  0 <= j < m.M()]]`
 *
 * \par Models:
 * - \ref FieldMatrix
 * - \ref DynamicMatrix
 * - \ref DiagonalMatrix
 *
 * \hideinitializer
 **/
template <class M>
concept Matrix = AlgebraicMatrix<M>
  && requires(const M& matrix, typename M::size_type i, typename M::size_type j)
{
  { matrix[i][j] } -> std::convertible_to<typename M::value_type>;
};


/// \brief A mutable Matrix is a collection with direct mutable element acccess.
/**
 * The concept models matrix-like collections, providing mutable indexed element access.
 *
 * \par Refinement of:
 * - \ref Matrix
 *
 * \par Notation:
 * - `m`: a matrix of type `M`.
 * - `i,j`: indices of `size_type`.
 * - `value` of `value_type`.
 *
 * \par Valid expressions:
 * - `m[i][j]`: Mutable access to the (i,j)'th element of the matrix where `i` is the
 *   row index and `j` the column index. `[[pre: 0 <= i < m.N() &&  0 <= j < m.M()]]`
 *
 * \hideinitializer
 **/
template <class M>
concept MutableMatrix = Matrix<M>
  && requires(M& matrix, typename M::size_type i, typename M::size_type j, typename M::value_type value)
{
  matrix[i][j] = value;
};

/// \brief A \ref Matrix with constant size.
/// \hideinitializer
template <class M>
concept ConstantSizeMatrix = Matrix<M> && ConstantSizeAlgebraicMatrix<M>;

/// \brief A \ref MutableMatrix with constant size.
/// \hideinitializer
template <class M>
concept MutableConstantSizeMatrix = MutableMatrix<M> && ConstantSizeAlgebraicMatrix<M>;


/// \brief A matrix that can be resized
/**
 * The concept models matrix collections with `resize()` function.
 *
 * \par Refinement of:
 * - \ref Matrix
 *
 * \par Notation:
 * - `m`: a matrix of type `M`.
 * - `r,c`: new number of rows and columns of `size_type`
 *
 * \par Valid expressions:
 * - `m.resize(r,c)`: Resize the matrix to the new size `r x c` where `r` is the number of
 *   rows and `c` is the number of columns. `[[pre: r >= 0 && c >= 0]]`
 *
 * \par Models:
 * - \ref DynamicMatrix
 *
 * \hideinitializer
 **/
template <class M>
concept ResizeableMatrix = Matrix<M>
  && requires(M& matrix, typename M::size_type r, typename M::size_type c)
{
  matrix.resize(r, c);
};



/// \brief A \ref Matrix that is traversable
/**
 * The concept models matrices providing iterators to traverse all entries in the matrix.
 * The iterator might visit only nonzero entries, or all entry. And it might traverse the
 * rows first or the columns. A specialization in which way the matrix is traversed is given
 * by the concepts \ref RowMajorTraversableMatrix and \ref ColMajorTraversableMatrix.
 *
 * \par Refinement of:
 * - \ref TraversableCollection
 *
 * \par Notation:
 * - `m`: a matrix of type `M`.
 *
 * \par Valid expressions:
 * - `*m.begin()`: Return a traversable collection, meaning the dereferenced iterator can
 *   be traversed again and the traversal iterator is a model of \ref TraversableCollection.
 *
 * \hideinitializer
 **/
template <class M, class RangeWrapper = Identity>
concept TraversableMatrix = TraversableCollection<M, RangeWrapper>
  && requires(const M& matrix, RangeWrapper wrapper)
{
  requires TraversableCollection<std::decay_t<decltype(*wrapper(matrix).begin())>>;
};

/// \brief A \ref MutableMatrix that is traversable
/**
 * \par Refinement of:
 * - \ref TraversableMatrix
 * - \ref TraversableMutableCollection
 *
 * \hideinitializer
 **/
template <class M, class RangeWrapper = Identity>
concept TraversableMutableMatrix = TraversableMatrix<M, RangeWrapper>
  && TraversableMutableCollection<M, RangeWrapper>
  && requires(M& matrix, RangeWrapper wrapper)
{
  requires TraversableMutableCollection<std::decay_t<decltype(*wrapper(matrix).begin())>>;
};


template <class M>
concept RowTraversal = requires(M matrix)
{
  matrix.begin_rows();
  matrix.end_rows();
};

/// \brief Concept Map to transform iterators `begin_rows()` into `begin()` and `end_rows()` into `end()`.
struct RowMajorRange
{
  template <class M>
  struct ConstRange {
    auto begin () const { return m.begin_rows(); }
    auto end () const { return m.end_rows(); }
    M const& m;
  };

  template <class M>
  struct MutableRange {
    auto begin () { return m.begin_rows(); }
    auto end () { return m.end_rows(); }
    M& m;
  };

  template <RowTraversal M>
  auto operator() (const M& matrix) const { return ConstRange{matrix}; }

  template <RowTraversal M>
  auto operator() (M& matrix) const { return MutableRange{matrix}; }
};


template <class M>
concept ColTraversal = requires(M matrix)
{
  matrix.begin_cols();
  matrix.end_cols();
};

/// \brief Concept Map to transform iterators `begin_cols()` into `begin()` and `end_cols()` into `end()`.
struct ColMajorRange
{
  template <class M>
  struct ConstRange {
    auto begin () const { return m.begin_cols(); }
    auto end () const { return m.end_cols(); }
    M const& m;
  };

  template <class M>
  struct MutableRange {
    auto begin () { return m.begin_cols(); }
    auto end () { return m.end_cols(); }
    M& m;
  };

  template <ColTraversal M>
  auto operator() (const M& matrix) const { return ConstRange{matrix}; }

  template <ColTraversal M>
  auto operator() (M& matrix) const { return MutableRange{matrix}; }
};


/// \brief A traversable matrix with row-major traversal.
/**
 * A matrix collection that provides iterators `begin_rows()` and `end_rows()` traversing the
 * rows of the matrix. The dereferenced iterators are traversable and provide iterators
 * over the columns of the corresponding row.
 *
 * \hideinitializer
 **/
template <class M>
concept RowMajorTraversableMatrix = Matrix<M> && TraversableMatrix<M,RowMajorRange>;

/// \brief A traversable matrix with column-major traversal.
/**
 * A matrix collection that provides iterators `begin_cols()` and `end_cols()` traversing the
 * columns of the matrix. The dereferenced iterators are traversable and provide iterators
 * over the rows of the corresponding column.
 *
 * \hideinitializer
 **/
template <class M>
concept ColMajorTraversableMatrix = Matrix<M> && TraversableMatrix<M,ColMajorRange>;



/// \brief A \ref Matrix that might have a sparse storage of the nonzero entries.
/**
 * The concept models matrices with possibly sparse storage, like banded matrices or
 * crs/ccs matrices.
 *
 * \par Refinement of:
 * - \ref Matrix
 *
 * \par Notation:
 * - `m`: a matrix of type `M`.
 * - `i,j`: indices of `size_type`.
 *
 * \par Valid expressions:
 * - `m.exists(i,j)`: Return `true` if the element `(i,j)` is stored in the sparse matrix
 *   and thus is a nonzero element, `false` otherwise. `[[pre: 0 <= i < m.N() &&  0 <= j < m.M()]]`
 * - `m.nonzeroes()`: Return the number of nonzero elements stored in the sparse matrix.
 *   `[[post: m.nonzeroes() >= 0]]`
 *
 * \hideinitializer
 **/
template <class M>
concept SparseMatrix = Matrix<M>
  && requires(const M& matrix, typename M::size_type i, typename M::size_type j)
{
  { matrix.exists(i,j) } -> std::convertible_to<bool>;
  { matrix.nonzeroes() } -> std::convertible_to<typename M::size_type>;
};


/// \brief A DiagonalMatrix is a refinement of a \ref SparseMatrix with access to its diagonal.
/// \hideinitializer
template <class M>
concept DiagonalMatrix = SparseMatrix<M>
  && requires(const M& matrix, typename M::size_type i)
{
  matrix.diagonal();
  requires Vector<std::decay_t<decltype(matrix.diagonal())>>;
  { matrix.diagonal(i) } -> std::convertible_to<typename M::value_type>;
};

/// \brief A MutableDiagonalMatrix is a refinement of a \ref SparseMatrix with mutable
/// access to its diagonal.
/// \hideinitializer
template <class M>
concept MutableDiagonalMatrix = DiagonalMatrix<M>
  && requires(M& matrix, typename M::size_type i, typename M::value_type value)
{
  requires MutableVector<std::decay_t<decltype(matrix.diagonal())>>;
  matrix.diagonal(i) = value;
};

/** @} */

}} // end namespace Dune::Concept

#endif //__has_include(<concepts>)
#endif // DUNE_CONCEPTS_MATRIX_HH
