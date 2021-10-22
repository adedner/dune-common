add_python_targets(common
  __init__
  checkconfiguration
  compatibility
  deprecated            # deprecated 2.8
  hashit
  locking
  module
  pickle                # deprecated 2.8
  project
  utility
)
dune_add_pybind11_module(NAME _common)
set_property(TARGET _common PROPERTY LINK_LIBRARIES dunecommon APPEND)
install(TARGETS _common LIBRARY DESTINATION python/dune/common)

set( DIMS 1 2 3 )
foreach( DIM ${DIMS} )
  dune_add_pybind11_module(NAME _defaultfieldvector_${DIM}
                           SOURCES _defaultfieldvector.cc
                           COMPILE_DEFINITIONS PRIVATE "DIM=${DIM};MODNAME=_defaultfieldvector_${DIM}")
  set_property(TARGET _defaultfieldvector_${DIM} PROPERTY LINK_LIBRARIES dunecommon APPEND)
  install(TARGETS _defaultfieldvector_${DIM} LIBRARY DESTINATION python/dune/common)
endforeach()
