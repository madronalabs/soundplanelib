#--------------------------------------------------------------------
# soundplanelib/CMakeLists.txt
# root CMake file for soundplanelib project.
#--------------------------------------------------------------------


#--------------------------------------------------------------------
# set min version and deployment target -- before project
#--------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10" CACHE STRING "Minimum OS X deployment version")

#--------------------------------------------------------------------
# project and version
#--------------------------------------------------------------------

project(soundplanelib CXX)

set(PROJECT_NAME Soundplanelib)
set(SP_VERSION_MAJOR 1)
set(SP_VERSION_MINOR 8)
set(SP_VERSION_UPDATE 0)
set(SP_VERSION "${SP_VERSION_MAJOR}.${SP_VERSION_MINOR}.${SP_VERSION_UPDATE}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#--------------------------------------------------------------------
# choose USB library to use
#--------------------------------------------------------------------

option(SP_USE_LIBUSB "use libusb-1.0 based driver" OFF)

if(NOT APPLE)
  set(SP_USE_LIBUSB ON)
endif()

if(SP_USE_LIBUSB)
  include(FindPkgConfig)
endif()

#--------------------------------------------------------------------
# Compiler flags
#--------------------------------------------------------------------

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O3")

#--------------------------------------------------------------------
# Choose library output name
#--------------------------------------------------------------------

set(soundplanelib_NAME soundplane)

#--------------------------------------------------------------------
# Enforce out of source build
#--------------------------------------------------------------------

if(CMAKE_BINARY_DIR EQUAL CMAKE_SOURCE_DIR)
  message(FATAL_ERROR "Madronalib requires an out of source build")
endif()

#--------------------------------------------------------------------
# Add subdirectories
#--------------------------------------------------------------------

add_subdirectory(source)
add_subdirectory(examples)
