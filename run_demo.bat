@echo off
setlocal

:: Run demo
:: Usage: run_demo.bat input.jpg output_dir --width 300

set BUILD_TYPE=Release
set EXE=build\%BUILD_TYPE%\seam_carving_demo.exe

if not exist "%EXE%" (
    echo [run_demo] ERROR: %EXE% not found. Please run build_msvc.bat first.
    exit /b 1
)

echo [run_demo] Running %EXE% %*
"%EXE%" %*
