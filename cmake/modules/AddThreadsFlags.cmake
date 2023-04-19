# SPDX-FileCopyrightInfo: Copyright © DUNE Project contributors, see file LICENSE.md in module root
# SPDX-License-Identifier: LicenseRef-GPL-2.0-only-with-DUNE-exception

include_guard(GLOBAL)

# text for feature summary
set_package_properties("Threads" PROPERTIES
  DESCRIPTION "Multi-threading library")

find_package(Threads)

set_property(GLOBAL APPEND PROPERTY DUNE_DEPENDENCY_REGISRTY "THREADS")
set_property(GLOBAL PROPERTY        DUNE_DEPENDENCY_REGISRTY_THREADS "include(AddThreadsFlags)")

# set HAVE_THREADS for config.h
set(HAVE_THREADS ${Threads_FOUND})

# register the Threads imported target globally
if(Threads_FOUND)
  link_libraries(Threads::Threads)
  add_compile_definitions(ENABLE_THREADS=1)
endif()
