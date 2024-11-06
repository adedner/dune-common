#include <dune/common/parallel/mpihelper.hh>

#include <dune/common/fvector.hh>
#include <dune/common/parametertree.hh>

int main() {
  Dune::MPIHelper::instance();

  Dune::FieldVector<double,2> lower_left(0.);
  Dune::FieldVector<double,2> upper_right(-1.);

  Dune::ParameterTree ptree;
  ptree["foo"] = "bar";

  return 0;
}
