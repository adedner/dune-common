// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_VECTOR_HH
#define DUNE_COMMON_VECTOR_HH

#include <algorithm>
#if __has_include(<compare>)
  #include <compare>
#endif
#include <cstddef>
#include <initializer_list>
#include <limits>
#include <memory>
#include <stdexcept>
#include <type_traits>

#include <dune/common/assert.hh>
#include <dune/common/std/no_unique_address.hh>
#include <dune/common/ftraits.hh>

namespace Dune {

/**
 * \brief Vector is a sequence container that encapsulates dynamic size arrays.
 *
 * This sequence container data structure is similar to the `std::vector` container
 * without the inremental size-changing operations, like `push_back`. The motivation
 * for developing our own data container is the unfortunate specialization of the
 * `std::vector<bool>` with proxy reference types. This leads to lots of codes where
 * boolean values are to be represented as `char` or `int`.
 *
 * This container is a more restrictive than the `std::vector` implementation. We
 * do not allow incremental size changes. The `resize()` function creates a new
 * storage even if the size is reduced.
 *
 * \tparam Element The type of the elements stored in the container.
 * \tparam An allocator that is used to acquire/release memory and to construct/destroy
 *         the elements in that memory.
 */
template <class Element,
          class Allocator = std::allocator<Element>>
class Vector
{
  static_assert(std::is_same_v<Element, typename Allocator::value_type>);

public:
  using ElementType = Element;
  using ValueType = std::remove_const_t<ElementType>;
  using AllocatorType = Allocator;
  using SizeType = typename AllocatorType::size_type;
  using DifferenceType = typename AllocatorType::difference_type;
  using Reference = ElementType&;
  using ConstReference = const ElementType&;
  using Pointer = ElementType*;
  using ConstPointer = const ElementType*;
  using Iterator = Pointer;
  using ConstIterator = ConstPointer;

public: // typedefs following the std::vector interface
  using value_type = ValueType;
  using size_type = SizeType;
  using difference_type = DifferenceType;
  using allocator_type = AllocatorType;
  using reference = Reference;
  using const_reference = ConstReference;
  using pointer = Pointer;
  using const_pointer = ConstPointer;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

public:

  /// \name Constructors and Destructor
  /// @{

  /// \brief Construct an empty vector and default construct the allocator.
  constexpr Vector () noexcept( noexcept(AllocatorType()) )
  {}

  /// \brief Construct an empty vector and construct the allocator from the argument.
  constexpr explicit Vector (const AllocatorType& alloc) noexcept
    : alloc_(alloc)
  {}

  /// \brief Construct a vector of size `size` using the default or provided allocator
  constexpr explicit Vector (SizeType size, const AllocatorType& alloc = AllocatorType())
    : size_(size)
    , alloc_(alloc)
    , data_(size_ > 0 ? alloc_.allocate(size_) : nullptr)
  {}

  /// \brief Construct a vector of size `size` with all element initialized to `value`
  constexpr Vector (SizeType size, ValueType value, const AllocatorType& alloc = AllocatorType())
    : size_(size)
    , alloc_(alloc)
    , data_(size_ > 0 ? alloc_.allocate(size_) : nullptr)
  {
    for (SizeType i = 0; i < size_; ++i)
      data_[i] = value;
  }

  /// \brief Construct the vector from an iterator range `[first, last)`
  template <class InputIt,
    decltype(*std::declval<const InputIt&>(), ++std::declval<InputIt&>(), bool{}) = true>
  constexpr Vector (InputIt first, InputIt last, const AllocatorType& alloc = AllocatorType())
    : Vector(SizeType(std::distance(first,last)), alloc)
  {
    for (SizeType i = 0; first != last; ++first, ++i)
      data_[i] = *first;
  }

  /// \brief Construct a copy of the vector `other`
  constexpr Vector (const Vector& other)
    : Vector(other, std::allocator_traits<Allocator>::select_on_container_copy_construction(other.alloc_))
  {}

  /// \brief Construct a copy of the vector `other`using a provided allocator
  constexpr Vector (const Vector& other, const AllocatorType& alloc)
    : Vector(other.size_, alloc)
  {
    for (SizeType i = 0; i < size_; ++i)
      data_[i] = other[i];
  }

  /// \brief Move construct the vector from `other` leaving `other` in an empty state
  constexpr Vector (Vector&& other) noexcept
    : Vector()
  {
    this->swap(other);
  }

