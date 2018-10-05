cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0074 NEW)

project(vnpy_api)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_MODULE_PATH ${CMAKE_ROOT}/Modules ${CMAKE_SOURCE_DIR}/cmake)
include(_config_compiler_)

# get user-specified cmake-config file
set(CONFIG_NAME $ENV{CONFIG_NAME})
set(CONFIG_FILE ${CMAKE_SOURCE_DIR}/cmake/${CONFIG_NAME}.cmake)
message(STATUS "Using config name: ${CONFIG_NAME}. Config file: ${CONFIG_FILE}")

# set output path for all APIs
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/${CONFIG_NAME})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/${CONFIG_NAME})

# build the project as static library by default
option(BUILD_SHARED_LIBS "build the project as shared libraries" OFF)

# set boost options
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)

# load developer-specified configuration
include(_config_)

# Now we have get the following variables from config:
# PYTHON_INTERP: like "python35"
# PYTHON_ARCH: 32 or 64
# PYTHON_INCLUDE_DIR: like "D:/App_x86/Python35-32/include"
# PYTHON_LIBRARIES: like "D:/App_x86/Python35-32/libs/python35.lib"
# Boost_INCLUDE_DIR: like "D:/ProgramFiles/boost_1_68_0"
# Boost_LIBRARY_DIR: like "D:/ProgramFiles/boost_1_68_0/lib32-msvc-14.0"
# Boost_LIBRARIES: like "D:/ProgramFiles/boost_1_68_0/lib32-msvc-14.0"

# include headers
include_directories(${PYTHON_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

# go into api directory
add_subdirectory(vnpy/api)
