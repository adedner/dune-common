// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_CONCEPTS_MATRIX_HH
#define DUNE_CONCEPTS_MATRIX_HH

#if __has_include(<concepts>)
#include <concepts>

#include <dune/common/concepts/collection.hh>
#include <dune/common/concepts/vector.hh>

namespace Dune {
namespace Concept {

// Element access

template <class M, class I, class J>
concept IndexAccessibleMatrix = Collection<M>
  && std::convertible_to<I, typename M::size_type>
  && std::convertible_to<J, typename M::size_type>
  && requires(const M& matrix, I i, J j)
{
  { matrix[i][j] } -> std::convertible_to<typename M::value_type>;
};

template <class M, class I, class J>
concept MutableIndexAccessibleMatrix = MutableCollection<M>
  && std::convertible_to<I, typename M::size_type>
  && std::convertible_to<J, typename M::size_type>
  && requires(M& matrix, I i, J j, typename M::value_type value)
{
  matrix[i][j] = value;
};


// Matrix types

template <class M>
concept Matrix = AlgebraicCollection<M>
  && IndexAccessibleMatrix<M, typename M::size_type, typename M::size_type>;

template <class M>
concept MutableMatrix = Matrix<M>
  && MutableIndexAccessibleMatrix<M, typename M::size_type, typename M::size_type>;

template <class M>
concept ConstantSizeMatrix = Matrix<M>
  && ConstantSizeAlgebraicCollection<M>;

template <class M>
concept MutableConstantSizeMatrix = MutableMatrix<M>
  && ConstantSizeAlgebraicCollection<M>;


// Resizeable

template <class M>
concept ResizeableMatrix = Matrix<M>
  && requires(M& matrix, typename M::size_type r, typename M::size_type c)
{
  matrix.resize(r, c);
};


// Traversal

template <class M>
struct IsRowMajor : std::false_type {};

template <class M>
struct IsColMajor : std::false_type {};


template <class M>
concept RowMajorTraversableMatrix = Matrix<M> && TraversableCollection<M> && IsRowMajor<M>::value;

template <class M>
concept ColMajorTraversableMatrix = Matrix<M> && TraversableCollection<M> && IsColMajor<M>::value;


// Sparse matrices

template <class M>
concept SparseMatrix = Matrix<M>
  && requires(const M& matrix, typename M::size_type i, typename M::size_type j)
{
  { matrix.exists(i,j) } -> std::convertible_to<bool>;
  { matrix.nonzeroes() } -> std::convertible_to<typename M::size_type>;
};

template <class M>
concept DiagonalMatrix = SparseMatrix<M>
  && requires(const M& matrix, typename M::size_type i)
{
  { matrix.diagonal()  } -> Vector;
  { matrix.diagonal(i) } -> std::convertible_to<typename M::value_type>
};

template <class M>
concept MutableDiagonalMatrix = DiagonalMatrix<M>
  && requires(M& matrix, typename M::size_type i)
{
  { matrix.diagonal()  } -> MutableVector;
  { matrix.diagonal(i) } -> std::convertible_to<typename M::reference>
};


}} // end namespace Dune::Concepts

#endif //__has_include(<concepts>)
#endif // DUNE_CONCEPTS_MATRIX_HH
