# .. cmake_module::
#
#    Module that checks for inkscape
#
#    Sets the following variables
#
#    :code:`INKSCAPE_FOUND`
#       Whether inkscape was found
#
#    :code:`INKSCAPE`
#       Path to inkscape to generate .png's form .svg's

# text for feature summary
include(FeatureSummary)
set_package_properties("Inkscape" PROPERTIES
  DESCRIPTION "converts SVG images"
  URL "www.inkscape.org"
  PURPOSE "To generate the documentation with LaTeX")

find_program(INKSCAPE inkscape DOC "Path to inkscape to generate png files from svg files")
# find_program(CONVERT convert DOC "Path to convert program")

# Behave like a CMake module is supposed to behave
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args("Inkscape" REQUIRED_VARS INKSCAPE)

if(INKSCAPE)
  set(INKSCAPE_FOUND TRUE)

  # check for inkscape >= 1.0
  execute_process(COMMAND ${INKSCAPE} -z -e OUTPUT_QUIET ERROR_QUIET RESULT_VARIABLE INKSCAPE_RETURNED_ONE)

  # if error (i.e. 1) was returned we have new inkscape version (>=1.0)
  if(INKSCAPE_RETURNED_ONE)
    set(INKSCAPE_NEW_VERSION True)
  endif()
endif(INKSCAPE)