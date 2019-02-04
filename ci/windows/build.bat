
@echo off

::Display information
@echo.
echo Environment variables:
echo.
set
echo.

echo.
echo CMake version:
echo.
cmake --version
echo.


::Detect CI_GENERATOR
if "%CI_GENERATOR%"== "" (
    @echo WARN: CI_GENERATOR is not defined. set to Visual Studio 15 2017 Win64
    set CI_GENERATOR=Visual Studio 15 2017 Win64
) else (
    @echo CI_GENERATOR: %CI_GENERATOR%
)

::Detect CI_BUILD_TYPE
if "%CI_BUILD_TYPE%"== "" (
    @echo WARN: CI_BUILD_TYPE is not defined. set to Release
    set CI_BUILD_TYPE=Release
) else (
    @echo CI_BUILD_TYPE: %CI_BUILD_TYPE%
)

::Determine repository path
if not "%BUILD_REPOSITORY_LOCALPATH%"=="" (
    @echo BUILD_REPOSITORY_LOCALPATH defined: %BUILD_REPOSITORY_LOCALPATH%
    set REPOSITORY_PATH=%BUILD_REPOSITORY_LOCALPATH%
) else (
    set REPOSITORY_PATH=C:\Cygwin64\home\wenho\scattercc
)
pushd . || exit /b 1
cd /D %REPOSITORY_PATH% || exit /b 1
set REPOSITORY_PATH=%cd%
popd || exit /b 1
@echo REPOSITORY_PATH: %REPOSITORY_PATH%


::Determine number of cores
if "%NUMBER_OF_PROCESSORS%"== "" (
    @echo WARN: NUMBER_OF_PROCESSORS is not defined. set to 1
    set NUMBER_OF_PROCESSORS=1
) else (
    @echo NUMBER_OF_PROCESSORS: %NUMBER_OF_PROCESSORS%
)


::Build and test
if exist %SystemDrive%\scattercc_build (
    rmdir /q /s %SystemDrive%\scattercc_build || exit /b 1
)
mkdir %SystemDrive%\scattercc_build || exit /b 1
cd /D %SystemDrive%\scattercc_build || exit /b 1

cmake %REPOSITORY_PATH% -G "%CI_GENERATOR%" || exit /b 1

cmake --build . --config %CI_BUILD_TYPE% -j %NUMBER_OF_PROCESSORS% --target ALL_BUILD || exit /b 1
::cmake --build . --config %CI_BUILD_TYPE% --target ALL_BUILD || exit /b 1

::cmake --build . --config %CI_BUILD_TYPE% --target INSTALL || exit /b 1

ctest -VV -C %CI_BUILD_TYPE% || exit /b 1
