/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#define __EXPAND(x)                                      x
#define __COUNT(_1, _2, _3, _4, _5, _6, _7, COUNT, ...)  COUNT
#define __VA_SIZE(...)                                   __EXPAND(__COUNT(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1))
#define __CAT1(a, b)                                     a ## b
#define __CAT2(a, b)                                     __CAT1(a, b)
#define __DOC1(n1)                                       __doc_##n1
#define __DOC2(n1, n2)                                   __doc_##n1##_##n2
#define __DOC3(n1, n2, n3)                               __doc_##n1##_##n2##_##n3
#define __DOC4(n1, n2, n3, n4)                           __doc_##n1##_##n2##_##n3##_##n4
#define __DOC5(n1, n2, n3, n4, n5)                       __doc_##n1##_##n2##_##n3##_##n4##_##n5
#define __DOC6(n1, n2, n3, n4, n5, n6)                   __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define __DOC7(n1, n2, n3, n4, n5, n6, n7)               __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define DOC(...)                                         __EXPAND(__EXPAND(__CAT2(__DOC, __VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *__doc_Dune_DenseIterator =
R"doc(! Generic iterator class for dense vector and matrix implementations

provides sequential access to DenseVector, FieldVector and FieldMatrix)doc";

static const char *__doc_Dune_DenseIterator_DenseIterator = R"doc()doc";

static const char *__doc_Dune_DenseIterator_DenseIterator_2 = R"doc()doc";

static const char *__doc_Dune_DenseIterator_DenseIterator_3 = R"doc()doc";

static const char *__doc_Dune_DenseIterator_DenseIterator_4 = R"doc()doc";

static const char *__doc_Dune_DenseIterator_advance = R"doc()doc";

static const char *__doc_Dune_DenseIterator_container = R"doc()doc";

static const char *__doc_Dune_DenseIterator_decrement = R"doc()doc";

static const char *__doc_Dune_DenseIterator_dereference = R"doc()doc";

static const char *__doc_Dune_DenseIterator_distanceTo = R"doc()doc";

static const char *__doc_Dune_DenseIterator_distanceTo_2 = R"doc()doc";

static const char *__doc_Dune_DenseIterator_elementAt = R"doc()doc";

static const char *__doc_Dune_DenseIterator_equals = R"doc()doc";

static const char *__doc_Dune_DenseIterator_equals_2 = R"doc()doc";

static const char *__doc_Dune_DenseIterator_increment = R"doc()doc";

static const char *__doc_Dune_DenseIterator_index = R"doc(return index)doc";

static const char *__doc_Dune_DenseIterator_position = R"doc()doc";

static const char *__doc_Dune_DenseMatrix =
R"doc(A dense n x m matrix.

Matrices represent linear maps from a vector space V to a vector space
W. This class represents such a linear map by storing a two-
dimensional %array of numbers of a given field type K. The number of
rows and columns is given at compile time.

Template parameter ``MAT``:
    type of the matrix implementation)doc";

static const char *__doc_Dune_DenseMatrix_2 =
R"doc(A dense n x m matrix.

Matrices represent linear maps from a vector space V to a vector space
W. This class represents such a linear map by storing a two-
dimensional %array of numbers of a given field type K. The number of
rows and columns is given at compile time.

Template parameter ``MAT``:
    type of the matrix implementation)doc";

static const char *__doc_Dune_DenseMatrixAssigner =
R"doc(you have to specialize this structure for any type that should be
assignable to a DenseMatrix

Template parameter ``DenseMatrix``:
    Some type implementing the dense matrix interface

Template parameter ``RHS``:
    Right hand side type)doc";

static const char *__doc_Dune_DenseMatrixAssigner_2 =
R"doc(you have to specialize this structure for any type that should be
assignable to a DenseMatrix

Template parameter ``DenseMatrix``:
    Some type implementing the dense matrix interface

Template parameter ``RHS``:
    Right hand side type)doc";

static const char *__doc_Dune_DenseMatrixHelp_multAssign = R"doc(calculates ret = matrix * x)doc";

