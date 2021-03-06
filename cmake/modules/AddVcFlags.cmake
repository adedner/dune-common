# Defines the functions to use Vc
#
# Vc is a library for high-level Vectorization support in C++
# see https://github.com/VcDevel/Vc
#
# .. cmake_function:: dune_add_vc_flags
#
#    .. cmake_param:: targets
#       :positional:
#       :single:
#       :required:
#
#       A list of targets to use VC with.
#

function(dune_add_vc_flags _targets)
  if(Vc_FOUND)
    foreach(_target ${_targets})
      target_link_libraries(${_target} PUBLIC ${Vc_LIBRARIES})
      target_compile_options(${_target} PUBLIC ${Vc_COMPILE_FLAGS})
      target_compile_definitions(${_target} PUBLIC ENABLE_VC=1)
      target_include_directories(${_target} SYSTEM PUBLIC ${Vc_INCLUDE_DIR})
    endforeach(_target ${_targets})
  endif(Vc_FOUND)
endfunction(dune_add_vc_flags)

macro(add_dune_vc_flags _targets)
  message(DEPRECATION "add_dune_vc_flags is deprecated. Use 'dune_add_vc_flags' instead.")
  dune_add_gmp_flags(${_targets})
endmacro(add_dune_vc_flags)

if(Vc_FOUND)
  dune_register_package_flags(COMPILE_OPTIONS "${Vc_COMPILE_FLAGS};-DENABLE_VC=1"
                              LIBRARIES "${Vc_LIBRARIES}"
                              INCLUDE_DIRS "${Vc_INCLUDE_DIR}")
endif(Vc_FOUND)
set(HAVE_VC ${Vc_FOUND})
