// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_CONCEPTS_MULTITYPE_HH
#define DUNE_CONCEPTS_MULTITYPE_HH

#if __has_include(<concepts>) || DOXYGEN
#include <type_traits>
#include <utility>

#include <dune/common/concepts/collection.hh>
#include <dune/common/std/type_traits.hh>

namespace Dune {
namespace Concept {

/**
 * \ingroup CxxConcepts
 * @{
 **/

/// \brief A MultiTypeCollection is a collection of elements of possible different types
/**
 * \par Associated types:
 * - `size_type`: An integer type that can represent the number of elements
 *
 * \hideinitializer
 **/
template <class C>
concept MultiTypeCollection = requires
{
  typename C::size_type;
};

/// \brief A MultiTypeAlgebraicCollection is a collection with static size information
/**
 * \par Refinement of:
 * - \ref MultiTypeCollection
 *
 * \par Valid expressions:
 * - `C::size()`: Returns constexpr number of elements in the collection. `[[post: C::size() >= 0]]`
 *
 * \hideinitializer
 **/
template <class C>
concept MultiTypeAlgebraicCollection = MultiTypeCollection<C>
  && requires
{
  std::integral_constant<typename C::size_type, C::size()>{};
};


/// \brief A MultiTypeAlgebraicMatrix is a collection with static number of rows and columns
/**
 * \par Refinement of:
 * - \ref MultiTypeAlgebraicCollection
 *
 * \par Valid expressions:
 * - `C::N()`: Returns constexpr number of rows in the matrix. `[[post: C::N() >= 0]]`
 * - `C::M()`: Returns constexpr number of columns in the matrix. `[[post: C::M() >= 0]]`
 *
 * \hideinitializer
 **/
template <class C>
concept MultiTypeAlgebraicMatrix = MultiTypeAlgebraicCollection<C>
  && requires
{
  std::integral_constant<typename C::size_type, C::N()>{};
  std::integral_constant<typename C::size_type, C::M()>{};
};


namespace Impl
{
  template <class M, class I, class J>
  using IndexAccessibleConstantMatrix = decltype(
    std::declval<const M&>()[std::declval<I>()][std::declval<J>()]
  );

  template <class M, class I, class J>
  using MutableIndexAccessibleConstantMatrix = decltype(
    std::declval<M&>()[std::declval<I>()][std::declval<J>()]
  );

  template <class M, class RowSeq, class ColSeq,
            template <class,class,class> class Check>
  struct AllIndexAccessibleMat;

  template <class M, class S, S... rows, class ColSeq,
            template <class,class,class> class Check>
  struct AllIndexAccessibleMat<M, std::integer_sequence<S, rows...>, ColSeq, Check>
  {
    template <S row, class Seq> struct Helper;
    template <S row, S... cols> struct Helper<row, std::integer_sequence<S, cols...>>
      : std::conjunction<Dune::Std::is_detected<Check, M,
          std::integral_constant<S,row>, std::integral_constant<S,cols>>...> {};

