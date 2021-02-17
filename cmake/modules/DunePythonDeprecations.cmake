# This file only contains the deprecation messages from the overhaul
# of the Python CMake integration. After the deprecation period, this
# file can simply be deleted (and its include in DunePythonCommonMacros.cmake removed)

if(DUNE_PYTHON_INSTALL_EDITABLE)
  message(WARNING
"    The DUNE_PYTHON_INSTALL_EDITABLE variable is deprecated.
     Installations into the Dune-internal virtualenv are always
     editable. If you want to enable editable installation into
     your system environment (e.g. because it is a virtual environment),
     you can do so with `-DDUNE_PYTHON_ADDITIONAL_PIP_PARAMS=--editable`
")
endif()
