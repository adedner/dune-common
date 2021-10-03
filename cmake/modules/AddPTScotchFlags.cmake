# Defines the functions to use PTScotch
#
# .. cmake_function:: add_dune_ptscotch_flags
#
#    .. cmake_param:: targets
#       :positional:
#       :single:
#       :required:
#
#       A list of targets to use PTScotch with.
#
include_guard(GLOBAL)

# set HAVE_PTSCOTCH for config.h
set(HAVE_PTSCOTCH ${PTScotch_FOUND} CACHE INTERNAL "")

# register all PTScotch related flags
if(PTScotch_SCOTCH_FOUND)
  dune_register_package_flags(LIBRARIES PTScotch::Scotch)
endif()
if(PTScotch_PTSCOTCH_FOUND)
  dune_register_package_flags(LIBRARIES PTScotch::PTScotch)
endif()

function(add_dune_ptscotch_flags _targets)
  if(PTScotch_SCOTCH_FOUND)
    foreach(_target ${_targets})
      get_target_property(_type ${_target} TYPE)
      set(_scope "PUBLIC")
      if (${_type} STREQUAL "INTERFACE_LIBRARY")
        set(_scope "INTERFACE")
      endif()
      target_link_libraries(${_target} ${_scope} PTScotch::Scotch)
    endforeach(_target ${_targets})
  endif()
  if(PTScotch_PTSCOTCH_FOUND)
    foreach(_target ${_targets})
      get_target_property(_type ${_target} TYPE)
      set(_scope "PUBLIC")
      if (${_type} STREQUAL "INTERFACE_LIBRARY")
        set(_scope "INTERFACE")
      endif()
      target_link_libraries(${_target} ${_scope} PTScotch::PTScotch)
    endforeach(_target ${_targets})
  endif()
endfunction(add_dune_ptscotch_flags)
