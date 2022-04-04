#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <fstream>

#include <dune/common/parametertree.hh>
#include <dune/common/parametertreeparser.hh>

using namespace Dune;

#define expect(A)                                                       \
    std::cout << "checking " #A << std::endl;                           \
    tests++;                                                            \
    try {                                                               \
        if (!(A)) {                                                     \
            std::cerr << "ERROR: failed expectation\n\t" #A << std::endl; \
            failures++;                                                 \
        }                                                               \
    }                                                                   \
    catch (Dune::Exception & e) {                                       \
        std::cerr << "ERROR: " << e.what() << std::endl;                \
        failures++;                                                     \
    }


int main(){

  std::ofstream pyfile;
  std::string filename = "readpythonparametertree.py";
  ParameterTree ptree;
  ParameterTreeParser::readPythonFile(filename, ptree,true,"foo");
  ptree.report();
  int failures = 0;
  int tests = 0;
  expect(ptree.get("str", "foo") == "test");
  expect(ptree.sub("subTree").get("b", -1) == 2);
  expect(ptree.get<double>("value") == 11.);
  expect(ptree.get<int>("value") == 11);
  expect(ptree.get<bool>("bool") == true);
  expect(ptree.get<bool>("bool") != false);

  // apparently ParameterTree does not allow to cast a boolean into an int
  // expect(ptree.get<int>("bool") != 0);

  // try to read different vector vector types from string, list, tuple and numpy.array
  using V = std::vector<double>;
  using A = std::array<float,3>;
  using FV = Dune::FieldVector<double,3>;

  // 'vectorStr': "1 127 2.5",
  {
      expect(ptree.get<V>("vectorStr")  == V({1., 127., 2.5}));
      expect(ptree.get<A>("vectorStr")  == A({1., 127., 2.5}));
      expect(ptree.get<FV>("vectorStr") == FV({1., 127., 2.5}));
  }

  // 'array': [2, 12, 5.2]
  {
      expect(ptree.get<V>("array")  == V({2,12,5.2}));
      expect(ptree.get<A>("array")  == A({2,12,5.2}));
      expect(ptree.get<FV>("array") == FV({2,12,5.2}));
  }

  // 'tuple': (3, 13, 6.2)
  {
      expect(ptree.get<V>("tuple")  == V({3, 13, 6.2}));
      expect(ptree.get<A>("tuple")  == A({3, 13, 6.2}));
      expect(ptree.get<FV>("tuple") == FV({3, 13, 6.2}));
  }

  // if we have numpy, we can also test reading numpy arrays
  if (ptree.get<bool>("haveNumpy"))
  {
      // 'numpy': np.array([2,4,6], dtype=np.float64)
      expect(ptree.get<V>("numpy")  == V({2,4,6}));
      expect(ptree.get<A>("numpy")  == A({2,4,6}));
      expect(ptree.get<FV>("numpy") == FV({2,4,6}));

      // 'numpyInt': np.array([3,5,7])
      expect(ptree.get<V>("numpyInt")  == V({3,5,7}));
      expect(ptree.get<A>("numpyInt")  == A({3,5,7}));
      expect(ptree.get<FV>("numpyInt") == FV({3,5,7}));
  }

  if (failures > 0)
      std::cerr << "=== ERROR: " << failures << " tests out of " << tests << " failed" << std::endl;
  return failures;
}
