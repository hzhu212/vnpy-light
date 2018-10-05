# The following variables should be set:

# PYTHON_INTERP: like "python35"
# PYTHON_ARCH: 32 or 64
# PYTHON_INCLUDE_DIR: like "D:/App_x86/Python35-32/include"
# PYTHON_LIBRARIES: like "D:/App_x86/Python35-32/libs/python35.lib"
# Boost_INCLUDE_DIR: like "D:/ProgramFiles/boost_1_68_0"
# Boost_LIBRARY_DIR: like "D:/ProgramFiles/boost_1_68_0/lib32-msvc-14.0"


# this config file does'n work, when linking with MSVC, it throws the following error:
#    error LNK2019: unresolved external symbol "struct _object * __cdecl boost::python::detail::init_module(char const *,void (__cdecl*)(void))" (?init_module@detail@python@boost@@YAPEAU_object@@PEBDP6AXXZ@Z)
# the cause of this issue might be that new versions of Boost or Visual Studio doesn't support python2


# assistant variables, just temporary
set(_PYTHON27_64_ROOT "D:/App/Python27")
set(_PYTHON36_64_ROOT "D:/App/Python36")
# weird!!! when building with python27, it requires python36.lib also!!!
set(_PYTHON27_LIBRARY "${_PYTHON27_64_ROOT}/libs/python27.lib")
set(_PYTHON36_LIBRARY "${_PYTHON36_64_ROOT}/libs/python36.lib")


# set variables
set(PYTHON_INTERP python27)
set(PYTHON_ARCH 64)
set(PYTHON_INCLUDE_DIR "${_PYTHON27_64_ROOT}/include")
set(PYTHON_LIBRARIES ${_PYTHON27_LIBRARY} ${_PYTHON36_LIBRARY})
set(Boost_INCLUDE_DIR "D:/ProgramFiles/boost_1_68_0")
set(Boost_LIBRARY_DIR "D:/ProgramFiles/boost_1_68_0/lib64-msvc-14.0")