static const char *__doc_Dune_DenseMatrix_Elim = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_ElimDet = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_ElimDet_ElimDet = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_ElimDet_operator_call = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_ElimDet_sign = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_ElimDet_swap = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_ElimPivot = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_ElimPivot_ElimPivot = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_ElimPivot_operator_call = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_ElimPivot_pivot = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_ElimPivot_swap = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_Elim_Elim = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_Elim_operator_call = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_Elim_rhs = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_Elim_swap = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_M = R"doc(number of columns)doc";

static const char *__doc_Dune_DenseMatrix_N = R"doc(number of rows)doc";

static const char *__doc_Dune_DenseMatrix_asImp = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_asImp_2 = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_axpy = R"doc(vector space axpy operation (*this += a x))doc";

static const char *__doc_Dune_DenseMatrix_beforeBegin =
R"doc(Returns:
    an iterator that is positioned before the first entry of the
    vector.)doc";

static const char *__doc_Dune_DenseMatrix_beforeBegin_2 =
R"doc(Returns:
    an iterator that is positioned before the first entry of the
    vector.)doc";

static const char *__doc_Dune_DenseMatrix_beforeEnd =
R"doc(Returns:
    an iterator that is positioned before the end iterator of the
    vector, i.e. at the last entry.)doc";

static const char *__doc_Dune_DenseMatrix_beforeEnd_2 =
R"doc(Returns:
    an iterator that is positioned before the end iterator of the
    vector. i.e. at the last element)doc";

static const char *__doc_Dune_DenseMatrix_begin = R"doc(begin iterator)doc";

static const char *__doc_Dune_DenseMatrix_begin_2 = R"doc(begin iterator)doc";

static const char *__doc_Dune_DenseMatrix_blocklevel = R"doc(The number of block levels we contain. This is 1.)doc";

static const char *__doc_Dune_DenseMatrix_cols = R"doc(number of columns)doc";

static const char *__doc_Dune_DenseMatrix_determinant = R"doc(calculates the determinant of this matrix)doc";

static const char *__doc_Dune_DenseMatrix_end = R"doc(end iterator)doc";

static const char *__doc_Dune_DenseMatrix_end_2 = R"doc(end iterator)doc";

static const char *__doc_Dune_DenseMatrix_exists = R"doc(return true when (i,j) is in pattern)doc";

static const char *__doc_Dune_DenseMatrix_frobenius_norm = R"doc(frobenius norm: sqrt(sum over squared values of entries))doc";

static const char *__doc_Dune_DenseMatrix_frobenius_norm2 = R"doc(square of frobenius norm, need for block recursion)doc";

static const char *__doc_Dune_DenseMatrix_infinity_norm = R"doc(infinity norm (row sum norm, how to generalize for blocks?))doc";

static const char *__doc_Dune_DenseMatrix_infinity_norm_2 = R"doc(infinity norm (row sum norm, how to generalize for blocks?))doc";

static const char *__doc_Dune_DenseMatrix_infinity_norm_real = R"doc(simplified infinity norm (uses Manhattan norm for complex values))doc";

static const char *__doc_Dune_DenseMatrix_infinity_norm_real_2 = R"doc(simplified infinity norm (uses Manhattan norm for complex values))doc";

static const char *__doc_Dune_DenseMatrix_invert =
R"doc(Compute inverse

\exception FMatrixError if the matrix is singular)doc";

static const char *__doc_Dune_DenseMatrix_leftmultiply = R"doc(Multiplies M from the left to this matrix)doc";

static const char *__doc_Dune_DenseMatrix_luDecomposition =
R"doc(do an LU-Decomposition on matrix A

Parameter ``A``:
    The matrix to decompose, and to store the result in.

Parameter ``func``:
    Functor used for swapping lanes and to conduct the elimination.
    Depending on the functor, ``luDecomposition``() can be used for
    solving, for inverting, or to compute the determinant.

Parameter ``nonsingularLanes``:
    SimdMask of lanes that are nonsingular.

Parameter ``throwEarly``:
    Whether to throw an ``FMatrixError`` immediately as soon as one
    lane is discovered to be singular. If ``False``, do not throw,
    instead continue until finished or all lanes are singular, and
    exit via return in both cases.

