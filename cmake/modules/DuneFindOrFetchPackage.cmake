include_guard(GLOBAL)

function(dune_find_or_fetch_package)
  cmake_minimum_required(VERSION 3.13)
  cmake_parse_arguments(ARG
  ""
  "NAME"
  "FIND;FETCH"
  ${ARGN}
  )

  # try to get library from system
  find_package(${ARG_NAME} QUIET ${ARG_FIND})

  # ... library not installed, we build it from source
  if(NOT ${ARG_NAME}_FOUND)
    include(FetchContent)

    string(TOLOWER ${ARG_NAME} lcName)
    string(TOUPPER ${ARG_NAME} upName)
    FetchContent_GetProperties(${lcName})
    if(NOT ${lcName}_POPULATED)
      # if source project is besides a dir with package name, we take it as a fetch content source
      string(REPLACE "${CMAKE_PROJECT_NAME}" ${lcName} ${lcName}_SOURCE_DIR "${CMAKE_SOURCE_DIR}")
      if(EXISTS "${${lcName}_SOURCE_DIR}")
        set(FETCHCONTENT_SOURCE_DIR_${upName} "${${lcName}_SOURCE_DIR}" CACHE PATH "source directory of ${lcName}")
      endif()
      FetchContent_Declare(
        ${lcName}
        ${ARG_FETCH}
      )

      message("-- Populating ${lcName}")
      FetchContent_Populate(${lcName})
      add_subdirectory("${${lcName}_SOURCE_DIR}" "${${lcName}_BINARY_DIR}")
    endif()
  endif()
endfunction()
