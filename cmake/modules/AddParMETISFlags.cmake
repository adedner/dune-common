# Defines the functions to use ParMETIS
#
# .. cmake_function:: add_dune_parmetis_flags
#
#    .. cmake_param:: targets
#       :positional:
#       :single:
#       :required:
#
#       A list of targets to use ParMETIS with.
#
include_guard(GLOBAL)

include(AddMPIFlags)

# set HAVE_PARMETIS for config.h
set(HAVE_PARMETIS ${ParMETIS_FOUND} CACHE INTERNAL "")

# register all ParMETIS related flags
if(ParMETIS_FOUND)
  dune_register_package_flags(LIBRARIES ParMETIS::ParMETIS)
endif()

# add function to link against the ParMETIS library
function(add_dune_parmetis_flags _targets)
  if(ParMETIS_FOUND)
    foreach(_target ${_targets})
      get_target_property(_type ${_target} TYPE)
      set(_scope "PUBLIC")
      if (${_type} STREQUAL "INTERFACE_LIBRARY")
        set(_scope "INTERFACE")
      endif()
      target_link_libraries(${_target} ${_scope} ParMETIS::ParMETIS)
    endforeach(_target)
    add_dune_mpi_flags(${ARGV})
  endif()
endfunction(add_dune_parmetis_flags)