Parameter ``doPivoting``:
    Enable pivoting.

There are two modes of operation:

* Terminate as soon as one lane is discovered to be singular. Early
termination is done by throwing an ``FMatrixError``. On entry,
``Simd::allTrue``(nonsingularLanes) and ``throwEarly``==true should
hold. After early termination, the contents of ``A`` should be
considered bogus, and ``nonsingularLanes`` has the lane(s) that
triggered the early termination unset. There may be more singular
lanes than the one reported in ``nonsingularLanes``, which just havent
been discovered yet; so the value of ``nonsingularLanes`` is mostly
useful for diagnostics.

* Terminate only when all lanes are discovered to be singular. Use
this when you want to apply special postprocessing in singular lines
(e.g. setting the determinant of singular lanes to 0 in
``determinant``()). On entry, ``nonsingularLanes`` may have any value
and ``throwEarly``==false should hold. The function will not throw an
exception if some lanes are discovered to be singular, instead it will
continue running until all lanes are singular or until finished, and
terminate only via normal return. On exit, ``nonsingularLanes``
contains the map of lanes that are valid in ``A``.)doc";

static const char *__doc_Dune_DenseMatrix_mmhv = R"doc(y -= A^H x)doc";

static const char *__doc_Dune_DenseMatrix_mmtv = R"doc(y -= A^T x)doc";

static const char *__doc_Dune_DenseMatrix_mmv = R"doc(y -= A x)doc";

static const char *__doc_Dune_DenseMatrix_mtv = R"doc(y = A^T x)doc";

static const char *__doc_Dune_DenseMatrix_mv = R"doc(y = A x)doc";

static const char *__doc_Dune_DenseMatrix_operator_array = R"doc(random access)doc";

static const char *__doc_Dune_DenseMatrix_operator_array_2 = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_operator_assign = R"doc()doc";

static const char *__doc_Dune_DenseMatrix_operator_eq = R"doc(Binary matrix comparison)doc";

static const char *__doc_Dune_DenseMatrix_operator_iadd = R"doc(vector space addition)doc";

static const char *__doc_Dune_DenseMatrix_operator_idiv = R"doc(vector space division by scalar)doc";

static const char *__doc_Dune_DenseMatrix_operator_imul = R"doc(vector space multiplication with scalar)doc";

static const char *__doc_Dune_DenseMatrix_operator_isub = R"doc(vector space subtraction)doc";

static const char *__doc_Dune_DenseMatrix_operator_ne = R"doc(Binary matrix incomparison)doc";

static const char *__doc_Dune_DenseMatrix_operator_sub = R"doc(Matrix negation)doc";

static const char *__doc_Dune_DenseMatrix_rightmultiply = R"doc(Multiplies M from the right to this matrix)doc";

static const char *__doc_Dune_DenseMatrix_rows = R"doc(number of rows)doc";

static const char *__doc_Dune_DenseMatrix_size = R"doc(size method (number of rows))doc";

static const char *__doc_Dune_DenseMatrix_solve =
R"doc(Solve system A x = b

\exception FMatrixError if the matrix is singular)doc";

static const char *__doc_Dune_DenseMatrix_umhv = R"doc(y += A^H x)doc";

static const char *__doc_Dune_DenseMatrix_umtv = R"doc(y += A^T x)doc";

static const char *__doc_Dune_DenseMatrix_umv = R"doc(y += A x)doc";

static const char *__doc_Dune_DenseMatrix_usmhv = R"doc(y += alpha A^H x)doc";

static const char *__doc_Dune_DenseMatrix_usmtv = R"doc(y += alpha A^T x)doc";

static const char *__doc_Dune_DenseMatrix_usmv = R"doc(y += alpha A x)doc";

static const char *__doc_Dune_DenseVector =
R"doc(Interface for a class of dense vectors over a given field.

Template parameter ``V``:
    implementation class of the vector)doc";

static const char *__doc_Dune_DenseVector_2 =
R"doc(Interface for a class of dense vectors over a given field.

