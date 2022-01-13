# Defines the functions to use Python Development things
#
# .. cmake_function:: add_dune_python_flags
#
#    .. cmake_param:: targets
#       :positional:
#       :single:
#       :required:
#
#       A list of targets to use Python with.
#
include_guard(GLOBAL)

# set HAVE_QUADMATH for config.h
set(HAVE_PYTHON3_DEV ${Python3_Development_FOUND})
set(HAVE_PYTHON3_EMBED ${Python3_Development.Embed_FOUND})

# register the QuadMath imported target
if(HAVE_PYTHON3_DEV)
  message(WARNING ${Python3_LIBRARIES})
  dune_register_package_flags(
    LIBRARIES ${Python3_LIBRARIES}
    INCLUDE_DIRS ${Python3_INCLUDE_DIRS}
    COMPILE_DEFINITIONS "ENABLE_PYTHON3_DEV=1"
  )
endif()

# add function to link against QuadMath::QuadMath
function(add_dune_python3_dev_flags _targets)
  if(HAVE_PYTHON3_DEV)
    foreach(_target ${_targets})
      target_link_libraries(${_target} PUBLIC ${Python3_LIBRARIES})
      target_include_directories(dunecommon PRIVATE ${Python3_INCLUDE_DIRS})
      target_compile_definitions(${_target} PUBLIC ENABLE_PYTHON3_DEV=1)
    endforeach(_target ${_targets})
  endif()
endfunction(add_dune_python3_dev_flags)
