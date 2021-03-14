// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_CONCEPTS_COLLECTION_HH
#define DUNE_CONCEPTS_COLLECTION_HH

#if __has_include(<concepts>)
#include <concepts>
#include <iterator>

namespace Dune {
namespace Concept {

// Type information

template <class C>
concept Collection = requires
{
  typename C::value_type;
  typename C::const_reference;
  typename C::size_type;
};

template <class C>
concept MutableCollection = requires
{
  typename T::reference;
};


// Size information

template <class C>
concept AlgebraicCollection = Collection<C>
  && requires(const C& collection)
{
  { collection.N() } -> std::convertible_to<typename C::size_type>; // number of rows
  { collection.M() } -> std::convertible_to<typename C::size_type>; // number of columns
  { collection.size() } -> std::convertible_to<typename C::size_type>; // number of elements
};

template <class C>
concept ConstantSizeAlgebraicCollection = AlgebraicCollection<C>
  && requires
{
  std::integral_constant<typename C::size_type, C::N()>;
  std::integral_constant<typename C::size_type, C::M()>;
  std::integral_constant<typename C::size_type, C::size()>;
};


// Traversable

template <class C>
concept TraversableCollection = Collection<C>
  && requires(const C& collection)
{
  { collection.begin()  } -> std::forward_iterator;
  { collection.end()    } -> std::forward_iterator;
//{ collection.cbegin() } -> std::forward_iterator;
//{ collection.cend()   } -> std::forward_iterator;
};

template <class I>
concept ForwardOutputIterator = std::forward_iterator<I> && std::output_iterator<I>;

template <class C>
concept TraversableMutableCollection = MutableCollection<C>
  && TraversableCollection<C>
  && requires(C& collection)
{
  { collection.begin() } -> ForwardOutputIterator;
  { collection.end()   } -> ForwardOutputIterator;
};



// Reverse-Traversable

template <class C>
concept ReverseTraversableCollection = Collection<C>
  && requires(const C& collection)
{
  { collection.beforeEnd()   } -> std::bidirectional_iterator;
  { collection.beforeBegin() } -> std::bidirectional_iterator;
};

template <class I>
concept BidirectionalOutputIterator = std::bidirectional_iterator<I> && std::output_iterator<I>;

template <class C>
concept ReverseTraversableMutableCollection = MutableCollection<C>
  && ReverseTraversableCollection<C>
  && requires(C& collection)
{
  { collection.beforeEnd()   } -> BidirectionalOutputIterator;
  { collection.beforeBegin() } -> BidirectionalOutputIterator;
};

}} // end namespace Dune::Concepts

#endif // __has_include(<concepts>)
#endif // DUNE_CONCEPTS_COLLECTION_HH