Template parameter ``V``:
    implementation class of the vector)doc";

static const char *__doc_Dune_DenseVector_DenseVector = R"doc()doc";

static const char *__doc_Dune_DenseVector_DenseVector_2 = R"doc()doc";

static const char *__doc_Dune_DenseVector_N = R"doc(number of blocks in the vector (are of size 1 here))doc";

static const char *__doc_Dune_DenseVector_asImp = R"doc()doc";

static const char *__doc_Dune_DenseVector_asImp_2 = R"doc()doc";

static const char *__doc_Dune_DenseVector_axpy = R"doc(vector space axpy operation ( *this += a x ))doc";

static const char *__doc_Dune_DenseVector_back = R"doc(return reference to last element)doc";

static const char *__doc_Dune_DenseVector_back_2 = R"doc(return reference to last element)doc";

static const char *__doc_Dune_DenseVector_beforeBegin =
R"doc(Returns:
    an iterator that is positioned before the first entry of the
    vector.)doc";

static const char *__doc_Dune_DenseVector_beforeBegin_2 =
R"doc(Returns:
    an iterator that is positioned before the first entry of the
    vector.)doc";

static const char *__doc_Dune_DenseVector_beforeEnd =
R"doc(Returns:
    an iterator that is positioned before the end iterator of the
    vector, i.e. at the last entry.)doc";

static const char *__doc_Dune_DenseVector_beforeEnd_2 =
R"doc(Returns:
    an iterator that is positioned before the end iterator of the
    vector. i.e. at the last element)doc";

static const char *__doc_Dune_DenseVector_begin = R"doc(begin iterator)doc";

static const char *__doc_Dune_DenseVector_begin_2 = R"doc(begin ConstIterator)doc";

static const char *__doc_Dune_DenseVector_blocklevel = R"doc(The number of block levels we contain)doc";

static const char *__doc_Dune_DenseVector_dim = R"doc(dimension of the vector space)doc";

static const char *__doc_Dune_DenseVector_dot =
R"doc(vector dot product :math:`\left (x^H \cdot y \right)` which
corresponds to Petsc's VecDot

http://www.mcs.anl.gov/petsc/petsc-
current/docs/manualpages/Vec/VecDot.html

Parameter ``x``:
    other vector

Returns:)doc";

static const char *__doc_Dune_DenseVector_empty = R"doc(checks whether the container is empty)doc";

static const char *__doc_Dune_DenseVector_end = R"doc(end iterator)doc";

static const char *__doc_Dune_DenseVector_end_2 = R"doc(end ConstIterator)doc";

static const char *__doc_Dune_DenseVector_find = R"doc(return iterator to given element or end())doc";

static const char *__doc_Dune_DenseVector_find_2 = R"doc(return iterator to given element or end())doc";

static const char *__doc_Dune_DenseVector_front = R"doc(return reference to first element)doc";

static const char *__doc_Dune_DenseVector_front_2 = R"doc(return reference to first element)doc";

static const char *__doc_Dune_DenseVector_infinity_norm = R"doc(infinity norm (maximum of absolute values of entries))doc";

static const char *__doc_Dune_DenseVector_infinity_norm_2 = R"doc(infinity norm (maximum of absolute values of entries))doc";

static const char *__doc_Dune_DenseVector_infinity_norm_real = R"doc(simplified infinity norm (uses Manhattan norm for complex values))doc";

static const char *__doc_Dune_DenseVector_infinity_norm_real_2 = R"doc(simplified infinity norm (uses Manhattan norm for complex values))doc";

static const char *__doc_Dune_DenseVector_one_norm = R"doc(one norm (sum over absolute values of entries))doc";

static const char *__doc_Dune_DenseVector_one_norm_real = R"doc(simplified one norm (uses Manhattan norm for complex values))doc";

static const char *__doc_Dune_DenseVector_operator_add = R"doc(Binary vector addition)doc";

static const char *__doc_Dune_DenseVector_operator_array = R"doc(random access)doc";

static const char *__doc_Dune_DenseVector_operator_array_2 = R"doc()doc";

static const char *__doc_Dune_DenseVector_operator_assign = R"doc(Assignment operator for scalar)doc";

