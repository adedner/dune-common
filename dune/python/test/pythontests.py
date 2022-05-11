from io import StringIO
from numpy import array
from concurrent.futures import ThreadPoolExecutor

import os
# enable DUNE_SAVE_BUILD to test various output options
os.environ['DUNE_LOG_LEVEL'] = 'info'
os.environ['DUNE_SAVE_BUILD'] = 'console'

classACode="""
struct MyClassA {
  MyClassA(int a,int b) : a_(a), b_(b) {}
  double b() { return b_; }
  int a_,b_;
};
"""
classBCode="""
#include <iostream>
#include <cmath>
#include <numeric>
#include <dune/python/common/numpyvector.hh>
template <class T> struct MyClassB {
  // the following fails to work correctly
  // MyClassB(T &t, int p, const Dune::Python::NumPyVector<double> &b)
  // - with 'const NumPyVector<double> b_;':  call to deleter ctor
  // - with 'const NumPyVector<double> &b_;': segfaults (this is expected
  //        since array_t->NumPyArray in the __init__ function is explicit and
  //        leads to dangling reference)
  // The following has to be used:
  MyClassB(T &t, int p, pybind11::array_t< double >& b)
  : a_(std::pow(t.a_,p)), b_(b) {}
  double b() { return std::accumulate(b_.begin(),b_.end(),0.); }
  int a_;
  const Dune::Python::NumPyVector<double> b_;
};
"""
runCode="""
#include <iostream>
template <class T> int run(T &t) {
  return t.a_ * t.b();
}
"""

runVec="""
#include <dune/python/common/numpyvector.hh>
template <class T>
void run(pybind11::array_t< T >& a)
{
  Dune::Python::NumPyVector< T > x( a );
  for( size_t i=0; i<x.size(); ++i )
    x[ i ] += i;
}
"""

def test_numpyvector():
    """
    Test correct exchange of numpy arrays to C++ side.
    """
    from dune.generator.algorithm import run
    x = array([0.]*100)
    run("run",StringIO(runVec),x)
    for i in range(len(x)):
        assert x[i] == float(i)

addVec="""
#include <dune/python/common/numpyvector.hh>
template <class T>
void add(pybind11::array_t< T >& a, pybind11::array_t< T >& b, pybind11::array_t< T >& c)
{
  Dune::Python::NumPyVector< T > x( a );
  Dune::Python::NumPyVector< T > y( b );
  Dune::Python::NumPyVector< T > ret( c );
  for( size_t i=0; i<x.size(); ++i )
    ret[ i ] = x[i] + y[i];
}
"""
dotVec="""
#include <dune/python/common/numpyvector.hh>
template <class T>
double dot(pybind11::array_t< T >& a, pybind11::array_t< T >& b)
{
  double ret = 0;
  Dune::Python::NumPyVector< T > x( a );
  Dune::Python::NumPyVector< T > y( b );
  for( size_t i=0; i<x.size(); ++i )
    ret += x[i]*y[i];
  return ret;
}
"""

def test_threaded():
    """
    Test building multiple modules using threading
    """
    from dune.generator.algorithm import load
    from dune.generator.threadedgenerator import ThreadedGenerator
    x = array([1.]*100)
    y = array([-1.]*100)
    z = array([2.]*100)

    """
    # build two algorthms in parallel
    generator = ThreadedGenerator(maxWorkers=2)
    generator.add( load, "add", StringIO(addVec), x,y,z)
    generator.add( load, "dot", StringIO(dotVec), x,y)
    add,dot = generator.execute()
    """

    with ThreadPoolExecutor(max_workers=2) as executor:
        addFuture = executor.submit( load, "add", StringIO(addVec), x,y,z)
        dotFuture = executor.submit( load, "dot", StringIO(dotVec), x,y)
    while not all([addFuture.done(), dotFuture.done()]): sleep(2)
    add,dot = addFuture.result(), dotFuture.result()

    add(x,y,z)
    for i in range(len(z)):
        assert z[i] == float(0)
    a = dot(x,y)
    assert a == -len(z)

def test_class_export():
    from dune.generator.importclass import load
    from dune.generator.algorithm   import run
    from dune.typeregistry import generateTypeName
    a = 2.
    x = array([2.]*10)
    cls = load("MyClassA",StringIO(classACode),10,20)
    assert run("run",StringIO(runCode),cls) == 10*20
    clsName, _ = generateTypeName("MyClassB",cls)
    cls = load(clsName,StringIO(classBCode),cls,2,x)
    assert run("run",StringIO(runCode),cls) == 10**2*10*a
    x[:] = array([3.]*10)[:]
    assert run("run",StringIO(runCode),cls) == 10**2*10*3
    # the following does not work
    x = array([4.]*10)
    # the 'B' class still keeps the old vector 'x' alive
    assert run("run",StringIO(runCode),cls) == 10**2*10*3

if __name__ == "__main__":
    from dune.packagemetadata import getDunePyDir
    _ = getDunePyDir()
    test_class_export()
    test_numpyvector()
    test_threaded()
