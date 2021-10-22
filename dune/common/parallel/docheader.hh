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


static const char *__doc_Dune_Communication =
R"doc(! Collective communication interface and sequential default
implementation

Communication offers an abstraction to the basic methods of parallel
communication, following the message-passing paradigm. It allows one
to switch parallel features on and off, without changing the code.
Currently only MPI and sequential code are supported.

A Communication object is returned by all grids (also the sequential
ones) in order to allow code to be written in a transparent way for
sequential and parallel grids.

This class provides a default implementation for sequential grids. The
number of processes involved is 1, any sum, maximum, etc. returns just
its input argument and so on.

In specializations one can implement the real thing using appropriate
communication functions, e.g. there exists an implementation using the
Message Passing %Interface (MPI), see Dune::Communication<MPI_Comm>.

Moreover, the communication subsystem used by an implementation is not
visible in the interface, i.e. Dune grid implementations are not
restricted to MPI.

Template parameter ``Communicator``:
    The communicator type used by your message-passing implementation.
    For MPI this will be MPI_Comm. For sequential codes there is the
    dummy communicator No_Comm. It is assumed that if you want to
    specialize the Communication class for a message-passing system
    other than MPI, that message-passing system will have something
    equivalent to MPI communicators.)doc";

static const char *__doc_Dune_Communication_Communication = R"doc(Construct default object)doc";

static const char *__doc_Dune_Communication_Communication_2 =
R"doc(Constructor with a given communicator

As this is implementation for the sequential setting, the communicator
is a dummy and simply discarded.)doc";

static const char *__doc_Dune_Communication_allgather =
R"doc(Gathers data from all tasks and distribute it to all.

The block of data sent from the jth process is received by every
process and placed in the jth block of the buffer recvbuf.

Parameter ``sbuf``:
    The buffer with the data to send. Has to be the same for each
    task.

Parameter ``count``:
    The number of elements to send by any process.

Parameter ``rbuf``:
    The receive buffer for the data. Has to be of size notasks*count,
    with notasks being the number of tasks in the communicator.

Returns:
    MPI_SUCCESS (==0) if successful, an MPI error code otherwise)doc";

static const char *__doc_Dune_Communication_allgatherv =
R"doc(Gathers data of variable length from all tasks and distribute it to
all.

The block of data sent from the jth process is received by every
process and placed in the jth block of the buffer out.

Parameter ``in``:
    The send buffer with the data to send.

Parameter ``sendlen``:
    The number of elements to send on each task.

Parameter ``out``:
    The buffer to store the received data in.

Parameter ``recvlen``:
    An array with size equal to the number of processes containing the
    number of elements to receive from process i at position i, i.e.
    the number that is passed as sendlen argument to this function in
    process i.

Parameter ``displ``:
    An array with size equal to the number of processes. Data received
    from process i will be written starting at out+displ[i].

Returns:
    MPI_SUCCESS (==0) if successful, an MPI error code otherwise)doc";

static const char *__doc_Dune_Communication_allreduce =
R"doc(Compute something over all processes for each component of an array
and return the result in every process.

The template parameter BinaryFunction is the type of the binary
function to use for the computation

Parameter ``inout``:
    The array to compute on.

Parameter ``len``:
    The number of components in the array

Returns:
    MPI_SUCCESS (==0) if successful, an MPI error code otherwise)doc";

static const char *__doc_Dune_Communication_allreduce_2 =
R"doc(Compute something over all processes for each component of an array
and return the result in every process.

The template parameter BinaryFunction is the type of the binary
function to use for the computation

Parameter ``in``:
    The array to compute on.

Parameter ``out``:
    The array to store the results in.

Parameter ``len``:
    The number of components in the array

Returns:
    MPI_SUCCESS (==0) if successful, an MPI error code otherwise)doc";

static const char *__doc_Dune_Communication_barrier =
R"doc(Wait until all processes have arrived at this point in the program.

Returns:
    MPI_SUCCESS (==0) if successful, an MPI error code otherwise)doc";

static const char *__doc_Dune_Communication_broadcast =
R"doc(Distribute an array from the process with rank root to all other
processes

Returns:
    MPI_SUCCESS (==0) if successful, an MPI error code otherwise)doc";

static const char *__doc_Dune_Communication_gather =
R"doc(Gather arrays on root task.

Each process sends its in array of length len to the root process
(including the root itself). In the root process these arrays are
stored in rank order in the out array which must have size len *
number of processes.

Parameter ``in``:
    The send buffer with the data to send.

Parameter ``out``:
    The buffer to store the received data in. Might have length zero
    on non-root tasks.

Parameter ``len``:
    The number of elements to send on each task.

Parameter ``root``:
    The root task that gathers the data.

Returns:
    MPI_SUCCESS (==0) if successful, an MPI error code otherwise)doc";

static const char *__doc_Dune_Communication_gatherv =
R"doc(Gather arrays of variable size on root task.

Each process sends its in array of length sendlen to the root process
(including the root itself). In the root process these arrays are
stored in rank order in the out array.

