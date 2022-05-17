from io import StringIO
from time import time
from dune.generator.importclass import load
from dune.common import FieldVector

codeA = """
#ifndef STRUCT_A
#define STRUCT_A
struct MyClassA {
  MyClassA(int& a) : a_(a) {}
  MyClassA(const MyClassA& a) : a_(a.a()) {}
  const int& a() const { return a_; }
  int a_;
};
#endif
"""

codeB = """
struct MyClassB {
  MyClassB(double& b) : b_(b) {}
  const double& b() { return b_; }
  double b_;
};
"""

start = time()
A = load("MyClassA", StringIO(codeA), 42)
B = load("MyClassB", StringIO(codeB), 42.0)
C = load("MyClassA", StringIO(codeA), A)
v = FieldVector([0])
print(A)
print(B)
print(C)
print(v)
print(f"Took {time()-start:.2f}")
