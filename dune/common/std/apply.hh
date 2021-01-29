// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_COMMON_STD_APPLY_HH
#define DUNE_COMMON_STD_APPLY_HH
#include <dune/internal/dune-common.hh>

#if DUNE_HAVE_CXX_APPLY
  #include <tuple>
#elif DUNE_HAVE_CXX_EXPERIMENTAL_APPLY
  #include <experimental/tuple>
#else
  #include <cstddef>
  #include <utility>
  #include <tuple>
#endif

#include <dune/common/tupleutility.hh>


namespace Dune
{

  namespace Std
  {

#if DUNE_HAVE_CXX_APPLY

    using std::apply;

#elif DUNE_HAVE_CXX_EXPERIMENTAL_APPLY

    using std::experimental::apply;

#else

    /**
     * \brief Apply function with arguments given as tuple
     *
     * \param f A callable object
     * \param args Tuple of arguments
     *
     * This will call the function with arguments generated by unpacking the tuple.
     *
     * \ingroup CxxUtilities
     */
    template<class F, class ArgTuple>
    decltype(auto) apply(F&& f, ArgTuple&& args)
    {
      auto indices = std::make_index_sequence<std::tuple_size<std::decay_t<ArgTuple>>::value>();
      return applyPartial(std::forward<F>(f), std::forward<ArgTuple>(args), indices);
    }

#endif

  } // namespace Std
} // namespace Dune

#endif // #ifndef DUNE_COMMON_STD_APPLY_HH
