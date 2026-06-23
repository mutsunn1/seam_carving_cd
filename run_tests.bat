@echo off
setlocal

:: Run C++ tests
:: Usage: run_tests.bat [Debug|Release] [module_prefix]
::   module_prefix: 只运行名字以该前缀开头的用例，例如 energy_map、dp_search

set BUILD_TYPE=%1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Release

set MODULE_PREFIX=%2

set EXE=build\%BUILD_TYPE%\seam_carving_tests.exe

if not exist "%EXE%" (
    echo [run_tests] ERROR: %EXE% not found. Please run build_msvc.bat first.
    exit /b 1
)

if "%MODULE_PREFIX%"=="" (
    echo [run_tests] Running %EXE% ...
    "%EXE%"
) else (
    echo [run_tests] Running %EXE% with prefix "%MODULE_PREFIX%" ...
    "%EXE%" "%MODULE_PREFIX%"
)