  /// \brief Move construct the vector from using using a provided allocator by
  /// move assigning all element to the new allocated memory.
  constexpr Vector (Vector&& other, const AllocatorType& alloc)
    : Vector(other.size_, alloc)
  {
    for (SizeType i = 0; i < size_; ++i)
      data_[i] = std::move(other[i]);
  }

  /// \brief Construct the vector from the provided initializer list
  constexpr Vector (std::initializer_list<ElementType> init, const AllocatorType& alloc = AllocatorType())
    : Vector(init.begin(), init.end(), alloc)
  {}

  /// \brief The destructor frees all allocated memory
  ~Vector () noexcept
  {
    if (data_)
      alloc_.deallocate(data_, size_);
  }

  /// @}


  /// \name Assignment
  /// @{

  /// \brief Copy-assign `other` to this
  constexpr Vector& operator= (const Vector& other)
  {
    Vector that(other);
    this->swap(that);
    return *this;
  }

  /// \brief Move assign `other` to this.
  constexpr Vector& operator= (Vector&& other)
  {
    Vector that(std::move(other));
    this->swap(that);
    return *this;
  }

  /// \brief Replaces the contents with `count` copies of value `value`.
  constexpr void assign (SizeType count, const ElementType& value)
  {
    for (SizeType i = 0; i < std::min(count,size_); ++i)
      data_[i] = value;
  }

  /// \brief Replaces the contents with copies of those in the range `[first, last)`.
  template <class InputIt>
  constexpr void assign (InputIt first, InputIt last)
  {
    for (SizeType i = 0; i < size_ && first != last;  ++i, ++first)
      data_[i] = *first;
  }

  /// \brief Replaces the contents with the elements from the initializer list `ilist`.
  constexpr void assign (std::initializer_list<ElementType> ilist)
  {
    assign(ilist.begin(), ilist.end());
  }

  /// \brief Returns the associated allocator.
  constexpr AllocatorType get_allocator () const noexcept
  {
    return alloc_;
  }

  /// @}


  /// \name Element access
  /// @{

  /// \brief Access specified element at position `pos` with bounds checking
  constexpr Reference at (SizeType pos)
  {
    if (indexInRange(pos))
      return data_[pos];
    else
      throw std::out_of_range("Index out of range.");
  }

  /// \brief Access specified element at position `pos` with bounds checking
  constexpr ConstReference at (SizeType pos) const
  {
    if (indexInRange(pos))
      return data_[pos];
    else
      throw std::out_of_range("Index out of range.");
  }

  /// \brief Access specified element at position `pos`
  constexpr Reference operator[] (SizeType pos) noexcept
  {
    DUNE_ASSERT_MSG(indexInRange(pos), "Index out of range.");
    return data_[pos];
  }

  /// \brief Access specified element at position `pos`
  constexpr ConstReference operator[] (SizeType pos) const noexcept
  {
    DUNE_ASSERT_MSG(indexInRange(pos), "Index out of range.");
    return data_[pos];
  }

  /// \brief Access the first element
  constexpr Reference front () noexcept
  {
    return data_[0];
  }

  /// \brief Access the first element
  constexpr ConstReference front () const noexcept
  {
    return data_[0];
  }

  /// \brief Access the last element
  constexpr Reference back () noexcept
  {
    return data_[size_-1];
  }

  /// \brief Access the last element
  constexpr ConstReference back () const noexcept
  {
    return data_[size_-1];
  }

  /// \brief Direct access to the underlying contiguous storage
  constexpr ElementType* data () noexcept
  {
    return data_;
  }

  /// \brief Direct access to the underlying contiguous storage
  constexpr const ElementType* data () const noexcept
  {
    return data_;
  }

  /// @}


  /// \name Iterators
  /// @{

  /// \brief Returns an iterator to the beginning
  constexpr Iterator begin () noexcept
  {
    return data_;
  }

  /// \brief Returns an iterator to the beginning
  constexpr ConstIterator begin () const noexcept
  {
    return data_;
  }

  /// \brief Returns an iterator to the beginning
  constexpr ConstIterator cbegin () const noexcept
  {
    return data_;
  }

  /// \brief Returns an iterator to the end
  constexpr Iterator end () noexcept
  {
    return data_ + size_;
  }

  /// \brief Returns an iterator to the end
  constexpr ConstIterator end () const noexcept
  {
    return data_ + size_;
  }

  /// \brief Returns an iterator to the end
  constexpr ConstIterator cend () const noexcept
  {
    return data_ + size_;
  }

  /// @}


