# detect the architecture of target file is 64 bits or not
set(IS_X64 OFF)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(IS_X64 ON)
endif()
message(STATUS "Project target architecture IS_X64: ${IS_X64}")

# compiler settings
# if (CMAKE_COMPILER_IS_GNUC OR CMAKE_COMPILER_IS_GNUCXX)
message(STATUS "Using compiler: ${CMAKE_CXX_COMPILER_ID}")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GNU C/C++
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11")
    if(NOT IS_X64)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
    endif()
# elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using MSVC/Visual Studio C++
endif ()
