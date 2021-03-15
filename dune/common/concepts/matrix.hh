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
 * \par Notation:
 * Let `m` be a matrix of type `M` and `i,j` indices of `size_type`
 *
 * \par Refinement:
 * - `M` is a model of \ref AlgebraicMatrix
 *
 * \par Valid expressions:
 * - `m[i][j]`: Access the (i,j)'th element of the matrix where `i` is the row index
 *   and `j` the column index
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
 * \par Notation:
 * Let `m` be a matrix of type `M` and `i,j` indices of `size_type`
 *
 * \par Refinement:
 * - `M` is a model of \ref Matrix
 *
 * \par Valid expressions:
 * - `m[i][j]`: Mutable access to the (i,j)'th element of the matrix where `i` is the
 *   row index and `j` the column index
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
 * \par Notation:
 * Let `m` be a matrix of type `M` and `r,c` new number of rows and columns of `size_type`
 *
 * \par Refinement:
 * - `M` is a model of \ref Matrix
 *
 * \par Valid expressions:
 * - `m.resize(r,c)`: Resize the matrix to the new size `rxc` where `r` is the number of
 *   rows and `c` is the number of columns.
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
 * \par Notation:
 * Let `m` be a matrix of type `M`
 *
 * \par Refinement:
 * - `M` is a model of \ref TraversableCollection
 *
 * \par Valid expressions:
 * - `*m.begin()`: Return a traversable collection, meaning the dereferenced iterator can
 *   be traversed again and the traversal iterator id a model of TraversableCollection.
 *
 * \hideinitializer
 **/
template <class M>
concept TraversableMatrix = TraversableCollection<M>
  && requires(const M& matrix)
{
  requires TraversableCollection<std::decay_t<decltype(*matrix.begin())>>;
};

/// \brief A \ref MutableMatrix that is traversable
/**
 * Refinement of \ref TraversableMatrix but with mutable traversal.
 **/
template <class M>
concept TraversableMutableMatrix = TraversableMatrix<M>
  && TraversableMutableCollection<M>
  && requires(M& matrix)
{
  requires TraversableMutableCollection<std::decay_t<decltype(*matrix.begin())>>;
};


/// \brief Traits class indicating whether matrix-traversal is row-major.
/// \hideinitializer
template <class M>
struct IsRowMajor : std::false_type {};

/// \brief Traits class indicating whether matrix-traversal is column-major.
/// \hideinitializer
template <class M>
struct IsColMajor : std::false_type {};


/// \brief A traversable matrix with row-major traversal.
template <class M>
concept RowMajorTraversableMatrix = Matrix<M> && TraversableMatrix<M> && IsRowMajor<M>::value;

/// \brief A traversable matrix with column-major traversal.
template <class M>
concept ColMajorTraversableMatrix = Matrix<M> && TraversableMatrix<M> && IsColMajor<M>::value;



/// \brief A \ref Matrix that might have a sparse storage of the nonzero entries.
/**
 * The concept models matrices with possibly sparse storage, like banded matrices or
 * crs/ccs matrices.
 *
 * \par Notation:
 * Let `m` be a matrix of type `M`, `i,j` indices of `size_type`
 *
 * \par Refinement:
 * - `M` is a model of \ref Matrix
 *
 * \par Valid expressions:
 * - `m.exists(i,j)`: Return whether the element `(i,j)` is stored in the sparse matrix
 *   and thus a nonzero element.
 * - `m.nonzeroes()`: Return the number of nonzero elements stored in the sparse matrix.
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
