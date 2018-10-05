# build CTP API

# set CTP API path according to system platform
if(WIN32)
    message(STATUS "Under Windows. IS_X64: ${IS_X64}")
    if(IS_X64)
        set(CTPAPI_PATH ctpapi/win64)
    else()
        set(CTPAPI_PATH ctpapi/win32)
    endif()
elseif(UNIX)
    message(STATUS "Under Linux. IS_X64: ${IS_X64}")
    if(IS_X64)
        set(CTPAPI_PATH ctpapi/linux64)
    else()
        message(FATAL_ERROR "CTP API supports only 64 bits under Linux")
    endif()
else()
    message(FATAL_ERROR "CTP API supports only Windows and Linux platform")
endif()

# reset path to absulute path for stability considerations
set(CTPAPI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${CTPAPI_PATH})
message(STATUS "CTPAPI_PATH: ${CTPAPI_PATH}")

# include headers of CTP API
include_directories(${CTPAPI_PATH})

# find CTP API libraries: .lib and .dll
unset(CTPAPI_MD_LIBRARY CACHE)
unset(CTPAPI_TD_LIBRARY CACHE)
find_library(CTPAPI_MD_LIBRARY NAMES thostmduserapi PATHS ${CTPAPI_PATH})
find_library(CTPAPI_TD_LIBRARY NAMES thosttraderapi PATHS ${CTPAPI_PATH})
message(STATUS "CTPAPI_MD_LIBRARY: " ${CTPAPI_MD_LIBRARY})
message(STATUS "CTPAPI_TD_LIBRARY: " ${CTPAPI_TD_LIBRARY})

# set path of CTP API wrapper source code
set(CTP_MD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vnctpmd)
set(CTP_TD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vnctptd)
message(STATUS "CTP_MD_PATH: ${CTP_MD_PATH}")
message(STATUS "CTP_TD_PATH: ${CTP_TD_PATH}")

# include headers of CTP API wrapper
include_directories(${CTP_MD_PATH})
include_directories(${CTP_TD_PATH})

# add shared library of CTP API wrapper
add_library(vnctpmd SHARED ${CTP_MD_PATH}/vnctpmd.cpp)
add_library(vnctptd SHARED ${CTP_TD_PATH}/vnctptd.cpp)

# set properties of the libraries to build
if(WIN32)
    # on Windows, change the libraries' extension name from .dll to .pyd
    set_target_properties(vnctpmd vnctptd PROPERTIES SUFFIX ".pyd")
elseif(UNIX)
    # on Linux, remove the default prefix (lib) of .so file
    set_target_properties(vnctpmd vnctptd PROPERTIES PREFIX "")
endif()

# link all the libraries: CTP wrapper, CTP API, Python, Boost
target_link_libraries(vnctpmd ${CTPAPI_MD_LIBRARY} ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(vnctptd ${CTPAPI_TD_LIBRARY} ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})

# after build has been completed, copy .pyd/.so to ctp directory
add_custom_command(TARGET vnctpmd
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:vnctpmd> ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_command(TARGET vnctptd
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:vnctptd> ${CMAKE_CURRENT_SOURCE_DIR})

# and then, copy corresponding .dll(s) to ctp directory and build directory
file(GLOB CTPAPI_DLL ${CTPAPI_PATH}/*.dll)
add_custom_command(TARGET vnctptd
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CTPAPI_DLL} ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CTPAPI_DLL} "${LIBRARY_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}"
    COMMAND ${CMAKE_COMMAND} -E echo "====== Build files and corresponding libraries have been copied to ${CMAKE_CURRENT_SOURCE_DIR}"
    )

# now we have made .pyd/.so file according to system platform
