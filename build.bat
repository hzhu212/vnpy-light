@echo off

rem print help message if lacks parameter
if "%1"=="" goto printHelp
if "%2"=="" goto printHelp

rem read parameters
set CONFIG_NAME=%1
set ARCH=%2

rem load config for building
call config-build.bat


rem architecture x64
if "%ARCH%"=="64" (
@echo You have choosed architecture [x64]
if not exist build_api_64 mkdir build_api_64
pushd build_api_64
%cmake% -G "%MSVC_IDENTIFIER% Win64" .. && ^
%MSBuild64% ALL_BUILD.vcxproj /nologo /t:build /p:Configuration=Release
popd

rem architecture x86
) else (
@echo You have choosed architecture [x86]
if not exist build_api_32 mkdir build_api_32
pushd build_api_32
%cmake% .. && ^
%MSBuild32% ALL_BUILD.vcxproj /nologo /t:build /p:Configuration=Release
popd
)

exit 0
pause



:printHelp

@echo.
@echo Usage:
@echo   build.bat ^<CONFIG_NAME^> ^<ARCH^>
@echo.
@echo Argument:
@echo   CONFIG_NAME - name of a cmake-config-file. config-files are under `./cmake`
@echo   directory and with names like `config-*.cmake`.
@echo   ARCH - architecture of the target file you want to build (32 or 64).
@echo.
@echo Example:
@echo   build.bat config-win-py35-x32 32
@echo   build.bat config-win-py35-x64 64
@echo.
@echo WARN: `ARCH` should be consistent with the architecture of python declared at
@echo `CONFIG_NAME`, or it will lead to an error.
@echo.