static const char *__doc_Dune_DenseVector_operator_assign_2 = R"doc(Assignment operator for other DenseVector of same type)doc";

static const char *__doc_Dune_DenseVector_operator_assign_3 = R"doc(Assignment operator for other DenseVector of different type)doc";

static const char *__doc_Dune_DenseVector_operator_eq = R"doc(Binary vector comparison)doc";

static const char *__doc_Dune_DenseVector_operator_iadd = R"doc(vector space addition)doc";

static const char *__doc_Dune_DenseVector_operator_iadd_2 =
R"doc(vector space add scalar to all comps

we use enable_if to avoid an ambiguity, if the function parameter can
be converted to value_type implicitly. (see FS#1457)

The function is only enabled, if the parameter is directly convertible
to value_type.)doc";

static const char *__doc_Dune_DenseVector_operator_idiv =
R"doc(vector space division by scalar

we use enable_if to avoid an ambiguity, if the function parameter can
be converted to field_type implicitly. (see FS#1457)

The function is only enabled, if the parameter is directly convertible
to field_type.)doc";

static const char *__doc_Dune_DenseVector_operator_imul =
R"doc(vector space multiplication with scalar

we use enable_if to avoid an ambiguity, if the function parameter can
be converted to field_type implicitly. (see FS#1457)

The function is only enabled, if the parameter is directly convertible
to field_type.)doc";

static const char *__doc_Dune_DenseVector_operator_isub = R"doc(vector space subtraction)doc";

static const char *__doc_Dune_DenseVector_operator_isub_2 =
R"doc(vector space subtract scalar from all comps

we use enable_if to avoid an ambiguity, if the function parameter can
be converted to value_type implicitly. (see FS#1457)

The function is only enabled, if the parameter is directly convertible
to value_type.)doc";

static const char *__doc_Dune_DenseVector_operator_mul =
R"doc(indefinite vector dot product :math:`\left (x^T \cdot y \right)` which
corresponds to Petsc's VecTDot

http://www.mcs.anl.gov/petsc/petsc-
current/docs/manualpages/Vec/VecTDot.html

Parameter ``x``:
    other vector

Returns:)doc";

static const char *__doc_Dune_DenseVector_operator_ne = R"doc(Binary vector incomparison)doc";

static const char *__doc_Dune_DenseVector_operator_sub = R"doc(Binary vector subtraction)doc";

static const char *__doc_Dune_DenseVector_operator_sub_2 = R"doc(Vector negation)doc";

static const char *__doc_Dune_DenseVector_size = R"doc(size method)doc";

static const char *__doc_Dune_DenseVector_two_norm = R"doc(two norm sqrt(sum over squared values of entries))doc";

static const char *__doc_Dune_DenseVector_two_norm2 =
R"doc(square of two norm (sum over squared values of entries), need for
block recursion)doc";

static const char *__doc_Dune_Elim = R"doc()doc";

static const char *__doc_Dune_ElimPivot = R"doc()doc";

static const char *__doc_Dune_FMatrixError = R"doc(Error thrown if operations of a FieldMatrix fail. */)doc";

static const char *__doc_Dune_FMatrixHelp_invertMatrix = R"doc(invert scalar without changing the original matrix)doc";

static const char *__doc_Dune_FMatrixHelp_invertMatrix_2 = R"doc(invert 2x2 Matrix without changing the original matrix)doc";

static const char *__doc_Dune_FMatrixHelp_invertMatrix_3 = R"doc(invert 3x3 Matrix without changing the original matrix)doc";

static const char *__doc_Dune_FMatrixHelp_invertMatrix_retTransposed = R"doc(invert scalar without changing the original matrix)doc";

static const char *__doc_Dune_FMatrixHelp_invertMatrix_retTransposed_2 =
R"doc(invert 2x2 Matrix without changing the original matrix return
transposed matrix)doc";

static const char *__doc_Dune_FMatrixHelp_invertMatrix_retTransposed_3 = R"doc(invert 3x3 Matrix without changing the original matrix)doc";

