# The following variables should be set:

# PYTHON_INTERP: like "python35"
# PYTHON_ARCH: 32 or 64
# PYTHON_INCLUDE_DIR: like "D:/App_x86/Python35-32/include"
# PYTHON_LIBRARIES: like "D:/App_x86/Python35-32/libs/python35.lib"
# Boost_INCLUDE_DIR: like "D:/ProgramFiles/boost_1_68_0"
# Boost_LIBRARY_DIR: like "D:/ProgramFiles/boost_1_68_0/lib32-msvc-14.0"


# assistant variables, just temporary
set(_PYTHON35_32_ROOT "D:/App_x86/Python35-32")
set(_PYTHON36_32_ROOT "D:/App_x86/Python36-32")
# weird!!! when building with python35, it requires python36.lib also!!!
set(_PYTHON35_LIBRARY "${_PYTHON35_32_ROOT}/libs/python35.lib")
set(_PYTHON36_LIBRARY "${_PYTHON36_32_ROOT}/libs/python36.lib")


# set variables
set(PYTHON_INTERP python35)
set(PYTHON_ARCH 32)
set(PYTHON_INCLUDE_DIR "${_PYTHON35_32_ROOT}/include")
set(PYTHON_LIBRARIES ${_PYTHON35_LIBRARY} ${_PYTHON36_LIBRARY})
set(Boost_INCLUDE_DIR "D:/ProgramFiles/boost_1_68_0")
set(Boost_LIBRARY_DIR "D:/ProgramFiles/boost_1_68_0/lib32-msvc-14.0")