Parameter ``in``:
    The send buffer with the data to be sent

Parameter ``sendlen``:
    The number of elements to send on each task

Parameter ``out``:
    The buffer to store the received data in. May have length zero on
    non-root tasks.

Parameter ``recvlen``:
    An array with size equal to the number of processes containing the
    number of elements to receive from process i at position i, i.e.
    the number that is passed as sendlen argument to this function in
    process i. May have length zero on non-root tasks.

Parameter ``displ``:
    An array with size equal to the number of processes. Data received
    from process i will be written starting at out+displ[i] on the
    root process. May have length zero on non-root tasks.

Parameter ``root``:
    The root task that gathers the data.

Returns:
    MPI_SUCCESS (==0) if successful, an MPI error code otherwise)doc";

static const char *__doc_Dune_Communication_iallgather =
R"doc(Gathers data from all tasks and distribute it to all nonblocking.

Returns:
    Future<TOUT, TIN> containing the distributed data)doc";

static const char *__doc_Dune_Communication_iallreduce =
R"doc(Compute something over all processes nonblocking

Returns:
    Future<TOUT, TIN> containing the computed something)doc";

static const char *__doc_Dune_Communication_iallreduce_2 =
R"doc(Compute something over all processes nonblocking and in-place

Returns:
    Future<T> containing the computed something)doc";

static const char *__doc_Dune_Communication_ibarrier =
R"doc(Nonblocking barrier

Returns:
    Future<void> which is complete when all processes have reached the
    barrier)doc";

static const char *__doc_Dune_Communication_ibroadcast =
R"doc(Distribute an array from the process with rank root to all other
processes nonblocking

Returns:
    Future<T> containing the distributed data)doc";

static const char *__doc_Dune_Communication_igather =
R"doc(Gather arrays on root task nonblocking

Returns:
    Future<TOUT, TIN> containing the gathered data)doc";

static const char *__doc_Dune_Communication_irecv =
R"doc(Receives the data from the source_rank nonblocking

Returns:
    Future<T> containing the received data when complete)doc";

static const char *__doc_Dune_Communication_iscatter =
R"doc(Scatter array from a root to all other task nonblocking.

Returns:
    Future<TOUT, TIN> containing scattered data;)doc";

static const char *__doc_Dune_Communication_isend =
R"doc(Sends the data to the dest_rank nonblocking

Returns:
    Future<T> containing the send buffer, completes when data is send)doc";

static const char *__doc_Dune_Communication_max =
R"doc(Compute the maximum of the argument over all processes and return the
result in every process. Assumes that T has an operator<)doc";

static const char *__doc_Dune_Communication_max_2 =
R"doc(Compute the maximum over all processes for each component of an array
and return the result in every process. Assumes that T has an
operator<

Returns:
    MPI_SUCCESS (==0) if successful, an MPI error code otherwise)doc";

static const char *__doc_Dune_Communication_min =
R"doc(Compute the minimum of the argument over all processes and return the
result in every process. Assumes that T has an operator<)doc";

static const char *__doc_Dune_Communication_min_2 =
R"doc(Compute the minimum over all processes for each component of an array
and return the result in every process. Assumes that T has an
operator<

Returns:
    MPI_SUCCESS (==0) if successful, an MPI error code otherwise)doc";

static const char *__doc_Dune_Communication_operator_No_Comm = R"doc(cast to the underlying Fake MPI communicator)doc";

static const char *__doc_Dune_Communication_prod =
R"doc(Compute the product of the argument over all processes and return the
result in every process. Assumes that T has an operator*)doc";

static const char *__doc_Dune_Communication_prod_2 =
R"doc(Compute the product over all processes for each component of an array
and return the result in every process. Assumes that T has an
operator*

Returns:
    MPI_SUCCESS (==0) if successful, an MPI error code otherwise)doc";

static const char *__doc_Dune_Communication_rank = R"doc(Return rank, is between 0 and size()-1)doc";

static const char *__doc_Dune_Communication_recv =
R"doc(Receives the data from the source_rank

Returns:
    MPI_SUCCESS (==0) if successful, an MPI error code otherwise)doc";

static const char *__doc_Dune_Communication_rrecv = R"doc()doc";

static const char *__doc_Dune_Communication_scatter =
R"doc(Scatter array from a root to all other task.

The root process sends the elements with index from k*len to
(k+1)*len-1 in its array to task k, which stores it at index 0 to
len-1.

Parameter ``send``:
    The array to scatter. Might have length zero on non-root tasks.

Parameter ``recv``:
    The buffer to store the received data in. Upon completion of the
    method each task will have same data stored there as the one in
    send buffer of the root task before.

Parameter ``len``:
    The number of elements in the recv buffer.

Parameter ``root``:
    The root task that gathers the data.

Returns:
    MPI_SUCCESS (==0) if successful, an MPI error code otherwise)doc";

static const char *__doc_Dune_Communication_scatterv =
R"doc(Scatter arrays of variable length from a root to all other tasks.