static const char *__doc_Dune_FMatrixHelp_mult = R"doc(calculates ret = matrix * x)doc";

static const char *__doc_Dune_FMatrixHelp_multAssignTransposed = R"doc(calculates ret = matrix^T * x)doc";

static const char *__doc_Dune_FMatrixHelp_multMatrix = R"doc(calculates ret = A * B)doc";

static const char *__doc_Dune_FMatrixHelp_multTransposed = R"doc(calculates ret = matrix^T * x)doc";

static const char *__doc_Dune_FMatrixHelp_multTransposedMatrix = R"doc(calculates ret= A_t*A)doc";

static const char *__doc_Dune_FieldMatrix =
R"doc(work around a problem of FieldMatrix/FieldVector, there is no unique
way to obtain the size of a class

.. deprecated::
    VectorSize is deprecated; please call the 'size()' method directly
    instead. This will be removed after Dune 2.8.)doc";

static const char *__doc_Dune_FieldMatrix_2 =
R"doc(! \file

Implements a matrix constructed from a given type representing a field
and compile-time given number of rows and columns.)doc";

static const char *__doc_Dune_FieldMatrix_3 =
R"doc(! \file

Implements a matrix constructed from a given type representing a field
and compile-time given number of rows and columns.)doc";

static const char *__doc_Dune_FieldMatrix_FieldMatrix = R"doc(Default constructor)doc";

static const char *__doc_Dune_FieldMatrix_FieldMatrix_2 = R"doc(Constructor initializing the matrix from a list of vector)doc";

static const char *__doc_Dune_FieldMatrix_FieldMatrix_3 = R"doc()doc";

static const char *__doc_Dune_FieldMatrix_cols = R"doc(The number of columns.)doc";

static const char *__doc_Dune_FieldMatrix_data = R"doc()doc";

static const char *__doc_Dune_FieldMatrix_leftmultiplyany = R"doc(Multiplies M from the left to this matrix, this matrix is not modified)doc";

static const char *__doc_Dune_FieldMatrix_mat_access = R"doc()doc";

static const char *__doc_Dune_FieldMatrix_mat_access_2 = R"doc()doc";

static const char *__doc_Dune_FieldMatrix_mat_cols = R"doc()doc";

static const char *__doc_Dune_FieldMatrix_mat_rows = R"doc()doc";

static const char *__doc_Dune_FieldMatrix_operator_assign = R"doc(copy assignment operator)doc";

static const char *__doc_Dune_FieldMatrix_operator_assign_2 = R"doc(copy assignment from FieldMatrix over a different field)doc";

static const char *__doc_Dune_FieldMatrix_operator_assign_3 = R"doc(no copy assignment from FieldMatrix of different size)doc";

static const char *__doc_Dune_FieldMatrix_rightmultiply = R"doc(Multiplies M from the right to this matrix)doc";

static const char *__doc_Dune_FieldMatrix_rightmultiplyany =
R"doc(Multiplies M from the right to this matrix, this matrix is not
modified)doc";

static const char *__doc_Dune_FieldMatrix_rows = R"doc(The number of rows.)doc";

static const char *__doc_Dune_FieldVector =
R"doc(vector space out of a tensor product of fields.

Template parameter ``K``:
    the field type (use float, double, complex, etc)

Template parameter ``SIZE``:
    number of components.)doc";

static const char *__doc_Dune_FieldVector_2 =
R"doc(! \file Implements a vector constructed from a given type representing
a field and a compile-time given size.)doc";

static const char *__doc_Dune_FieldVector_3 =
R"doc(! \file Implements a vector constructed from a given type representing
a field and a compile-time given size.)doc";

static const char *__doc_Dune_FieldVector_FieldVector = R"doc(Constructor making default-initialized vector)doc";

static const char *__doc_Dune_FieldVector_FieldVector_2 = R"doc(Constructor making vector with identical coordinates)doc";

static const char *__doc_Dune_FieldVector_FieldVector_3 = R"doc(Copy constructor)doc";

static const char *__doc_Dune_FieldVector_FieldVector_4 = R"doc(Construct from a std::initializer_list */)doc";

