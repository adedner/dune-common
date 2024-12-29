// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#ifndef DUNE_COMMON_VECTOR_HH
#define DUNE_COMMON_VECTOR_HH

#include <initializer_list>
#include <memory>
#include <vector>

#include <dune/common/ftraits.hh>
#include <dune/common/rangeutilities.hh>

namespace Dune {

/**
 * \brief Vector is a sequence container that encapsulates dynamic size arrays.
 *
 * This sequence container derives from `std::vector`, except for the element type
 * `bool`. There, a `bool`-wrapper type is used as element type to workaround the
 * `std::vector<bool>` specialization issue. Note, in general you should prefer
 * the `std::vector` container directly. Only if you are aware of the `bool`
 * issue and it matters in your application, this derived type `Dune::Vector`
 * could be a replacement. It is mainly used as internal data structure in other
 * Dune containers.
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


namespace Impl {

// Wrapper type around a bool
struct alignas(bool) BoolType
{
  bool value_ = false;

  constexpr BoolType operator= (bool b) noexcept { value_ = b; return *this; }
  constexpr operator bool& () noexcept { return value_; }
  constexpr operator const bool& () const noexcept { return value_; }
};

} // end namespace Impl


template <class Allocator>
class Vector<bool, Allocator>
    : public std::vector<Impl::BoolType, typename std::allocator_traits<Allocator>::template rebind_alloc<Impl::BoolType>>
{
  using AllocatorType = typename std::allocator_traits<Allocator>::template rebind_alloc<Impl::BoolType>;
  using BaseType = std::vector<Impl::BoolType, AllocatorType>;

  static_assert(sizeof(bool) == sizeof(Impl::BoolType));

public:
  using allocator_type = AllocatorType;
  using size_type = typename BaseType::size_type;
  using difference_type = typename BaseType::difference_type;
  using value_type = bool;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;

private:
  struct BoolTypeToBool
  {
    reference operator() (Impl::BoolType& value) const
    {
      return value;
    }

    const_reference operator() (const Impl::BoolType& value) const
    {
      return value;
    }
  };

public:
  using iterator = Impl::TransformedRangeIterator<typename BaseType::iterator, BoolTypeToBool, ValueTransformationTag>;
  using const_iterator = Impl::TransformedRangeIterator<typename BaseType::const_iterator, BoolTypeToBool, ValueTransformationTag>;
  using reverse_iterator = Impl::TransformedRangeIterator<typename BaseType::reverse_iterator, BoolTypeToBool, ValueTransformationTag>;
  using const_reverse_iterator = Impl::TransformedRangeIterator<typename BaseType::const_reverse_iterator, BoolTypeToBool, ValueTransformationTag>;

public:
  /// \name Constructors and Destructor
  /// @{

  using BaseType::BaseType;

  /// \brief Construct a vector of size `size` with all element initialized to `value`
  constexpr Vector (size_type size, value_type value, const AllocatorType& alloc = AllocatorType())
    : BaseType(size, Impl::BoolType{value}, alloc)
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
    BaseType::assign(count, Impl::BoolType{value});
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
    return BaseType::at(pos);
  }

  /// \brief Access specified element at position `pos` with bounds checking
  constexpr const_reference at (size_type pos) const
  {
    return BaseType::at(pos);
  }

  /// \brief Access specified element at position `pos`
  constexpr reference operator[] (size_type pos) noexcept
  {
    return BaseType::operator[](pos);
  }

  /// \brief Access specified element at position `pos`
  constexpr const_reference operator[] (size_type pos) const noexcept
  {
    return BaseType::operator[](pos);
  }

  /// \brief Access the first element
  constexpr reference front () noexcept
  {
    return BaseType::front();
  }

  /// \brief Access the first element
  constexpr const_reference front () const noexcept
  {
    return BaseType::front();
  }

  /// \brief Access the last element
  constexpr reference back () noexcept
  {
    return BaseType::back();
  }

  /// \brief Access the last element
  constexpr const_reference back () const noexcept
  {
    return BaseType::back();
  }

  /// \brief Direct access to the underlying contiguous storage
  constexpr pointer data () noexcept
  {
    return &reference(*BaseType::data());
  }

  /// \brief Direct access to the underlying contiguous storage
  constexpr const_pointer data () const noexcept
  {
    return &const_reference(*BaseType::data());
  }

  /// @}


  /// \name Iterators
  /// @{

  /// \brief Returns an iterator to the beginning
  constexpr iterator begin () noexcept
  {
    return iterator{BaseType::begin(), BoolTypeToBool{}};
  }

  /// \brief Returns an iterator to the beginning
  constexpr const_iterator begin () const noexcept
  {
    return const_iterator{BaseType::begin(), BoolTypeToBool{}};
  }

  /// \brief Returns an iterator to the beginning
  constexpr const_iterator cbegin () const noexcept
  {
    return const_iterator{BaseType::cbegin(), BoolTypeToBool{}};
  }

  /// \brief Returns an iterator to the end
  constexpr iterator end () noexcept
  {
    return iterator{BaseType::end(), BoolTypeToBool{}};
  }

  /// \brief Returns an iterator to the end
  constexpr const_iterator end () const noexcept
  {
    return const_iterator{BaseType::end(), BoolTypeToBool{}};
  }

  /// \brief Returns an iterator to the end
  constexpr const_iterator cend () const noexcept
  {
    return const_iterator{BaseType::cend(), BoolTypeToBool{}};
  }

  /// \brief Returns an iterator to the beginning
  constexpr reverse_iterator rbegin () noexcept
  {
    return reverse_iterator{BaseType::rbegin(), BoolTypeToBool{}};
  }

  /// \brief Returns an iterator to the beginning
  constexpr const_reverse_iterator rbegin () const noexcept
  {
    return const_reverse_iterator{BaseType::rbegin(), BoolTypeToBool{}};
  }

  /// \brief Returns an iterator to the beginning
  constexpr const_reverse_iterator crbegin () const noexcept
  {
    return const_reverse_iterator{BaseType::crbegin(), BoolTypeToBool{}};
  }

  /// \brief Returns an iterator to the end
  constexpr reverse_iterator rend () noexcept
  {
    return reverse_iterator{BaseType::rend(), BoolTypeToBool{}};
  }

  /// \brief Returns an iterator to the end
  constexpr const_reverse_iterator rend () const noexcept
  {
    return const_reverse_iterator{BaseType::rend(), BoolTypeToBool{}};
  }

  /// \brief Returns an iterator to the end
  constexpr const_reverse_iterator crend () const noexcept
  {
    return const_reverse_iterator{BaseType::crend(), BoolTypeToBool{}};
  }

  /// @}


  /// \name Modifiers
  /// @{

  using BaseType::resize;

  /// \brief Changes the number of elements stored
  constexpr void resize (size_type count, const value_type& value)
  {
    BaseType::resize(count, Impl::BoolType{value});
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
