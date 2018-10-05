# --------------------------------------------------------
# !!! PLEASE DON NOT MODIFIED THIS FILE !!!
# YOU SHOULD WRITE YOUR CONFIGS IN `config-*.cmake` FILES.
# --------------------------------------------------------

# check config file
if(NOT EXISTS ${CONFIG_FILE})
    message(FATAL_ERROR "Config file `${CONFIG_FILE}` does not exist. Please Make sure you are using the correct config file name.")
endif()

include(${CONFIG_NAME})

# in CONFIG_NAME.cmake, the following variables should have been set:
# PYTHON_INTERP: like "python35"
# PYTHON_ARCH: 32 or 64
# PYTHON_INCLUDE_DIR: like "D:/Program Files (x86)/Python35-32/include"
# PYTHON_LIBRARIES: like "D:/Program Files (x86)/Python35-32/libs/python35.lib"
# Boost_INCLUDE_DIR: like "D:/ProgramFiles/boost_1_68_0"
# Boost_LIBRARY_DIR: like "D:/ProgramFiles/boost_1_68_0/lib32-msvc-14.0"

# ensure variables exist
set(HINT_MESSAGE "You must have it set in `${CONFIG_FILE}`")
if(NOT PYTHON_INTERP)
    message(FATAL_ERROR "Variable `PYTHON_INTERP` is not set. ${HINT_MESSAGE}.")
endif()
if(NOT PYTHON_ARCH)
    message(FATAL_ERROR "Variable `PYTHON_ARCH` is not set. ${HINT_MESSAGE}.")
endif()
if(NOT PYTHON_INCLUDE_DIR)
    message(FATAL_ERROR "Variable `PYTHON_INCLUDE_DIR` is not set. ${HINT_MESSAGE}.")
endif()
if(NOT PYTHON_LIBRARIES)
    message(FATAL_ERROR "Variable `PYTHON_LIBRARIES` is not set. ${HINT_MESSAGE}.")
endif()
if(NOT Boost_INCLUDE_DIR)
    message(FATAL_ERROR "Variable `Boost_INCLUDE_DIR` is not set. ${HINT_MESSAGE}.")
endif()
if(NOT Boost_LIBRARY_DIR)
    message(FATAL_ERROR "Variable `Boost_LIBRARY_DIR` is not set. ${HINT_MESSAGE}.")
endif()

# ensure directories and files exist
set(HINT_MESSAGE "Please check your config file: `${CONFIG_FILE}`")
if(NOT IS_DIRECTORY ${PYTHON_INCLUDE_DIR})
    message(FATAL_ERROR "PYTHON_INCLUDE_DIR: `${PYTHON_INCLUDE_DIR}` is not a directory. ${HINT_MESSAGE}.")
endif()
foreach(PYLIB ${PYTHON_LIBRARIES})
    if(NOT EXISTS ${PYLIB})
        message(FATAL_ERROR "PYTHON_LIBRARY: `${PYLIB}` does not exist. ${HINT_MESSAGE}.")
    endif()
endforeach(PYLIB)
if(NOT IS_DIRECTORY ${Boost_INCLUDE_DIR})
    message(FATAL_ERROR "Boost_INCLUDE_DIR: `${Boost_INCLUDE_DIR}` is not a directory. ${HINT_MESSAGE}.")
endif()
if(NOT IS_DIRECTORY ${Boost_LIBRARY_DIR})
    message(FATAL_ERROR "Boost_LIBRARY_DIR: `${Boost_LIBRARY_DIR}` is not a directory. ${HINT_MESSAGE}.")
endif()

# check python architecture
if(NOT ${PYTHON_ARCH} MATCHES $ENV{ARCH})
    message(FATAL_ERROR "Target architecture(ARCH=$ENV{ARCH}) does not matchs python architecture(PYTHON_ARCH=${PYTHON_ARCH})."
        "${HINT_MESSAGE} and make sure the target architecture is accessible.")
endif()

# print messages
message(STATUS "==================== User Configured Variables ====================")
message(STATUS "PYTHON_INTERP: ${PYTHON_INTERP}")
message(STATUS "PYTHON_ARCH: ${PYTHON_ARCH}")
message(STATUS "PYTHON_INCLUDE_DIR: ${PYTHON_INCLUDE_DIR}")
message(STATUS "PYTHON_LIBRARIES: ${PYTHON_LIBRARIES}")
message(STATUS "Boost_INCLUDE_DIR: ${Boost_INCLUDE_DIR}")
message(STATUS "Boost_LIBRARY_DIR: ${Boost_LIBRARY_DIR}")
message(STATUS "==================== Configured Variables End ====================")


# find boost libraries
find_package(Boost 1.60 COMPONENTS ${PYTHON_INTERP} locale thread system atomic date_time chrono REQUIRED)

# if not found, throw an error and exit
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Some Boost libraries not found")
endif()

# if found
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
