// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#include <config.h>
#include <string>
#define USING_DUNE_PYTHON 1
#include <dune/python/common/fvector.hh>

PYBIND11_MODULE( MODNAME, module )
{
  using pybind11::operator""_a;
  pybind11::module cls0 = module;
  {
    using DuneType = Dune::FieldVector< double, DIM >;
    std::string typeName = "Dune::FieldVector< double," + std::to_string(int(DIM)) + " >";
    auto cls = Dune::Python::insertClass< DuneType >( cls0, "FieldVector",pybind11::buffer_protocol(),
        Dune::Python::GenerateTypeName(typeName), Dune::Python::IncludeFiles{"dune/python/common/fvector.hh"}).first;
    Dune::Python::registerFieldVector( cls0, cls );
  }
}
