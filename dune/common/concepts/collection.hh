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
//  typename C::const_reference;
  typename C::size_type;
};

template <class C>
concept MutableCollection = Collection<C>
//  && requires
//{
//  typename C::reference;
//}
;


// Size information

template <class C>
concept AlgebraicVector = Collection<C>
  && requires(const C& collection)
{
  { collection.size() } -> std::convertible_to<typename C::size_type>; // number of elements
};

template <class C>
concept AlgebraicMatrix = Collection<C>
  && requires(const C& collection)
{
  { collection.N() } -> std::convertible_to<typename C::size_type>; // number of rows
  { collection.M() } -> std::convertible_to<typename C::size_type>; // number of columns
};

template <class C>
concept ConstantSizeAlgebraicVector = AlgebraicVector<C>
  && requires
{
  { std::integral_constant<typename C::size_type, C::size()>{} };
};

template <class C>
concept ConstantSizeAlgebraicMatrix = AlgebraicMatrix<C>
  && requires
{
  { std::integral_constant<typename C::size_type, C::N()>{} };
  { std::integral_constant<typename C::size_type, C::M()>{} };
};

// Traversable

template <class I, class SizeType>
concept ForwardIndexedIterator = std::forward_iterator<I>
  && requires(I iter)
{
  { iter.index() } -> std::convertible_to<SizeType>;
};

template <class C>
concept TraversableCollection = Collection<C>
  && requires(const C& collection)
{
  { collection.begin()  } -> ForwardIndexedIterator<typename C::size_type>;
  { collection.end()    } -> ForwardIndexedIterator<typename C::size_type>;
//{ collection.cbegin() } -> ForwardIndexedIterator<typename C::size_type>;
//{ collection.cend()   } -> ForwardIndexedIterator<typename C::size_type>;
};

template <class I, class T, class SizeType>
concept ForwardOutputIterator = ForwardIndexedIterator<I,SizeType> && std::output_iterator<I,T>;

template <class C>
concept TraversableMutableCollection = MutableCollection<C>
  && TraversableCollection<C>
  && requires(C& collection)
{
  { collection.begin() } -> ForwardOutputIterator<typename C::value_type, typename C::size_type>;
  { collection.end()   } -> ForwardOutputIterator<typename C::value_type, typename C::size_type>;
};



// Reverse-Traversable

template <class I, class SizeType>
concept BidirectionalIndexedIterator = std::bidirectional_iterator<I>
  && requires(I iter)
{
  { iter.index() } -> std::convertible_to<SizeType>;
};

template <class C>
concept ReverseTraversableCollection = Collection<C>
  && requires(const C& collection)
{
  { collection.beforeEnd()   } -> BidirectionalIndexedIterator<typename C::size_type>;
  { collection.beforeBegin() } -> BidirectionalIndexedIterator<typename C::size_type>;
};

template <class I, class T, class SizeType>
concept BidirectionalOutputIterator = BidirectionalIndexedIterator<I,SizeType> && std::output_iterator<I,T>;

template <class C>
concept ReverseTraversableMutableCollection = MutableCollection<C>
  && ReverseTraversableCollection<C>
  && requires(C& collection)
{
  { collection.beforeEnd()   } -> BidirectionalOutputIterator<typename C::value_type, typename C::size_type>;
  { collection.beforeBegin() } -> BidirectionalOutputIterator<typename C::value_type, typename C::size_type>;
};

}} // end namespace Dune::Concept

#endif // __has_include(<concepts>)
#endif // DUNE_CONCEPTS_COLLECTION_HH
