// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_CONCEPTS_VECTOR_HH
#define DUNE_CONCEPTS_VECTOR_HH

#if __has_include(<concepts>) || DOXYGEN
#include <concepts>

#include <dune/common/concepts/collection.hh>

namespace Dune {
namespace Concept {

/**
 * \ingroup CxxConcepts
 * @{
 **/

/// \brief A vector is a collection with direct element acccess.
/**
 * The concept models vector-like collections, providing indexed element access.
 *
 * \par Refinement of:
 * - \ref AlgebraicCollection
 *
 * \par Notation:
 * - `v`: a vector of type `V`.
 * - `i`: an index of `size_type`.
 *
 * \par Valid expressions:
 * - `v[i]`: Access the i'th element of the vector. `[[pre: 0 <= i < v.size()]]`
 *
 * \par Models:
 * - \ref FieldVector
 * - \ref DynamicVector
 *
 * \hideinitializer
 **/
template <class V>
concept Vector = AlgebraicCollection<V>
  && requires(const V& vector, typename V::size_type i)
{
  { vector[i] } -> std::convertible_to<typename V::value_type>;
};


/// \brief A mutable vector is a collection with direct mutable element acccess.
/**
 * The concept models vector-like collections, providing mutable indexed element access.
 *
 * \par Refinement of:
 * - \ref Vector
 *
 * \par Notation:
 * - `v`: a vector of type `V`.
 * - `i`: an index of `size_type`.
 * - `value` of `value_type`.
 *
 * \par Valid expressions:
 * - `v[i]`: Mutable access to the i'th element of the vector. `[[pre: 0 <= i < v.size()]]`
 *
 * \hideinitializer
 **/
template <class V>
concept MutableVector = Vector<V>
  && requires(V& vector, typename V::size_type i, typename V::value_type value)
{
  vector[i] = value;
};


/// \brief A \ref Vector with constant size.
/// \hideinitializer
template <class V>
concept ConstantSizeVector = Vector<V> && ConstantSizeAlgebraicCollection<V>;

/// \brief A \ref MutableVector with constant size.
/// \hideinitializer
template <class V>
concept MutableConstantSizeVector = MutableVector<V> && ConstantSizeAlgebraicCollection<V>;


/// \brief A vector that can be resized
/**
 * The concept models vector collections with `resize()` function.
 *
 * \par Refinement of:
 * - \ref Vector
 *
 * \par Notation:
 * - `v`: a vector of type `V`.
 * - `s`: new size of `size_type`.
 *
 * \par Valid expressions:
 * - `v.resize(s)`: Resize the vector to the new size `s`: `[[pre: s >= 0]]`
 *
 * \par Models:
 * - \ref DynamicVector
 *
 * \hideinitializer
 **/
template <class V>
concept ResizeableVector = Vector<V>
  && requires(V& vector, typename V::size_type s)
{
  vector.resize(s);
};


/// \brief A \ref Vector that is traversable.
/// \hideinitializer
template <class V>
concept TraversableVector = Vector<V> && TraversableCollection<V>;

/** @} */

}} // end namespace Dune::Concept

#endif // __has_include(<concepts>)
#endif // DUNE_CONCEPTS_VECTOR_HH
