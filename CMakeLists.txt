
# From http://eldalin.com/2010/10/21/introduction-to-cmake-2/
########################################################################
#
# A generalized cmake file for developing cross-platform games.
#

cmake_minimum_required (VERSION 2.6 FATAL_ERROR)
# set (CMAKE_VERBOSE_MAKEFILE ON)


########################################################################
# Ensure that we are not building in our source directories.

set (Build_Dir_OK "TRUE")
string (REGEX MATCH "^${CMAKE_SOURCE_DIR}" In_Sub_Dir ${CMAKE_BINARY_DIR})
if (In_Sub_Dir)
  string (REGEX MATCH "^${CMAKE_SOURCE_DIR}/build" In_Build_Dir ${CMAKE_BINARY_DIR})
  if (NOT In_Build_Dir)
    set (Build_Dir_OK "FALSE")
  endif ()
endif ()

if (NOT Build_Dir_OK)
  message (FATAL_ERROR "You must run cmake from a directory that is not in your source tree, or that is in a special subdirectory of the tree whose name begins with ‘build’.")
endif ()


########################################################################
# Set up the basic build environment

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This messes up
  # differentiation between debug and release builds.
  set (CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()


########################################################################

link_directories(
  )

include_directories(
  ${CMAKE_SOURCE_DIR}/gdemu
  )

ADD_SUBDIRECTORY(gdemu)
ADD_SUBDIRECTORY(asteroids)