static const char *__doc_Dune_FieldVector_FieldVector_5 =
R"doc(Copy constructor from a second vector of possibly different type

If the DenseVector type of the this constructor's argument is
implemented by a FieldVector, it is statically checked if it has the
correct size. If this is not the case the constructor is removed from
the overload set using SFINAE.

Parameter ``x``:
    A DenseVector with correct size.

Parameter ``dummy``:
    A void* dummy argument needed by SFINAE.)doc";

static const char *__doc_Dune_FieldVector_FieldVector_6 = R"doc(Constructor making vector with identical coordinates)doc";

static const char *__doc_Dune_FieldVector_FieldVector_7 = R"doc()doc";

static const char *__doc_Dune_FieldVector_data = R"doc()doc";

static const char *__doc_Dune_FieldVector_data_2 = R"doc(return pointer to underlying array)doc";

static const char *__doc_Dune_FieldVector_data_3 = R"doc(return pointer to underlying array)doc";

static const char *__doc_Dune_FieldVector_dimension = R"doc(The size of this vector.)doc";

static const char *__doc_Dune_FieldVector_operator_array = R"doc()doc";

static const char *__doc_Dune_FieldVector_operator_array_2 = R"doc()doc";

static const char *__doc_Dune_FieldVector_operator_assign = R"doc(copy assignment operator)doc";

static const char *__doc_Dune_FieldVector_operator_assign_2 = R"doc()doc";

static const char *__doc_Dune_FieldVector_operator_assign_3 = R"doc()doc";

static const char *__doc_Dune_FieldVector_size = R"doc()doc";

static const char *__doc_Dune_HasDenseMatrixAssigner = R"doc()doc";

static const char *__doc_Dune_Impl_DenseMatrixAssigner = R"doc()doc";

static const char *__doc_Dune_Impl_hasDenseMatrixAssigner = R"doc()doc";

static const char *__doc_Dune_Impl_hasDenseMatrixAssigner_2 = R"doc()doc";

static const char *__doc_Dune_IsFieldVectorSizeCorrect =
R"doc(TMP to check the size of a DenseVectors statically, if possible.

If the implementation type of C is a FieldVector, we statically check
whether its dimension is SIZE.

Template parameter ``C``:
    The implementation of the other DenseVector

Template parameter ``SIZE``:
    The size we need assume.)doc";

static const char *__doc_Dune_IsFieldVectorSizeCorrect_value =
R"doc(Parameter ``True``:
    if C is not of type FieldVector or its dimension is not equal
    SIZE.)doc";

static const char *__doc_Dune_MathOverloads_isFinite = R"doc()doc";

static const char *__doc_Dune_MathOverloads_isInf = R"doc()doc";

static const char *__doc_Dune_MathOverloads_isNaN = R"doc()doc";

static const char *__doc_Dune_MathOverloads_isUnordered = R"doc()doc";

static const char *__doc_Dune_VectorSize = R"doc()doc";

static const char *__doc_Dune_VectorSize_size = R"doc()doc";

static const char *__doc_Dune_determinant = R"doc(calculates the determinant of this matrix)doc";

static const char *__doc_Dune_fvmeta_Sqrt = R"doc(\private \memberof Dune::DenseVector)doc";

static const char *__doc_Dune_fvmeta_Sqrt_sqrt = R"doc()doc";

static const char *__doc_Dune_fvmeta_abs2 = R"doc(\private \memberof Dune::DenseVector)doc";

static const char *__doc_Dune_fvmeta_abs2_2 = R"doc(\private \memberof Dune::DenseVector)doc";

static const char *__doc_Dune_fvmeta_absreal = R"doc(\private \memberof Dune::DenseVector)doc";

static const char *__doc_Dune_fvmeta_absreal_2 = R"doc(\private \memberof Dune::DenseVector)doc";

static const char *__doc_Dune_fvmeta_sqrt = R"doc(\private \memberof Dune::DenseVector)doc";

static const char *__doc_Dune_invert =
R"doc(Compute inverse

\exception FMatrixError if the matrix is singular)doc";

