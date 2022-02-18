#ifndef DUNE_PYTHON_COMMON_DIMRANGE_HH
#define DUNE_PYTHON_COMMON_DIMRANGE_HH

#include <cstddef>

#include <tuple>
#include <type_traits>
#include <utility>

#include <dune/python/common/fmatrix.hh>
#include <dune/python/common/fvector.hh>

namespace Dune
{

  namespace Python
  {

    namespace detail
    {

      template< class T >
      inline static constexpr T sum () noexcept
      {
        return static_cast< T >( 0 );
      }

      template< class T >
      inline static constexpr T sum ( T a ) noexcept
      {
        return a;
      }

      template< class T, class... U >
      inline static constexpr T sum ( T a, T b, U... c ) noexcept
      {
        return sum( a+b, c... );
      }


      template< class T, class Enable = void >
      struct DimRange;

      template< class T >
      struct DimRange< T, std::enable_if_t< std::is_arithmetic< T >::value > >
        : public std::integral_constant< std::size_t, 1 >
      {};

      template< class... T >
      struct DimRange< std::tuple< T... >, void >
        : public std::integral_constant< std::size_t, sum< std::size_t >( DimRange< T >::value... ) >
      {};

      template< class K, int n >
      struct DimRange< FieldVector< K, n >, void >
        : public std::integral_constant< std::size_t, n >
      {};

      template< class K, int m, int n >
      struct DimRange< FieldMatrix< K, m, n >, void >
        : public std::integral_constant< std::size_t, m*n >
      {};

      template< class T >
      struct DimRange< T, std::enable_if_t< std::is_class< typename T::FiniteElement >::value > >
        : public DimRange< std::decay_t< decltype( std::declval< typename T::FiniteElement >().localBasis() ) > >
      {};

      template< class T >
      struct DimRange< T, std::enable_if_t< std::is_same< std::size_t, decltype( static_cast< std::size_t >( T::Traits::dimRange ) ) >::value > >
        : public std::integral_constant< std::size_t, static_cast< std::size_t >( T::Traits::dimRange ) >
      {};

    } // namespace detail



    // DimRange
    // --------

    template< class T >
    using DimRange = detail::DimRange< T >;

  } // namespace Python

} // namespace Dune

#endif // #ifndef DUNE_PYTHON_COMMON_DIMRANGE_HH
