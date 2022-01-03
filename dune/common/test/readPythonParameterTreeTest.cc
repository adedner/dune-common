#include <iostream>
#include <fstream>

#include <dune/common/parametertree.hh>
#include <dune/common/parametertreeparser.hh>

using namespace Dune;

int main(){

  std::ofstream pyfile;
  std::string filename = "parametertree.py";
  pyfile.open(filename);
  pyfile << "test = 'test'" << std::endl
         << "test_dict = {'a': 1, 'b':2}" << std::endl;
  pyfile.close();
  ParameterTree ptree;
  ParameterTreeParser::readPythonTree(filename, ptree);
  ptree.report();
  assert(ptree.get("test", "foo") == "test");
  assert(ptree.sub("test_dict").get("b", -1) == 2);
  return 0;
}