    static constexpr bool value = (Helper<rows,ColSeq>::value &&...);
  };

} // end namespace Impl


/// \brief A MultiTypeMatrix is a matrix storing different type of elements that can be
/// accessed directly.
/**
 * \par Refinement of:
 * - \ref MultiTypeAlgebraicMatrix
 *
 * \par Notation:
 * - `m`: a matrix of type `M`
 * - `i,j`: integral constants of `size_type` with values in the range `0 <= i < C::N()`
 *   and `0 <= j < C::M()`.
 *
 * \par Valid expressions:
 * - `m[i][j]`: Access the (i,j)'th element of the matrix where `i` is the row index
 *   and `j` the column index. Note, all indices must be given as
 *   `std::integral_constant<size_type, ...>`.
 *
 * \hideinitializer
 **/
template <class M>
concept MultiTypeMatrix = MultiTypeAlgebraicMatrix<M> &&
  Impl::AllIndexAccessibleMat<M, std::make_integer_sequence<typename M::size_type, M::N()>,
                                 std::make_integer_sequence<typename M::size_type, M::M()>,
                              Impl::IndexAccessibleConstantMatrix>::value;

/// \brief A MutableMultiTypeMatrix is a \ref MultiTypeMatrix providing mutable access.
/**
 * \par Refinement of:
 * - \ref MultiTypeMatrix
 *
 * \par Notation:
 * - `m`: a matrix of type `M`
 * - `i,j`: integral constants of `size_type` with values in the range `0 <= i < C::N()`
 *   and `0 <= j < C::M()`.
 *
 * \par Valid expressions:
 * - `m[i][j]`: Mutable access to the (i,j)'th element of the matrix where `i` is the row index
 *   and `j` the column index. Note, all indices must be given as
 *   `std::integral_constant<size_type, ...>`.
 *
 * \hideinitializer
 **/
template <class M>
concept MutableMultiTypeMatrix = MultiTypeMatrix<M> &&
  Impl::AllIndexAccessibleMat<M, std::make_integer_sequence<typename M::size_type, M::N()>,
                                 std::make_integer_sequence<typename M::size_type, M::M()>,
                              Impl::MutableIndexAccessibleConstantMatrix>::value;


namespace Impl
{
  template <class V, class I>
  using IndexAccessibleConstantVector = decltype(
    std::declval<const V&>()[std::declval<I>()]
  );

  template <class V, class I>
  using MutableIndexAccessibleConstantVector = decltype(
    std::declval<V&>()[std::declval<I>()]
  );

  template <class V, class Seq, template <class,class> class Check>
  struct AllIndexAccessibleVec;

  template <class V, class S, S... indices, template <class,class> class Check>
  struct AllIndexAccessibleVec<V, std::integer_sequence<S, indices...>, Check>
  {
    static constexpr bool value
      = (Dune::Std::is_detected_v<Check, V, std::integral_constant<S,indices>> &&...);
  };

} // end namespace Impl


/// \brief A MultiTypeVector is a vector storing different types of elements that can be
/// accessed directly.
/**
 * \par Refinement of:
 * - \ref MultiTypeAlgebraicCollection
 *
 * \par Notation:
 * - `v`: a vector of type `V`.
 * - `i`: integral constants of `size_type` with values in the range `0 <= i < C::size()`.
 *
 * \par Valid expressions:
 * - `v[i]`: Access the i'th element of the vector where `i` must be given as
 *   `std::integral_constant<size_type, ...>`
 *
 * \hideinitializer
 **/
template <class V>
concept MultiTypeVector = MultiTypeAlgebraicCollection<V> &&
  Impl::AllIndexAccessibleVec<V, std::make_integer_sequence<typename V::size_type, V::size()>,
                              Impl::IndexAccessibleConstantVector>::value;


/// \brief A MutableMultiTypeVector is a \ref MultiTypeVector with mutable access to its elements
/**
 * \par Refinement of:
 * - \ref MultiTypeVector
 *
 * \par Notation:
 * - `v`: a vector of type `V`.
 * - `i`: integral constants of `size_type` with values in the range `0 <= i < C::size()`.
 *
 * \par Valid expressions:
 * - `v[i]`: Mutable access to the i'th element of the vector where `i` must be given as
 *   `std::integral_constant<size_type, ...>`
 *
 * \hideinitializer
 **/
template <class V>
concept MutableMultiTypeVector = MultiTypeVector<V> &&
  Impl::AllIndexAccessibleVec<V, std::make_integer_sequence<typename V::size_type, V::size()>,
                              Impl::MutableIndexAccessibleConstantVector>::value;

/** @} */

}} // end namespace Dune::Concept

#endif // __has_include(<concepts>)
#endif // DUNE_CONCEPTS_MULTITYPE_HH
