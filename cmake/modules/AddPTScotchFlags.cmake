# Defines the functions to use PTScotch
#
# .. cmake_function:: dune_add_ptscotch_flags
#
#    .. cmake_param:: targets
#       :positional:
#       :single:
#       :required:
#
#       A list of targets to use PTScotch with.
#

# set HAVE_PTSCOTCH for config.h
set(HAVE_PTSCOTCH ${PTScotch_FOUND})

# register all PTScotch related flags
if(PTScotch_FOUND)
  dune_register_package_flags(LIBRARIES "PTScotch::PTScotch")
endif()

function(dune_add_ptscotch_flags _targets)
  if(PTScotch_FOUND)
    foreach(_target ${_targets})
      target_link_libraries(${_target} PUBLIC PTScotch::PTScotch)
    endforeach(_target ${_targets})
  endif()
endfunction(dune_add_ptscotch_flags)

macro(add_dune_ptscotch_flags _targets)
  message(DEPRECATION "add_dune_ptscotch_flags is deprecated. Use 'dune_add_ptscotch_flags' instead.")
  dune_add_gmp_flags(${_targets})
endmacro(add_dune_ptscotch_flags)