include_guard(GLOBAL)

# text for feature summary
set_package_properties("Threads" PROPERTIES
  DESCRIPTION "Multi-threading library")

# set HAVE_THREADS for config.h
set(HAVE_THREADS ${Threads_FOUND} CACHE INTERNAL "")

# register the Threads imported target globally
if(Threads_FOUND)
  link_libraries(Threads::Threads)
endif()
