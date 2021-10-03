#[=======================================================================[.rst:
CheckCXXFeatures
----------------

This module checks for several CXX compiler features and introduces a macro to check if
given C++ source compiles and links into an executable.

.. command:: dune_check_cxx_source_compiles

  .. code-block:: cmake

    dune_check_cxx_source_compiles(<sources> <var>)

  The macro checks that the given sources compiles and links into an executable. Thus,
  it must at least provide a valid ``main()`` function. The macro is inspired by the
  cmake function ``check_cxx_source_compiles()`` but does not provide all of its
  functionality. It is also based on the :command:`try_compile` command but without any
  additional flags set, like include directories or link options. The only fixed flag that
  is passed to :command:`try_compile` is ``CXX_STANDARD`` with value 17. This can only be
  influenced by setting the global variable ``CMAKE_CXX_STANDARD``.

#]=======================================================================]

macro(dune_check_cxx_source_compiles SOURCE VAR)
  if(NOT DEFINED ${VAR})
    message(STATUS "Performing Test ${VAR}")
    file(WRITE "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/src_${VAR}.cxx" "${SOURCE}\n")

    try_compile(${VAR} ${CMAKE_BINARY_DIR}
        ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/src_${VAR}.cxx
      CXX_STANDARD 17
      OUTPUT_VARIABLE OUTPUT)

    file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
      "Performing C++ SOURCE FILE Test ${VAR} succeeded with the following output:\n"
      "${OUTPUT}\n"
      "Source file was:\n${SOURCE}\n")
    if(${VAR})
      message(STATUS "Performing Test ${VAR} - Success")
      set(${VAR} 1 CACHE INTERNAL "Test ${VAR}")
      file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
        "Performing C++ SOURCE FILE Test ${VAR} succeeded with the following output:\n"
        "${OUTPUT}\n"
        "Source file was:\n${SOURCE}\n")
    else()
      message(STATUS "Performing Test ${VAR} - Failed")
      set(${VAR} "" CACHE INTERNAL "Test ${VAR}")
      file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
        "Performing C++ SOURCE FILE Test ${VAR} failed with the following output:\n"
        "${OUTPUT}\n"
        "Source file was:\n${SOURCE}\n")
    endif()
  endif()
endmacro(dune_check_cxx_source_compiles)


function(dune_require_cxx_standard)
  message(DEPRECATION "No need to call dune_require_cxx_standard() any more. If you want to "
    "enforce a higher standard than requested in the core modules, simply use the cmake "
    "command target_compile_features(<target> <PRIVATE|PUBLIC|INTERFACE> cxx_std_[17|20|...]) "
    "to set the requirement on your target directly. Note, it is recommended to compile all "
    "modules with the same c++ standard flags. This can be achieved by setting the global "
    "cmake variable CMAKE_CXX_STANDARD to the requested maximal standard for all dune modules.")
endfunction()


# -----------------------------------------------------------------------------
# perform compile checks
# -----------------------------------------------------------------------------

# __attribute__((unused))
dune_check_cxx_source_compiles("
   int main(void)
   {
     int __attribute__((unused)) foo;
     return 0;
   };
" HAS_ATTRIBUTE_UNUSED)

# __attribute__((deprecated))
dune_check_cxx_source_compiles("
#define DEP __attribute__((deprecated))
   class bar
   {
     bar() DEP;
   };

   class peng { } DEP;

   template <class T>
   class t_bar
   {
     t_bar() DEP;
   };

   template <class T>
   class t_peng {
     t_peng() {};
   } DEP;

   void foo() DEP;

   void foo() {}

   int main(void)
   {
     return 0;
   };
"  HAS_ATTRIBUTE_DEPRECATED)

# __attribute__((deprecated("msg")))
dune_check_cxx_source_compiles("
#define DEP __attribute__((deprecated(\"message\")))
   class bar {
     bar() DEP;
   };

   class peng { } DEP;

   template <class T>
   class t_bar
   {
     t_bar() DEP;
   };

   template <class T>
   class t_peng
   {
     t_peng() {};
   } DEP;

   void foo() DEP;

   void foo() {}

   int main(void)
   {
     return 0;
   };
"  HAS_ATTRIBUTE_DEPRECATED_MSG)

# Check for `std::experimental::make_array(...)`
dune_check_cxx_source_compiles("
  #include <experimental/array>
  int main() { auto a = std::experimental::make_array(1,2,3); }
" DUNE_HAVE_CXX_EXPERIMENTAL_MAKE_ARRAY)

# Check for `std::experimental::is_detected<...>`
dune_check_cxx_source_compiles("
  #include <experimental/type_traits>
  int main() { std::experimental::detected_t<std::decay_t,int>{}; }
" DUNE_HAVE_CXX_EXPERIMENTAL_IS_DETECTED)

# Check for `std::identity<...>`
dune_check_cxx_source_compiles("
  #include <functional>
  int main() { std::identity{}; }
" DUNE_HAVE_CXX_STD_IDENTITY)
