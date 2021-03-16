#if HAVE_CONFIG_H
#include <config.h>
#endif

#if __has_include(<concepts>)

#include <array>
#include <vector>

#include <dune/common/fvector.hh>
#include <dune/common/tuplevector.hh>
#include <dune/common/concepts/multitype.hh>

int main(int argc, char** argv)
{
  using Vec = Dune::TupleVector<Dune::FieldVector<double,2>, Dune::FieldVector<double,3>>;
  // static_assert(Dune::Concept::MultiTypeVector<Vec>); // missing size_type
}

#else
int main()
{
  return 77;
}
#endif
