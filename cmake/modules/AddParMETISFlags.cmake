# Defines the functions to use ParMETIS
#
# .. cmake_function:: dune_add_parmetis_flags
#
#    .. cmake_param:: targets
#       :positional:
#       :single:
#       :required:
#
#       A list of targets to use ParMETIS with.
#

# set HAVE_PARMETIS for config.h
set(HAVE_PARMETIS ${ParMETIS_FOUND})

# register all ParMETIS related flags
if(ParMETIS_FOUND)
  dune_register_package_flags(LIBRARIES ParMETIS::ParMETIS)
endif()

# add function to link against the ParMETIS library
function(dune_add_parmetis_flags _targets)
  if(ParMETIS_FOUND)
    foreach(_target ${_targets})
      target_link_libraries(${_target} PUBLIC ParMETIS::ParMETIS)
    endforeach(_target)
  endif()
endfunction(dune_add_parmetis_flags)

macro(add_dune_parmetis_flags _targets)
  message(DEPRECATION "add_dune_parmetis_flags is deprecated. Use 'dune_add_parmetis_flags' instead.")
  dune_add_gmp_flags(${_targets})
endmacro(add_dune_parmetis_flags)