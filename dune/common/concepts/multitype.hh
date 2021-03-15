// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_CONCEPTS_MULTITYPE_HH
#define DUNE_CONCEPTS_MULTITYPE_HH

#if __has_include(<concepts>) || DOXYGEN
#include <type_traits>
#include <utility>

#include <dune/common/concepts/collection.hh>
#include <dune/common/std/type_traits.hh>

namespace Dune {
namespace Concept {

/**
 * \ingroup CxxConcepts
 * @{
 **/

template <class C>
concept MultiTypeCollection = requires
{
  typename C::size_type;
};

template <class C>
concept MultiTypeAlgebraicCollection = MultiTypeCollection<C>
  && requires
{
  std::integral_constant<typename C::size_type, C::N()>; // number of rows
  std::integral_constant<typename C::size_type, C::M()>; // number of columns
  std::integral_constant<typename C::size_type, C::size()>; // number of elements
};


namespace Impl
{
  template <class M, class S, S I, S J>
  using IndexAccessibleConstantMatrix = decltype(
    std::declval<const M&>()[std::integral_constant<S,I>{}][std::integral_constant<S,J>]
  );

  template <class M, class S, S I, S J>
  using MutableIndexAccessibleConstantMatrix = decltype(
    std::declval<M&>()[std::integral_constant<S,I>{}][std::integral_constant<S,J>]
  );

  template <class M, class RowSeq, class ColSeq,
            template <class,class,typename M::size_type,typename M::size_type> class Check>
  struct AllIndexAccessibleMat;

  template <class M, class S, S... rows, class ColSeq,
            template <class,class,typename M::size_type,typename M::size_type> class Check>
  struct AllIndexAccessibleMat<M, std::integer_sequence<S, rows...>, ColSeq, Check>
  {
    template <S row, class Seq> struct Helper;
    template <S row, S... cols> struct Helper<row, std::integer_sequence<S, cols...>>
      : std::conjunction<Dune::Std::is_detected<Check, M, S, row, cols>...> {};

    static constexpr bool value = (Helper<rows,ColSeq>::value &&...);
  };

} // end namespace Impl

template <class M>
concept MultiTypeMatrix = MultiTypeAlgebraicCollection<M> &&
  Impl::AllIndexAccessibleMat<M, std::make_integer_sequence<typename M::size_type, M::N()>,
                                 std::make_integer_sequence<typename M::size_type, M::M()>,
                              Impl::IndexAccessibleConstantMatrix>::value;

template <class M>
concept MutableMultiTypeMatrix = MultiTypeAlgebraicCollection<M> &&
  Impl::AllIndexAccessibleMat<M, std::make_integer_sequence<typename M::size_type, M::N()>,
                                 std::make_integer_sequence<typename M::size_type, M::M()>,
                              Impl::MutableIndexAccessibleConstantMatrix>::value;


namespace Impl
{
  template <class V, class S, S I>
  using IndexAccessibleConstantVector = decltype(
    std::declval<const V&>()[std::integral_constant<S,I>{}]
  );

  template <class V, class S, S I>
  using MutableIndexAccessibleConstantVector = decltype(
    std::declval<V&>()[std::integral_constant<S,I>{}]
  );

  template <class V, class Seq, template <class,class,typename V::size_type> class Check>
  struct AllIndexAccessibleVec;

  template <class V, class S, S... indices, template <class,class,typename V::size_type> class Check>
  struct AllIndexAccessibleVec<V, std::integer_sequence<S, indices...>, Check>
  {
    static constexpr bool value = (Dune::Std::is_detected_v<Check, M, S, indices> &&...);
  };

} // end namespace Impl

template <class V>
concept MultiTypeVector = MultiTypeAlgebraicCollection<V> &&
  Impl::AllIndexAccessibleVec<V, std::make_integer_sequence<typename V::size_type, V::size()>,
                              Impl::IndexAccessibleConstantVector>::value;

template <class V>
concept MutableMultiTypeVector = MultiTypeAlgebraicCollection<V> &&
  Impl::AllIndexAccessibleVec<V, std::make_integer_sequence<typename V::size_type, V::size()>,
                              Impl::MutableIndexAccessibleConstantVector>::value;

/** @} */

}} // end namespace Dune::Concept

#endif // __has_include(<concepts>)
#endif // DUNE_CONCEPTS_MULTITYPE_HH
