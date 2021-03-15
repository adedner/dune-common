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
 * - `value_type`  The type of the elements stored in the collection
 * - `size_type`   Integer type used store size and maybe index information
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
 * Let `c` be a collection of type `C`
 *
 * \par Valid Expressions:
 * - `c.size()`: Returns the number of elements in the collection.
 *
 * \hideinitializer
 **/
template <class C>
concept AlgebraicCollection = Collection<C>
  && requires(const C& collection)
{
  { collection.size() } -> std::convertible_to<typename C::size_type>; // number of elements
};


/// \brief An AlgebraicMatrix is an AlgebraicCollection with number of rows and columns.
/**
 * A collection that provides the number of rows and columns.
 *
 * \par Notation:
 * Let `c` be a collection of type `C`
 *
 * \par Valid Expressions:
 * - `c.N()`: Returns the number of rows
 * - `c.M()`: Returns the number of columns
 *
 * \hideinitializer
 **/
template <class C>
concept AlgebraicMatrix = AlgebraicCollection<C>
  && requires(const C& collection)
{
  { collection.N() } -> std::convertible_to<typename C::size_type>; // number of rows
  { collection.M() } -> std::convertible_to<typename C::size_type>; // number of columns
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
 * \par Notation:
 * Let `iter` be the iterator of type `I`.
 *
 * \par Refinement of:
 * - `std::forward_iterator`
 *
 * \par Valid expressions:
 * - `i.index()`: The element-index inside the traversed collection
 *
 * \hideinitializer
 **/
template <class I, class SizeType>
concept ForwardIndexedIterator = std::forward_iterator<I>
  && requires(I iter)
{
  { iter.index() } -> std::convertible_to<SizeType>;
};


/// \brief A collection that can be traversed similar to a range.
/**
 * The concept models collections that have begin and end iterators
 * providing an element index.
 *
 * \par Notation:
 * Let `c` be a collection of type `C`
 *
 * \par Refinement of:
 * - `C` is a model of \ref Collection
 *
 * \par Valid expressions:
 * - `c.begin()`: Iterator to the first element int the collection
 * - `c.end()`: Iterator to the one-past-end element of the collection
 *
 * \hideinitializer
 **/
template <class C>
concept TraversableCollection = Collection<C>
  && requires(const C& collection)
{
  { collection.begin()  } -> ForwardIndexedIterator<typename C::size_type>;
  { collection.end()    } -> ForwardIndexedIterator<typename C::size_type>;
//{ collection.cbegin() } -> ForwardIndexedIterator<typename C::size_type>;
//{ collection.cend()   } -> ForwardIndexedIterator<typename C::size_type>;
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
 * \par Notation:
 * Let `c` be a collection of type `C`
 *
 * \par Refinement of:
 * - `C` is a model of \ref MutableCollection
 * - `C` is a model of \ref TraversableCollection
 *
 * \par Valid expressions:
 * - `c.begin()`: Output-iterator to the first element int the collection
 * - `c.end()`: Output-iterator to the one-past-end element of the collection
 *
 * \hideinitializer
 **/
template <class C>
concept TraversableMutableCollection = MutableCollection<C>
  && TraversableCollection<C>
  && requires(C& collection)
{
  { collection.begin() } -> ForwardOutputIterator<typename C::value_type, typename C::size_type>;
  { collection.end()   } -> ForwardOutputIterator<typename C::value_type, typename C::size_type>;
};


/// \brief A bidirectional iterator providing an element index.
/**
 * A bidirectional iterator that provides for each iterated element its index inside the
 * collection that is traversed.
 *
 * \par Notation:
 * Let `iter` be the iterator of type `I`.
 *
 * \par Refinement of:
 * - `std::bidirectional_iterator`
 *
 * \par Valid expressions:
 * - `i.index()`: The element-index inside the traversed collection
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
 * \par Notation:
 * Let `c` be a collection of type `C`
 *
 * \par Refinement:
 * - `C` is a model of \ref Collection
 *
 * \par Valid expressions:
 * - `c.beforeEnd()`: Iterator to the last element int the collection
 * - `c.beforeBegin()`: Iterator to the one-before-first element of the collection
 *
 * \hideinitializer
 **/
template <class C>
concept ReverseTraversableCollection = Collection<C>
  && requires(const C& collection)
{
  { collection.beforeEnd()   } -> BidirectionalIndexedIterator<typename C::size_type>;
  { collection.beforeBegin() } -> BidirectionalIndexedIterator<typename C::size_type>;
};


/// \brief A BidirectionalIndexedIterator that is also an std::output_iterator.
/// \hideinitializer
template <class I, class T, class SizeType>
concept BidirectionalOutputIterator = BidirectionalIndexedIterator<I,SizeType> && std::output_iterator<I,T>;


/// \brief A ReverseTraversableCollection with iterators are output-iterators.
/**
 * The concept models collections that have beforeEnd and beforeBegin outpt-iterators
 * to modify the collection elements. Those iterators also provide an element index.
 * It allows to traverse in reverse order starting from the last element in the collection.
 *
 * \par Notation:
 * Let `c` be a collection of type `C`
 *
 * \par Refinement:
 * - `C` is a model of \ref MutableCollection
 * - `C` is a model of \ref ReverseTraversableCollection
 *
 * \par Valid expressions:
 * - `c.beforeEnd()`: Output-iterator to the last element int the collection
 * - `c.beforeBegin()`: Output-iterator to the one-before-first element of the collection
 *
 * \hideinitializer
 **/
template <class C>
concept ReverseTraversableMutableCollection = MutableCollection<C>
  && ReverseTraversableCollection<C>
  && requires(C& collection)
{
  { collection.beforeEnd()   } -> BidirectionalOutputIterator<typename C::value_type, typename C::size_type>;
  { collection.beforeBegin() } -> BidirectionalOutputIterator<typename C::value_type, typename C::size_type>;
};

/** @} */

}} // end namespace Dune::Concept

#endif // __has_include(<concepts>)
#endif // DUNE_CONCEPTS_COLLECTION_HH
