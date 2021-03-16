// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_CONCEPTS_COLLECTION_HH
#define DUNE_CONCEPTS_COLLECTION_HH

#if __has_include(<concepts>) || DOXYGEN
#include <concepts>
#include <iterator>

namespace Dune {
namespace Concept {

/**
 * \ingroup CxxConcepts
 * @{
 **/

/// \brief A Collection is the base concept of matrices and vectors
/**
 * A data-structure implementing a collection of elements of the same type.
 * The concept \ref Concept::Collection gives the notion of some associated types
 * these data-structures need to provide.
 *
 * \par Associated types:
 * - `C::value_type`: The type of the elements stored in the collection
 * - `C::size_type`: Integer type used to store size and maybe index information
 *
 * \hideinitializer
 **/
template <class C>
concept Collection = requires
{
  typename C::value_type;
  typename C::size_type;
//  typename C::const_reference;
};


/// \brief MutableCollection is a Collection that allows to modify its elements.
/**
 * \par Refinement of:
 * - \ref Collection
 *
 * \hideinitializer
 **/
template <class C>
concept MutableCollection = Collection<C>
 && requires
{
  // typename C::reference;
  true;
};


/// \brief An AlgebraicCollection is a Collection with size information.
/**
 * A collection that provides the size information, i.e., the number of elements
 * stored in the collection.
 *
 * \par Refinement of:
 * - \ref Collection
 *
 * \par Notation:
 * - `c`: a collection of type `C`
 *
 * \par Valid Expressions:
 * - `c.size()`: Returns the number of elements in the collection. `[[post: c.size() >= 0]]`
 *
 * \hideinitializer
 **/
template <class C>
concept AlgebraicCollection = Collection<C>
  && requires(const C& collection)
{
  { collection.size() } -> std::convertible_to<typename C::size_type>;
};


/// \brief An AlgebraicMatrix is an AlgebraicCollection with number of rows and columns.
/**
 * A collection that provides the number of rows and columns.
 *
 * \par Refinement of:
 * - \ref AlgebraicCollection
 *
 * \par Notation:
 * - `c`: a collection of type `C`
 *
 * \par Valid Expressions:
 * - `c.N()`: Returns the number of rows. `[[post: c.N() >= 0]]`
 * - `c.M()`: Returns the number of columns. `[[post: c.M() >= 0]]`
 *
 * \hideinitializer
 **/
template <class C>
concept AlgebraicMatrix = AlgebraicCollection<C>
  && requires(const C& collection)
{
  { collection.N() } -> std::convertible_to<typename C::size_type>;
  { collection.M() } -> std::convertible_to<typename C::size_type>;
};


/// \brief An AlgebraicCollection with size is constexpr.
/// \hideinitializer
template <class C>
concept ConstantSizeAlgebraicCollection = AlgebraicCollection<C>
  && requires
{
  { std::integral_constant<typename C::size_type, C::size()>{} };
};


/// \brief An AlgebraicMatrix with rows and columns is constexpr.
/// \hideinitializer
template <class C>
concept ConstantSizeAlgebraicMatrix = AlgebraicMatrix<C>
  && requires
{
  { std::integral_constant<typename C::size_type, C::N()>{} };
  { std::integral_constant<typename C::size_type, C::M()>{} };
};


/// \brief A ForwardIterator with element-index.
/**
 * A forward iterator that provides for each iterated element its index inside the
 * collection that is traversed.
 *
 * \par Refinement of:
 * - `std::forward_iterator`
 *
 * \par Notation:
 * - `iter`: the iterator of type `I`.
 *
 * \par Valid expressions:
 * - `iter.index()`: The element-index inside the traversed collection. `[[post: iter.index() >= 0]]`
 *
 * \hideinitializer
 **/
template <class I, class SizeType>
concept ForwardIndexedIterator = std::forward_iterator<I>
  && requires(I iter)
{
  { iter.index() } -> std::convertible_to<SizeType>;
};



struct Identity
{
  template <class C>
  decltype(auto) operator()(C&& collection) const { return collection; }
};


/// \brief A collection that can be traversed similar to a range.
/**
 * The concept models collections that have begin and end iterators
 * providing an element index.
 *
 * \par Refinement of:
 * - \ref Collection
 *
 * \par Notation:
 * - `c`: a collection of type `C`
 *
 * \par Valid expressions:
 * - `c.begin()`: Iterator to the first element int the collection
 * - `c.end()`: Iterator to the one-past-end element of the collection
 *
 * \hideinitializer
 **/
template <class C, class RangeWrapper = Identity>
concept TraversableCollection = Collection<C>
  && requires(const C& collection, RangeWrapper wrapper)
{
  { wrapper(collection).begin()  } -> ForwardIndexedIterator<typename C::size_type>;
  { wrapper(collection).end()    } -> ForwardIndexedIterator<typename C::size_type>;
};


/// \brief A ForwardIndexedIterator that is also an std::output_iterator.
/// \hideinitializer
template <class I, class T, class SizeType>
concept ForwardOutputIterator = ForwardIndexedIterator<I,SizeType> && std::output_iterator<I,T>;


/// \brief A TraversableCollection with iterators are output-iterators.
/**
 * The concept models collections that have begin and end output-iterators
 * to modify the collection elements. Those iterators also provide an element index.
 *
 * \par Refinement of:
 * - \ref MutableCollection
 * - \ref TraversableCollection
 *
 * \par Notation:
 * - `c`: a collection of type `C`
 *
 * \par Valid expressions:
 * - `c.begin()`: Output-iterator to the first element int the collection
 * - `c.end()`: Output-iterator to the one-past-end element of the collection
 *
 * \hideinitializer
 **/
template <class C, class RangeWrapper = Identity>
concept TraversableMutableCollection = MutableCollection<C>
  && TraversableCollection<C>
  && requires(C& collection, RangeWrapper wrapper)
{
  { wrapper(collection).begin() } -> ForwardOutputIterator<typename C::value_type, typename C::size_type>;
  { wrapper(collection).end()   } -> ForwardOutputIterator<typename C::value_type, typename C::size_type>;
};


/// \brief A bidirectional iterator providing an element index.
/**
 * A bidirectional iterator that provides for each iterated element its index inside the
 * collection that is traversed.
 *
 * \par Refinement of:
 * - `std::bidirectional_iterator`
 *
 * \par Notation:
 * - `iter`: the iterator of type `I`.
 *
 * \par Valid expressions:
 * - `iter.index()`: The element-index inside the traversed collection. `[[post: iter.index() >= 0]]`
 *
 * \hideinitializer
 **/
template <class I, class SizeType>
concept BidirectionalIndexedIterator = std::bidirectional_iterator<I>
  && requires(I iter)
{
  { iter.index() } -> std::convertible_to<SizeType>;
};


/// \brief A collection that can be traversed in reverse order.
/**
 * The concept models collections that have beforeEnd and beforeBegin iterators
 * providing an element index. It allows to traverse in reverse order starting
 * from the last element in the collection.
 *
 * \par Refinement of:
 * - \ref Collection
 *
 * \par Notation:
 * - `c`: a collection of type `C`
 *
 * \par Valid expressions:
 * - `c.beforeEnd()`: Iterator to the last element int the collection
 * - `c.beforeBegin()`: Iterator to the one-before-first element of the collection
 *
 * \hideinitializer
 **/
template <class C, class RangeWrapper = Identity>
concept ReverseTraversableCollection = Collection<C>
  && requires(const C& collection, RangeWrapper wrapper)
{
  { wrapper(collection).beforeEnd()   } -> BidirectionalIndexedIterator<typename C::size_type>;
  { wrapper(collection).beforeBegin() } -> BidirectionalIndexedIterator<typename C::size_type>;
};


/// \brief A BidirectionalIndexedIterator that is also an std::output_iterator.
/// \hideinitializer
template <class I, class T, class SizeType>
concept BidirectionalOutputIterator = BidirectionalIndexedIterator<I,SizeType> && std::output_iterator<I,T>;


/// \brief A ReverseTraversableCollection with iterators are output-iterators.
/**
 * The concept models collections that have beforeEnd and beforeBegin output-iterators
 * to modify the collection elements. Those iterators also provide an element index.
 * It allows to traverse in reverse order starting from the last element in the collection.
 *
 * \par Refinement of:
 * - \ref MutableCollection
 * - \ref ReverseTraversableCollection
 *
 * \par Notation:
 * - `c`: a collection of type `C`
 *
 * \par Valid expressions:
 * - `c.beforeEnd()`: Output-iterator to the last element int the collection
 * - `c.beforeBegin()`: Output-iterator to the one-before-first element of the collection
 *
 * \hideinitializer
 **/
template <class C, class RangeWrapper = Identity>
concept ReverseTraversableMutableCollection = MutableCollection<C>
  && ReverseTraversableCollection<C>
  && requires(C& collection, RangeWrapper wrapper)
{
  { wrapper(collection).beforeEnd()   } -> BidirectionalOutputIterator<typename C::value_type, typename C::size_type>;
  { wrapper(collection).beforeBegin() } -> BidirectionalOutputIterator<typename C::value_type, typename C::size_type>;
};

/** @} */

}} // end namespace Dune::Concept

#endif // __has_include(<concepts>)
#endif // DUNE_CONCEPTS_COLLECTION_HH
