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
concept Matrix = AlgebraicMatrix<M>
  && IndexAccessibleMatrix<M, typename M::size_type, typename M::size_type>;

template <class M>
concept MutableMatrix = Matrix<M>
  && MutableIndexAccessibleMatrix<M, typename M::size_type, typename M::size_type>;

template <class M>
concept ConstantSizeMatrix = Matrix<M>
  && ConstantSizeAlgebraicMatrix<M>;

template <class M>
concept MutableConstantSizeMatrix = MutableMatrix<M>
  && ConstantSizeAlgebraicMatrix<M>;


// Resizeable

template <class M>
concept ResizeableMatrix = Matrix<M>
  && requires(M& matrix, typename M::size_type r, typename M::size_type c)
{
  matrix.resize(r, c);
};


// Traversal

template <class M>
concept TraversableMatrix = TraversableCollection<M>
  && requires(const M& matrix)
{
  requires TraversableCollection<std::decay_t<decltype(*matrix.begin())>>;
};

template <class M>
concept TraversableMutableMatrix = TraversableMutableCollection<M>
  && requires(M& matrix)
{
  requires TraversableMutableCollection<std::decay_t<decltype(*matrix.begin())>>;
};

template <class M>
struct IsRowMajor : std::false_type {};

template <class M>
struct IsColMajor : std::false_type {};


template <class M>
concept RowMajorTraversableMatrix = Matrix<M> && TraversableMatrix<M> && IsRowMajor<M>::value;

template <class M>
concept ColMajorTraversableMatrix = Matrix<M> && TraversableMatrix<M> && IsColMajor<M>::value;


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
  matrix.diagonal();
  requires Vector<std::decay_t<decltype(matrix.diagonal())>>;
  { matrix.diagonal(i) } -> std::convertible_to<typename M::value_type>;
};

template <class M>
concept MutableDiagonalMatrix = DiagonalMatrix<M>
  && requires(M& matrix, typename M::size_type i, typename M::value_type value)
{
  requires MutableVector<std::decay_t<decltype(matrix.diagonal())>>;
  matrix.diagonal(i) = value;
};


}} // end namespace Dune::Concept

#endif //__has_include(<concepts>)
#endif // DUNE_CONCEPTS_MATRIX_HH