static const char *__doc_Dune_luDecomposition =
R"doc(do an LU-Decomposition on matrix A

Parameter ``A``:
    The matrix to decompose, and to store the result in.

Parameter ``func``:
    Functor used for swapping lanes and to conduct the elimination.
    Depending on the functor, ``luDecomposition``() can be used for
    solving, for inverting, or to compute the determinant.

Parameter ``nonsingularLanes``:
    SimdMask of lanes that are nonsingular.

Parameter ``throwEarly``:
    Whether to throw an ``FMatrixError`` immediately as soon as one
    lane is discovered to be singular. If ``False``, do not throw,
    instead continue until finished or all lanes are singular, and
    exit via return in both cases.

Parameter ``doPivoting``:
    Enable pivoting.

There are two modes of operation:

* Terminate as soon as one lane is discovered to be singular. Early
termination is done by throwing an ``FMatrixError``. On entry,
``Simd::allTrue``(nonsingularLanes) and ``throwEarly``==true should
hold. After early termination, the contents of ``A`` should be
considered bogus, and ``nonsingularLanes`` has the lane(s) that
triggered the early termination unset. There may be more singular
lanes than the one reported in ``nonsingularLanes``, which just havent
been discovered yet; so the value of ``nonsingularLanes`` is mostly
useful for diagnostics.

* Terminate only when all lanes are discovered to be singular. Use
this when you want to apply special postprocessing in singular lines
(e.g. setting the determinant of singular lanes to 0 in
``determinant``()). On entry, ``nonsingularLanes`` may have any value
and ``throwEarly``==false should hold. The function will not throw an
exception if some lanes are discovered to be singular, instead it will
continue running until all lanes are singular or until finished, and
terminate only via normal return. On exit, ``nonsingularLanes``
contains the map of lanes that are valid in ``A``.)doc";

static const char *__doc_Dune_operator_add = R"doc(Binary addition, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_add_2 = R"doc(Binary addition, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_call = R"doc()doc";

static const char *__doc_Dune_operator_div = R"doc(Binary division, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_div_2 = R"doc(Binary division, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_eq = R"doc(Binary compare, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_eq_2 = R"doc(Binary compare, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_ge = R"doc(Binary compare, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_ge_2 = R"doc(Binary compare, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_ge_3 = R"doc(Binary compare, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_gt = R"doc(Binary compare, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_gt_2 = R"doc(Binary compare, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_gt_3 = R"doc(Binary compare, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_le = R"doc(Binary compare, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_le_2 = R"doc(Binary compare, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_le_3 = R"doc(Binary compare, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_lshift = R"doc(Sends the matrix to an output stream */)doc";

static const char *__doc_Dune_operator_lshift_2 =
R"doc(Write a DenseVector to an output stream \relates DenseVector

Parameter ``s``:
    std :: ostream to write to

Parameter ``v``:
    DenseVector to write

Returns:
    the output stream (s))doc";

static const char *__doc_Dune_operator_lshift_3 = R"doc(Sends the matrix to an output stream */)doc";

static const char *__doc_Dune_operator_lt = R"doc(Binary compare, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_lt_2 = R"doc(Binary compare, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_lt_3 = R"doc(Binary compare, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_mul = R"doc(Binary multiplication, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_mul_2 = R"doc(Binary multiplication, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_ne = R"doc(Binary compare, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_ne_2 = R"doc(Binary compare, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_rshift =
R"doc(Read a FieldVector from an input stream \relates FieldVector

\note This operator is STL compliant, i.e., the content of v is only
changed if the read operation is successful.

Parameter ``in``:
    std :: istream to read from

Parameter ``v``:
    FieldVector to be read

Returns:
    the input stream (in))doc";

static const char *__doc_Dune_operator_sub = R"doc(Binary subtraction, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_operator_sub_2 = R"doc(Binary subtraction, when using FieldVector<K,1> like K)doc";

static const char *__doc_Dune_solve =
R"doc(Solve system A x = b

\exception FMatrixError if the matrix is singular)doc";

static const char *__doc_Dune_swap = R"doc()doc";

static const char *__doc_Dune_swap_2 = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif
