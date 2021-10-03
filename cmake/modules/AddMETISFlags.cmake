# Defines the functions to use METIS
#
# .. cmake_function:: add_dune_metis_flags
#
#    .. cmake_param:: targets
#       :positional:
#       :single:
#       :required:
#
#       A list of targets to use METIS with.
#
include_guard(GLOBAL)

# register HAVE_METIS for config.h
set(HAVE_METIS ${METIS_FOUND} CACHE INTERNAL "")

# register METIS library as dune package
if(METIS_FOUND)
  dune_register_package_flags(LIBRARIES METIS::METIS)
endif()

# Add function to link targets against METIS library
function(add_dune_metis_flags _targets)
  if(METIS_FOUND)
    foreach(_target ${_targets})
      get_target_property(_type ${_target} TYPE)
      set(_scope "PUBLIC")
      if (${_type} STREQUAL "INTERFACE_LIBRARY")
        set(_scope "INTERFACE")
      endif()
      target_link_libraries(${_target} ${_scope} METIS::METIS)
    endforeach(_target)
  endif()
endfunction(add_dune_metis_flags _targets)
