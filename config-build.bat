rem you need to set the following variables:
rem MSVC_IDENTIFIER
rem MSBuild32
rem MSBuild64
rem cmake

rem set msvc identifier. Choose one of the following:
rem Visual Studio 15 2017
rem Visual Studio 14 2015
rem Visual Studio 12 2013
rem Visual Studio 11 2012
rem Visual Studio 10 2010
rem Visual Studio 9 2008
rem ----------------------------------------------------------------------------
set MSVC_IDENTIFIER=Visual Studio 14 2015
rem ----------------------------------------------------------------------------

rem set MSBuild path.
rem ----------------------------------------------------------------------------
set MSBuild32=C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe
set MSBuild64=C:\Windows\Microsoft.NET\Framework64\v4.0.30319\MSBuild.exe
rem ----------------------------------------------------------------------------

rem set cmake path
rem ----------------------------------------------------------------------------
rem set cmake=C:\Program Files\CMake\bin\cmake.exe
rem it's simpler if cmake is already in PATH
set cmake=cmake
rem ----------------------------------------------------------------------------
