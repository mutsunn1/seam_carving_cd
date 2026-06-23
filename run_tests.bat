@echo off
setlocal

:: Run C++ tests
:: Usage:
::   run_tests.bat [Debug|Release] [module|all]
::   module: energy_map, dp_search, seam_remove, seam_insert, resize, demo
::   省略 module 则运行全量测试 seam_carving_tests.exe

set BUILD_TYPE=%1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Release

set MODULE=%2
if "%MODULE%"=="" set MODULE=all

if "%MODULE%"=="all" (
    set EXE=build\%BUILD_TYPE%\seam_carving_tests.exe
) else (
    set EXE=build\%BUILD_TYPE%\test_%MODULE%.exe
)

if not exist "%EXE%" (
    echo [run_tests] ERROR: %EXE% not found. Please run build_msvc.bat first.
    exit /b 1
)

echo [run_tests] Running %EXE% ...
"%EXE%"
