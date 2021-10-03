# Defines the functions to use TBB
#
# .. cmake_function:: add_dune_tbb_flags
#
#    .. cmake_param:: targets
#       :positional:
#       :single:
#       :required:
#
#       A list of targets to use TBB with.
#
include_guard(GLOBAL)

# set variable for config.h
set(HAVE_TBB ${TBB_FOUND} CACHE INTERNAL "")

# perform DUNE-specific setup tasks
if (TBB_FOUND)
  dune_register_package_flags(
    COMPILE_DEFINITIONS ENABLE_TBB=1
    LIBRARIES TBB::tbb
    )
endif()

# function for adding TBB flags to a list of targets
function(add_dune_tbb_flags _targets)
  if(TBB_FOUND)
    foreach(_target ${_targets})
      get_target_property(_type ${_target} TYPE)
      set(_scope "PUBLIC")
      if (${_type} STREQUAL "INTERFACE_LIBRARY")
        set(_scope "INTERFACE")
      endif()
      target_link_libraries(${_target} ${_scope} TBB::tbb)
      target_compile_definitions(${_target} ${_scope} ENABLE_TBB=1)
    endforeach(_target)
  endif()
endfunction(add_dune_tbb_flags)
