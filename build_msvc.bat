@echo off
setlocal enabledelayedexpansion

:: MSVC build script
:: Usage: build_msvc.bat [Debug|Release]
:: Requires Visual Studio 2022 with "Desktop development with C++" workload.

set BUILD_TYPE=%1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Release

echo [build_msvc] Build type: %BUILD_TYPE%

:: Locate vcvarsall.bat
set VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe
set VCVARS=

if exist "%VSWHERE%" (
    for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
        set VCVARS=%%i\VC\Auxiliary\Build\vcvarsall.bat
    )
)

if not exist "%VCVARS%" (
    echo [build_msvc] ERROR: vcvarsall.bat not found. Please install VS2022 C++ workload.
    exit /b 1
)

echo [build_msvc] Found vcvarsall: %VCVARS%

:: OpenCV must be provided via OPENCV_DIR environment variable
if not defined OPENCV_DIR (
    echo [build_msvc] ERROR: OPENCV_DIR is not set.
    echo.
    echo Please set OPENCV_DIR to the directory containing OpenCVConfig.cmake.
    echo Example:
    echo   set OPENCV_DIR=C:\opencv\build\x64\vc16\lib
    echo.
    echo If OpenCV is not installed, download it from:
    echo   https://github.com/opencv/opencv/releases
    exit /b 1
)

echo [build_msvc] OPENCV_DIR: %OPENCV_DIR%

:: Setup x64 MSVC environment and run CMake
call "%VCVARS%" x64 && cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DOpenCV_DIR="%OPENCV_DIR%" && cmake --build build --config %BUILD_TYPE%

if errorlevel 1 (
    echo [build_msvc] Build failed.
    exit /b 1
)

echo [build_msvc] Build succeeded. Executables are under build\%BUILD_TYPE%\
