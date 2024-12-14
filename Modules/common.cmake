# common.cmake
#   use 'includecommon.cmake' to find this file
#
# This file defines/consumes the following options:
#
#   BUILD_WALL_WERROR
#   BUILD_WITH_CCACHE
#
# This cmake files sets the following:
#
#   CMAKE_DEBUG_POSTFIX
#   CMAKE_CXX_FLAGS
#
# also adds a few defines to warn/error when building
#



# Safety and sanity checks.
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree build - bad practice!")
endif()

# Warnings as errors
OPTION(BUILD_WALL_WERROR "Build with -wall -werror etc" On)

# ccache - disabled by default
OPTION(BUILD_WITH_CCACHE "Use ccache to improve build times." On)
if(BUILD_WITH_CCACHE)
  find_program(CCACHE_FOUND ccache)
  if(NOT CCACHE_FOUND)
    message(FATAL_ERROR "Could not find ccache. BUILD_WITH_CCACHE is On. Is it installed? sudo apt install ccache")
  endif()
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()
message(STATUS "BUILD_WITH_CCACHE set to ${BUILD_WITH_CCACHE}.")


# Report build type once and only once.
if( NOT MESSAGE_COMMON_TELL_BUILD_TYPE )
  set(MESSAGE_COMMON_TELL_BUILD_TYPE 1)
  if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE set to: <empty>")
  else()
    message(STATUS "CMAKE_BUILD_TYPE set to: ${CMAKE_BUILD_TYPE}")
  endif()
endif()

# git should ignore the build directory.
file(WRITE "${CMAKE_BINARY_DIR}/.gitignore" "*\n")

# search 64 bit libs
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)


if(BUILD_WALL_WERROR)
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden" )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden" )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror" )
  #set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunreachable-code" )  At this time, -Wunreachable-code is probably unstable.
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfloat-equal")
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
  #  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wzero-as-null-pointer-constant")
endif()
