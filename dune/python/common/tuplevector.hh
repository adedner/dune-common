// -*- tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

#ifndef DUNE_PYTHON_COMMON_TUPLEVECTOR_HH
#define DUNE_PYTHON_COMMON_TUPLEVECTOR_HH

#include <string>
#include <tuple>
#include <utility>

#include <dune/common/indices.hh>
#include <dune/common/tuplevector.hh>

#include <dune/python/common/fvector.hh>
#include <dune/python/common/typeregistry.hh>
#include <dune/python/pybind11/pybind11.h>
#include <dune/python/pybind11/operators.h>

namespace Dune
{

  namespace Python
  {
    template <class... K>
    std::string to_string (const Dune::TupleVector<K...>& v)
    {
      using namespace Dune::Indices;
      using std::to_string;
      using Dune::Python::to_string;
      std::string repr = "(";
      if constexpr(sizeof...(K) > 0)
        repr += to_string(v[_0]);
      if constexpr(sizeof...(K) > 1) {
        using IR = Dune::StaticIntegralRange<std::size_t,sizeof...(K),1>;
        Dune::unpackIntegerSequence([&](auto... i) {
          ((repr += ", " + to_string(v[i])),...);
        }, IR::to_integer_sequence());
      }
      repr += ")";
      return repr;
    }


    template< class... K >
    void registerTupleVector ( pybind11::handle scope )
    {
      using namespace Dune::Indices;
      using pybind11::operator""_a;

      typedef Dune::TupleVector< K... > TV;

      auto cls = insertClass< TV >( scope, "TupleVector",
          GenerateTypeName("Dune::TupleVector",MetaType<K>()...),
          IncludeFiles{"dune/common/tuplevector.hh"} ).first;

      // default constructor
      cls.def( pybind11::init( []{ return new TV(); } ) );

      // constructor from a "list" of values
      cls.def( pybind11::init( []( pybind11::args x ) {
          assert(sizeof...(K) == x.size());
          return Dune::unpackIntegerSequence([&](auto... i) {
            return new TV((x[i].template cast<K>())...);
          }, std::make_index_sequence<sizeof...(K)>{});
        } ) );
      cls.def( pybind11::init( []( pybind11::list x ) {
          assert(sizeof...(K) == x.size());
          return Dune::unpackIntegerSequence([&](auto... i) {
            return new TV((x[i].template cast<K>())...);
          }, std::make_index_sequence<sizeof...(K)>{});
        } ), "x"_a );
      cls.def( pybind11::init( []( pybind11::tuple x ) {
          assert(sizeof...(K) == x.size());
          return Dune::unpackIntegerSequence([&](auto... i) {
            return new TV((x[i].template cast<K>())...);
          }, std::make_index_sequence<sizeof...(K)>{});
        } ), "x"_a );

      // numer of elements in the tuple
      cls.def("__len__", [](const TV &v) { return v.size();  });

      // element access
      Hybrid::forEach(std::make_index_sequence<sizeof...(K)>{},[&](auto i)
      {
        using Ki = std::decay_t<decltype(std::declval<TV>()[i])>;
        cls.def("__getitem__", [i](const TV& v, decltype(i)) { return v[i]; });
        cls.def("__setitem__", [i](TV& v, decltype(i), const Ki& x) { v[i] = x; });
      });

      // check whether two tuples are equal
      cls.def("__eq__",
          [](const TV& self, const TV& other) {
            const std::tuple<K...>& self_tuple(self);
            const std::tuple<K...>& other_tuple(other);
            return self == other;
          });

      // return a string representation
      cls.def("__str__",
          [](const TV& v) { return to_string(v); });

      // return a string representation
      cls.def("__repr__",
          [](const TV& v) { return "Dune::TupleVector" + to_string(v); });
    }

  } // namespace Python

} // namespace Dune

#endif // #ifndef DUNE_PYTHON_COMMON_TUPLEVECTOR_HH
