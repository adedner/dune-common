#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <array>
#include <vector>

#include <dune/common/fvector.hh>
#include <dune/common/dynvector.hh>
#include <dune/common/concepts/vector.hh>
#include <dune/common/concepts/vectorspace.hh>
#include <dune/common/test/testsuite.hh>

int main(int argc, char** argv)
{
  Dune::TestSuite test;

  static_assert(Dune::Concept::Vector<Dune::FieldVector<double,2>>);
  static_assert(Dune::Concept::TraversableCollection<Dune::FieldVector<double,2>>);
  static_assert(Dune::Concept::VectorSpace<Dune::FieldVector<double,2>>);

  // missing: const_reference
  // static_assert(Dune::Concept::Vector<Dune::DynamicVector<double>>);
  // static_assert(Dune::Concept::TraversableCollection<Dune::DynamicVector<double>>);
  static_assert(Dune::Concept::VectorSpace<Dune::DynamicVector<double>>);

  static_assert(Dune::Concept::Vector<std::array<double,2>>);
  static_assert(Dune::Concept::Vector<std::vector<double>>);
  static_assert(Dune::Concept::ResizeableVector<std::vector<double>>);

  // missing: index() methon in iterators
  // static_assert(Dune::Concept::TraversableCollection<std::array<double,2>>);
  // static_assert(Dune::Concept::TraversableCollection<std::vector<double>>);
}