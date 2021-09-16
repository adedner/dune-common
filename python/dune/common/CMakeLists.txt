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
target_link_libraries(_common PUBLIC Dune::common)
install(TARGETS _common LIBRARY DESTINATION python/dune/common)
