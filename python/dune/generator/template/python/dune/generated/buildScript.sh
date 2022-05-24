echo Building
. flags.make
{{ cxx_compiler }} $CXX_DEFINES $CXX_INCLUDES $CXX_FLAGS -MD -MT $1.dir/$1.cc.o -MF $1.dir/$1.cc.o.d -o ./$1.dir/$1.cc.o -c $1.cc
bash linker.sh $1
echo Done
