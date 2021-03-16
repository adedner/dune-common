#if HAVE_CONFIG_H
#include <config.h>
#endif

#if __has_include(<concepts>)

#include <array>
#include <vector>

#include <dune/common/arraylist.hh>
#include <dune/common/fvector.hh>
#include <dune/common/dynvector.hh>
#include <dune/common/reservedvector.hh>
#include <dune/common/sllist.hh>
#include <dune/common/concepts/vector.hh>
#include <dune/common/concepts/vectorspace.hh>

int main(int argc, char** argv)
{
  static_assert(Dune::Concept::Vector<Dune::FieldVector<double,2>>);
  static_assert(Dune::Concept::TraversableCollection<Dune::FieldVector<double,2>>);
  static_assert(Dune::Concept::VectorSpace<Dune::FieldVector<double,2>>);
  static_assert(Dune::Concept::BanachSpace<Dune::FieldVector<double,2>>);
  static_assert(Dune::Concept::HilbertSpace<Dune::FieldVector<double,2>>);

  static_assert(Dune::Concept::Vector<Dune::DynamicVector<double>>);
  static_assert(Dune::Concept::TraversableCollection<Dune::DynamicVector<double>>);
  static_assert(Dune::Concept::VectorSpace<Dune::DynamicVector<double>>);

  static_assert(Dune::Concept::Vector<std::array<double,2>>);
  static_assert(Dune::Concept::Vector<std::vector<double>>);
  static_assert(Dune::Concept::ResizeableVector<std::vector<double>>);

  // missing: index() methon in iterators
  // static_assert(Dune::Concept::TraversableCollection<std::array<double,2>>);
  // static_assert(Dune::Concept::TraversableCollection<std::vector<double>>);

  static_assert(Dune::Concept::Vector<Dune::ArrayList<double>>);
  // static_assert(Dune::Concept::Vector<Dune::SLList<double>>); // missing value_type
  static_assert(Dune::Concept::Vector<Dune::ReservedVector<double,2>>);
}

#else
int main()
{
  return 77;
}
#endif
