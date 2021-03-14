// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_CONCEPTS_VECTOR_HH
#define DUNE_CONCEPTS_VECTOR_HH

#if __has_include(<concepts>)
#include <concepts>

#include <dune/common/concepts/collection.hh>

namespace Dune {
namespace Concept {

// Element access

template <class V, class I>
concept IndexAccessibleVector = Collection<V>
  && std::convertible_to<I, typename V::size_type>
  && requires(const V& vector, I i)
{
  { vector[i] } -> std::convertible_to<typename V::value_type>;
};

template <class V, class I>
concept MutableIndexAccessibleVector = MutableCollection<V>
  && std::convertible_to<I, typename V::size_type>
  && requires(V& vector, I i, typename V::value_type value)
{
  vector[i] = value;
};


// Vector types

template <class V>
concept Vector = AlgebraicCollection<V>
  && IndexAccessibleVector<V, typename V::size_type>;

template <class V>
concept MutableVector = Vector<V>
  && MutableIndexAccessibleVector<V, typename V::size_type>;

template <class V>
concept ConstantSizeVector = Vector<V>
  && ConstantSizeAlgebraicCollection<V>;

template <class V>
concept MutableConstantSizeVector = MutableVector<V>
  && ConstantSizeAlgebraicCollection<V>;


// Resizeable

template <class V>
concept ResizeableVector = Vector<V>
  && requires(V& vector, typename V::size_type s)
{
  vector.resize(s);
};


// Traversal

template <class V>
concept TraversableVector = Vector<V> && TraversableCollection<V>;


}} // end namespace Dune::Concepts

#endif // __has_include(<concepts>)
#endif // DUNE_CONCEPTS_VECTOR_HH
