// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_VECTOR_HH
#define DUNE_COMMON_VECTOR_HH

#include <cstddef>
#include <initializer_list>
#include <limits>
#include <memory>
#include <stdexcept>
#include <type_traits>

namespace Dune {

template <class Element, class Allocator = std::allocator<Element>>
class Vector 
{
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

public:

  /// \name Constructors and Destructor
  /// @{

  constexpr Vector () noexcept( noexcept(AllocatorType()) )
  {}

  constexpr explicit Vector (const AllocatorType& alloc) noexcept
    : alloc_(alloc)
  {}

  constexpr explicit Vector (SizeType size, const AllocatorType& alloc = AllocatorType())
    : size_(size)
    , alloc_(alloc)
    , data_(size_ > 0 ? alloc_.allocate(size_) : nullptr)
  {}

  constexpr Vector (SizeType size, ValueType value, const AllocatorType& alloc = AllocatorType())
    : size_(size)
    , alloc_(alloc)
    , data_(size_ > 0 ? alloc_.allocate(size_) : nullptr)
  {
    for (SizeType i = 0; i < size_; ++i)
      data_[i] = value;
  }

  template <class InputIt,
    decltype(*std::declval<InputIt>(), ++std::declval<InputIt>(), bool{}) = true>
  constexpr Vector (InputIt first, InputIt last, const AllocatorType& alloc = AllocatorType())
    : Vector(SizeType(std::distance(first,last)), alloc)
  {
    for (SizeType i = 0; first != last; ++first, ++i)
      data_[i] = *first;
  }

  constexpr Vector (const Vector& other)
    : Vector(other, other.alloc_)
  {}

  constexpr Vector (const Vector& other, const AllocatorType& alloc)
    : Vector(other.size_, alloc)
  {
    for (SizeType i = 0; i < size_; ++i)
      data_[i] = other[i];
  }

  constexpr Vector (Vector&& other) noexcept
    : Vector()
  {
    this->swap(other);
  }

  constexpr Vector (Vector&& other, const AllocatorType& alloc)
    : Vector(other.size_, alloc)
  {
    for (SizeType i = 0; i < size_; ++i)
      data_[i] = std::move(other[i]);
  }

  constexpr Vector (std::initializer_list<ElementType> init, const AllocatorType& alloc = AllocatorType())
    : Vector(init.begin(), init.end(), alloc)
  {}

  ~Vector () noexcept
  {
    if (data_)
      alloc_.deallocate(data_, size_);
  }

  /// @}


  /// \name Assignment
  /// @{

  constexpr Vector& operator= (const Vector& other)
  {
    Vector that(other);
    this->swap(that);
    return *this;
  }

  constexpr Vector& operator= (Vector&& other)
  {
    Vector that(std::move(other));
    this->swap(that);
    return *this;
  }

  constexpr void swap (Vector& other)
  {
    using std::swap;
    swap(size_, other.size_);
    swap(alloc_, other.alloc_);
    swap(data_, other.data_);
  }

  constexpr void assign (SizeType count, const ElementType& value)
  {
    Vector tmp(count, value);
    this->swap(tmp);
  }

  template <class InputIt>
  constexpr void assign (InputIt first, InputIt last)
  {
    Vector tmp(first, last);
    this->swap(tmp);
  }

  constexpr void assign (std::initializer_list<ElementType> ilist)
  {
    Vector tmp(std::move(ilist));
    this->swap(tmp);
  }

  constexpr AllocatorType get_allocator () const noexcept
  {
    return alloc_;
  }

  /// @}


  /// \name Element access 
  /// @{

  constexpr Reference at (SizeType pos)
  {
    if (0 <= pos && pos < size_)
      return data_[pos];
    else 
      throw std::out_of_range("Index out of range.");
  }

  constexpr ConstReference at (SizeType pos) const
  {
    if (0 <= pos && pos < size_)
      return data_[pos];
    else 
      throw std::out_of_range("Index out of range.");
  }

  constexpr Reference operator[] (SizeType pos) noexcept
  {
    return data_[pos];
  }

  constexpr ConstReference operator[] (SizeType pos) const noexcept
  {
    return data_[pos];
  }

  constexpr Reference front (SizeType pos) noexcept
  {
    return data_[0];
  }

  constexpr ConstReference front (SizeType pos) const noexcept
  {
    return data_[0];
  }

  constexpr Reference back (SizeType pos) noexcept
  {
    return data_[size_-1];
  }

  constexpr ConstReference back (SizeType pos) const noexcept
  {
    return data_[size_-1];
  }

  constexpr ElementType* data () noexcept
  {
    return data_;
  }

  constexpr const ElementType* data () const noexcept
  {
    return data_;
  }

  /// @}


  /// \name Iterators
  /// @{

  constexpr Iterator begin () noexcept 
  {
    return data_;
  }
  
  constexpr ConstIterator begin () const noexcept
  {
    return data_;
  }

  constexpr ConstIterator cbegin () const noexcept
  {
    return data_;
  }

  constexpr Iterator end () noexcept 
  {
    return data_ + size_;
  }
  
  constexpr ConstIterator end () const noexcept
  {
    return data_ + size_;
  }

  constexpr ConstIterator cend () const noexcept
  {
    return data_ + size_;
  }

  /// @}


  /// \name Capacity
  /// @{

  [[nodiscard]] constexpr bool empty () const noexcept 
  {
    return size_ == 0 || data_ == nullptr;
  }

  [[nodiscard]] constexpr SizeType size () const noexcept 
  {
    return size_;
  }

  [[nodiscard]] constexpr SizeType max_size () const noexcept 
  {
    return std::numeric_limits<DifferenceType>::max();
  }

  constexpr void reserve (SizeType new_cap) noexcept
  {
    /* do nothing */
  }

  [[nodiscard]] constexpr SizeType capacity () const noexcept
  {
    return size_;
  }

  constexpr void shrink_to_fit () noexcept
  {
    /* do nothing */
  }

  /// @}

private:
  SizeType size_ = 0;
  AllocatorType alloc_ = {};
  ElementType* data_ = nullptr;
};

} // end namespace Dune

#endif // DUNE_COMMON_VECTOR_HH