  /// \name Capacity
  /// @{

  /// \brief Checks whether the container is empty
  [[nodiscard]] constexpr bool empty () const noexcept
  {
    return size_ == 0 || data_ == nullptr;
  }

  /// \brief Returns the number of elements
  [[nodiscard]] constexpr SizeType size () const noexcept
  {
    return size_;
  }

  /// \brief Returns the maximum possible number of elements
  [[nodiscard]] constexpr SizeType max_size () const noexcept
  {
    return std::allocator_traits<AllocatorType>::max_size();
  }

  /// \brief Returns the number of elements that can be held in currently allocated storage.
  [[nodiscard]] constexpr SizeType capacity () const noexcept
  {
    return size_;
  }

  /// \brief Reserves storage
  constexpr void reserve (SizeType count)
  {
    /* do nothing */
  }

  /// @}


  /// \name Modifiers
  /// @{

  /// \brief Clears the contents
  constexpr void clear () noexcept
  {
    resize(0);
  }

  /// \brief Changes the number of elements stored. If `count != size` allocate new storage.
  constexpr void resize (SizeType count)
  {
    if (size_ != count) {
      Vector tmp(count);
      this->swap(tmp);
    }
  }

  /// \brief Changes the number of elements stored and set the value of all elements to `value`.
  /// If `count != size` allocate new storage with the given initial value.
  constexpr void resize (SizeType count, const ElementType& value)
  {
    if (size_ != count) {
      Vector tmp(count, value);
      this->swap(tmp);
    }
  }

  /// \brief Swaps the contents with `other`
  constexpr void swap (Vector& other)
  {
    using std::swap;
    swap(size_, other.size_);
    swap(alloc_, other.alloc_);
    swap(data_, other.data_);
  }

  /// @}

private:
  constexpr inline bool indexInRange (SizeType pos) const
  {
    if constexpr (std::is_signed_v<SizeType>)
      return 0 <= pos && pos < size_;
    else
      return pos < size_;
  }

private:
  SizeType size_ = 0;
  DUNE_NO_UNIQUE_ADDRESS AllocatorType alloc_ = {};
  ElementType* data_ = nullptr;
};


/// \name Comparison operations
/// \relates Vector
/// @{

/// \brief Check if the content of the two Vectors is equal
template <class Element, class Allocator>
constexpr bool operator== (const Dune::Vector<Element, Allocator>& lhs,
                           const Dune::Vector<Element, Allocator>& rhs)
{
  return (lhs.size() != rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

#if  __cpp_impl_three_way_comparison >= 201907L && __cpp_lib_three_way_comparison >= 201907L

/// \brief Lexicographically compares the values of two Vectors
template <class Element, class Allocator>
constexpr auto operator<=> (const Dune::Vector<Element, Allocator>& lhs,
                            const Dune::Vector<Element, Allocator>& rhs)
{
  return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

#else

/// \brief Check if the content of the two Vectors is unequal
template <class Element, class Allocator>
bool operator!= (const Dune::Vector<Element, Allocator>& lhs,
                 const Dune::Vector<Element, Allocator>& rhs)
{
  return !(lhs == rhs);
}

/// \brief Lexicographically compares the values of two Vectors by <
template <class Element, class Allocator>
bool operator< (const Dune::Vector<Element, Allocator>& lhs,
                const Dune::Vector<Element, Allocator>& rhs)
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::less<>{});
}

/// \brief Lexicographically compares the values of two Vectors by <=
template <class Element, class Allocator>
bool operator<= (const Dune::Vector<Element, Allocator>& lhs,
                 const Dune::Vector<Element, Allocator>& rhs)
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::less_equal<>{});
}

/// \brief Lexicographically compares the values of two Vectors by >
template <class Element, class Allocator>
bool operator> (const Dune::Vector<Element, Allocator>& lhs,
                const Dune::Vector<Element, Allocator>& rhs)
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::greater<>{});
}

/// \brief Lexicographically compares the values of two Vectors by >=
template <class Element, class Allocator>
bool operator>= (const Dune::Vector<Element, Allocator>& lhs,
                 const Dune::Vector<Element, Allocator>& rhs)
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::greater_equal<>{});
}

#endif

/// @}


template<class T>
struct FieldTraits< Dune::Vector<T> >
{
  typedef typename FieldTraits<T>::field_type field_type;
  typedef typename FieldTraits<T>::real_type real_type;
};

} // end namespace Dune

#endif // DUNE_COMMON_VECTOR_HH
