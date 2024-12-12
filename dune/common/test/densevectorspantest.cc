// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
// SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception
#include <array>
#include <iostream>
#include <vector>

#include <dune/common/dynmatrix.hh>
#include <dune/common/densevectorspan.hh>
#include <dune/common/test/testsuite.hh>

int main()
{
  Dune::TestSuite test;

  Dune::DynamicMatrix<double> matrix(
  {
    { 2.0, -1.0, -1.0},
    {-1.0,  3.0, -1.0},
    {-1.0, -1.0,  4.0}
  });

  std::vector<double>  x(3);
  std::array<double,3> y{1.0,2.0,3.0};

  auto x_span = Dune::DenseVectorSpan(x);
  auto y_span = Dune::DenseVectorSpan(y);

  matrix.mv(y_span,x_span);
  test.check(x[0] == -3.0);
  test.check(x[1] ==  2.0);
  test.check(x[2] ==  9.0);

  return test.exit();
}
