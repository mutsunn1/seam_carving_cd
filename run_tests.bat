@echo off
setlocal

:: Run C++ tests
:: Usage: run_tests.bat [Debug|Release]

set BUILD_TYPE=%1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Release

set EXE=build\%BUILD_TYPE%\seam_carving_tests.exe

if not exist "%EXE%" (
    echo [run_tests] ERROR: %EXE% not found. Please run build_msvc.bat first.
    exit /b 1
)

echo [run_tests] Running %EXE% ...
"%EXE%"
