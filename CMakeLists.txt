#
#  Copyright (C) 2015 Rob Parrish
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along
#  with this program; if not, write to the Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

project(gaussian C CXX)
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Custom CMake modules location
list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_SOURCE_DIR}/cmake/compilers
    ${CMAKE_SOURCE_DIR}/cmake/math
)

# Options
option(ENABLE_OMP           "Enable OpenMP parallelization" ON)
option(ENABLE_PYTHON        "Enable Python interface" ON)
option(ENABLE_CXX11_SUPPORT "Enable C++11 compiler support" ON)

option(LIBINT               "Location of libint2 installation" "")
option(LIBAMBIT             "Location of ambit installation" "")

# include cmake modules as needed
include(ConfigVersion)
include(ConfigArchitecture)
include(ConfigCompilerFlags)
include(ConfigOMP)

include(ConfigExplicitLibs)
include(ConfigSafeGuards)
include(GenericMacros)

find_package(Threads REQUIRED)
link_libraries("${CMAKE_THREAD_LIBS_INIT}")

# Python Detection
include(ConfigPython)
link_libraries("${PYTHON_LIBRARIES}")
include_directories(SYSTEM "${PYTHON_INCLUDE_DIRS}")

# Boost Detection
# We need Boost.Python, so this has to come _after_ Python detection
include(ConfigBoost)
link_directories("${Boost_LIBRARY_DIRS}")
include_directories(SYSTEM "${Boost_INCLUDE_DIRS}")

# Append the suffix given from input to all generated executables
if(EXECUTABLE_SUFFIX)
   set(CMAKE_EXECUTABLE_SUFFIX "${EXECUTABLE_SUFFIX}")
   message(STATUS "Suffix ${CMAKE_EXECUTABLE_SUFFIX} will be appended to executables")
endif()

if(LDFLAGS)
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS}")
endif()

# libgaussian-specific includes and libraries
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(
    ${LIBINT}/include ${LIBINT}/include/libint2
    ${LIBAMBIT}/include
    ${PROJECT_BINARY_DIR}/include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

# recursively add source directories
add_subdirectory(src)

#python wrappers
add_subdirectory(python)

# sample suite
add_subdirectory(samples)

# this has to be the very last CMake module to be included
include(ConfigInfo)