The root process sends the elements with index from send+displ[k] to
send+displ[k]-1 in its array to task k, which stores it at index 0 to
recvlen-1.

Parameter ``send``:
    The array to scatter. May have length zero on non-root tasks.

Parameter ``sendlen``:
    An array with size equal to the number of processes containing the
    number of elements to scatter to process i at position i, i.e. the
    number that is passed as recvlen argument to this function in
    process i.

Parameter ``displ``:
    An array with size equal to the number of processes. Data
    scattered to process i will be read starting at send+displ[i] on
    root the process.

Parameter ``recv``:
    The buffer to store the received data in. Upon completion of the
    method each task will have the same data stored there as the one
    in send buffer of the root task before.

Parameter ``recvlen``:
    The number of elements in the recv buffer.

Parameter ``root``:
    The root task that gathers the data.

Returns:
    MPI_SUCCESS (==0) if successful, an MPI error code otherwise)doc";

static const char *__doc_Dune_Communication_send =
R"doc(Sends the data to the dest_rank

Returns:
    MPI_SUCCESS (==0) if successful, an MPI error code otherwise)doc";

static const char *__doc_Dune_Communication_size = R"doc(Number of processes in set, is greater than 0)doc";

static const char *__doc_Dune_Communication_sum =
R"doc(Compute the sum of the argument over all processes and return the
result in every process. Assumes that T has an operator+)doc";

static const char *__doc_Dune_Communication_sum_2 =
R"doc(Compute the sum over all processes for each component of an array and
return the result in every process. Assumes that T has an operator+

Returns:
    MPI_SUCCESS (==0) if successful, an MPI error code otherwise)doc";

static const char *__doc_Dune_FakeMPIHelper =
R"doc(@file Helpers for dealing with MPI.

Basically there are two helpers available: <dl> <dt>FakeMPIHelper</dt>
<dd>A class adhering to the interface of MPIHelper that does not need
MPI at all. This can be used to create a sequential program even if
MPI is used to compile it. </dd> <dt>MPIHelper</dt> <dd>A real MPI
helper. When the singleton gets instantiated MPI_Init will be called
and before the program exits MPI_Finalize will be called. </dd> </dl>

Example of who to use these classes:

A program that is parallel if compiled with MPI and sequential
otherwise:

```
int main(int argc, char** argv){
     typedef Dune::MPIHelper MPIHelper;
     MPIHelper::instance(argc, argv);
     typename MPIHelper::MPICommunicator world =
       MPIHelper::getCommunicator();
     ...
```

If one wants to have sequential program even if the code is compiled
with mpi then one simply has to exchange the typedef with

```
typedef Dune::MPIHelper FakeMPIHelper;
```

.

For checking whether we really use MPI or just fake please use
MPIHelper::isFake (this is also possible at compile time!)

A fake mpi helper.

This helper can be used if no MPI is available or one wants to run
sequentially even if MPI is available and used.)doc";

static const char *__doc_Dune_FakeMPIHelper_FakeMPIHelper = R"doc()doc";

static const char *__doc_Dune_FakeMPIHelper_FakeMPIHelper_2 = R"doc()doc";

static const char *__doc_Dune_FakeMPIHelper_getCollectiveCommunication = R"doc()doc";

static const char *__doc_Dune_FakeMPIHelper_getCommunication = R"doc()doc";

static const char *__doc_Dune_FakeMPIHelper_getCommunicator =
R"doc(get the default communicator

Return a communicator to exchange data with all processes

Returns:
    a fake communicator)doc";

static const char *__doc_Dune_FakeMPIHelper_getLocalCommunicator =
R"doc(get a local communicator

Returns a communicator to communicate with the local process only

Returns:
    a fake communicator)doc";

static const char *__doc_Dune_FakeMPIHelper_instance =
R"doc(Get the singleton instance of the helper.

This method has to be called with the same arguments that the main
method of the program was called:

```
int main(int argc, char** argv){
  MPIHelper::instance(argc, argv);
  // program code comes here
  ...
}
```

Parameter ``argc``:
    The number of arguments provided to main.

Parameter ``argv``:
    The arguments provided to main.)doc";

static const char *__doc_Dune_FakeMPIHelper_isFake =
R"doc(Are we fake (i.e. pretend to have MPI support but are compiled
without.))doc";

static const char *__doc_Dune_FakeMPIHelper_operator_assign = R"doc()doc";

static const char *__doc_Dune_FakeMPIHelper_rank = R"doc(return rank of process, i.e. zero)doc";

static const char *__doc_Dune_FakeMPIHelper_size = R"doc(return rank of process, i.e. one)doc";

static const char *__doc_Dune_No_Comm = R"doc()doc";

static const char *__doc_Dune_operator_eq =
R"doc(! Comparison operator for MPI compatibility

Always returns true.)doc";

static const char *__doc_Dune_operator_ne =
R"doc(! Comparison operator for MPI compatibility

Always returns false.)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif
