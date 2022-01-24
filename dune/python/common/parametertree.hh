// -*- tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_PYTHON_COMMON_PARAMETERTREE_HH
#define DUNE_PYTHON_COMMON_PARAMETERTREE_HH

#include <config.h>

#include <dune/common/parametertree.hh>
#include <dune/common/parametertreeparser.hh>
#include <dune/python/pybind11/pybind11.h>


namespace pybind11 { namespace detail {
    template <> struct type_caster<Dune::ParameterTree> {
    public:
      /**
       * This macro establishes the name 'ParameterTree' in
       * function signatures and declares a local variable
       * 'value' of type ParameterTree
       */
      PYBIND11_TYPE_CASTER(Dune::ParameterTree, _("ParameterTree"));

      /**
       * Conversion part 1 (Python->C++): convert a PyObject into a ParameterTree
       * instance or return false upon failure. The second argument
       * indicates whether implicit conversions should be applied.
       */
      bool load(handle src, bool) {
        /* Extract PyObject from handle */
        /* Try converting into a Python integer value */
        if(isinstance<dict>(src)){
          try{
            Dune::ParameterTreeParser::readPythonDict(src.cast<dict>(), value, true);
          }catch(Dune::Exception e){
            return false;
          }
          return true;
        }else
          return false;
      }

      /**
       * Conversion part 2 (C++ -> Python): convert an ParameterTree instance into
       * a Python object. The second and third arguments are used to
       * indicate the return value policy and parent object (for
       * ``return_value_policy::reference_internal``) and are generally
       * ignored by implicit casters.
       */
      static handle cast(Dune::ParameterTree src, return_value_policy /* policy */, handle /* parent */) {
        DUNE_THROW(Dune::NotImplemented, "The conversion of ParameterTree to python object is not implemented yet.");
      }
    };
  }} // namespace pybind11::detail

#endif // #ifndef DUNE_PYTHON_COMMON_MPIHELPER_HH
