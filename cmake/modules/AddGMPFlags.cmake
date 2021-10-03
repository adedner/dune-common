# Defines the functions to use GMP
#
# .. cmake_function:: add_dune_gmp_flags
#
#    .. cmake_param:: targets
#       :positional:
#       :single:
#       :required:
#
#       A list of targets to use GMP with.
#
include_guard(GLOBAL)

# set HAVE_GMP for the config.h file
set(HAVE_GMP ${GMP_FOUND} CACHE INTERNAL "")

# register all GMP related flags
if(GMP_FOUND)
  dune_register_package_flags(
    LIBRARIES GMP::gmpxx
    COMPILE_DEFINITIONS "ENABLE_GMP=1"
  )
endif()

# add function to link against the GMP library
function(add_dune_gmp_flags _targets)
  if(GMP_FOUND)
    foreach(_target ${_targets})
      get_target_property(_type ${_target} TYPE)
      set(_scope "PUBLIC")
      if (${_type} STREQUAL "INTERFACE_LIBRARY")
        set(_scope "INTERFACE")
      endif()
      target_link_libraries(${_target} ${_scope} GMP::gmpxx)
      target_compile_definitions(${_target} ${_scope} ENABLE_GMP=1)
    endforeach(_target ${_targets})
  endif(GMP_FOUND)
endfunction(add_dune_gmp_flags)
