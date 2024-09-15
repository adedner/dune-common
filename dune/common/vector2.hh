// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_VECTOR_HH
#define DUNE_COMMON_VECTOR_HH

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

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
    : public std::vector<Element,Allocator>
{
  using BaseType = std::vector<Element,Allocator>;

public:
  using BaseType::BaseType;
};

template <class Allocator>
class Vector<bool, Allocator>
    : public std::vector<std::byte, typename std::allocator_traits<Allocator>::template rebind_alloc<std::byte>>
{
  using AllocatorType = typename std::allocator_traits<Allocator>::template rebind_alloc<std::byte>;
  using BaseType = std::vector<std::byte, AllocatorType>;

public:
  using allocator_type = AllocatorType;
  using size_type = typename BaseType::size_type;
  using difference_type = typename BaseType::difference_type;
  using value_type = bool;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;

  /// \name Constructors and Destructor
  /// @{

  using BaseType::BaseType;

  /// \brief Construct a vector of size `size` with all element initialized to `value`
  constexpr Vector (size_type size, value_type value, const AllocatorType& alloc = AllocatorType())
    : BaseType(size, std::byte{value}, alloc)
  {}

  /// \brief Construct the vector from the provided initializer list
  constexpr Vector (std::initializer_list<value_type> init, const AllocatorType& alloc = AllocatorType())
    : BaseType(init.begin(), init.end(), alloc)
  {}

  /// @}


  /// \name Assignment
  /// @{

  using BaseType::assign;

  /// \brief Replaces the contents with `count` copies of value `value`.
  constexpr void assign (size_type count, const value_type& value)
  {
    BaseType::assign(count, std::byte{value});
  }

  /// \brief Replaces the contents with the elements from the initializer list `ilist`.
  constexpr void assign (std::initializer_list<value_type> ilist)
  {
    BaseType::assign(ilist.begin(), ilist.end());
  }

  /// @}


  /// \name Element access
  /// @{

  /// \brief Access specified element at position `pos` with bounds checking
  constexpr reference at (size_type pos)
  {
    return reinterpret_cast<reference>(BaseType::at(pos));
  }

  /// \brief Access specified element at position `pos` with bounds checking
  constexpr const_reference at (size_type pos) const
  {
    return reinterpret_cast<const_reference>(BaseType::at(pos));
  }

  /// \brief Access specified element at position `pos`
  constexpr reference operator[] (size_type pos) noexcept
  {
    return reinterpret_cast<reference>(BaseType::operator[](pos));
  }

  /// \brief Access specified element at position `pos`
  constexpr const_reference operator[] (size_type pos) const noexcept
  {
    return reinterpret_cast<const_reference>(BaseType::operator[](pos));
  }

  /// \brief Access the first element
  constexpr reference front () noexcept
  {
    return reinterpret_cast<reference>(BaseType::front());
  }

  /// \brief Access the first element
  constexpr const_reference front () const noexcept
  {
    return reinterpret_cast<const_reference>(BaseType::front());
  }

  /// \brief Access the last element
  constexpr reference back () noexcept
  {
    return reinterpret_cast<reference>(BaseType::back());
  }

  /// \brief Access the last element
  constexpr const_reference back () const noexcept
  {
    return reinterpret_cast<const_reference>(BaseType::back());
  }

  /// \brief Direct access to the underlying contiguous storage
  constexpr pointer data () noexcept
  {
    return reinterpret_cast<pointer>(BaseType::data());
  }

  /// \brief Direct access to the underlying contiguous storage
  constexpr const_pointer data () const noexcept
  {
    return reinterpret_cast<const_pointer>(BaseType::data());
  }

  /// @}


  /// \name Modifiers
  /// @{

  using BaseType::resize;

  /// \brief Changes the number of elements stored and set the value of all elements to `value`.
  /// If `count != size` allocate new storage with the given initial value.
  constexpr void resize (size_type count, const value_type& value)
  {
    BaseType::resize(count, std::byte{value});
  }

  /// @}
};

template<class T>
struct FieldTraits< Dune::Vector<T> >
{
  typedef typename FieldTraits<T>::field_type field_type;
  typedef typename FieldTraits<T>::real_type real_type;
};

} // end namespace Dune

#endif // DUNE_COMMON_VECTOR_HH
