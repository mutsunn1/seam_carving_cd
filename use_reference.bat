@echo off
:: 将 reference/ 中的参考实现复制到 src/，覆盖占位代码
echo [use_reference] Copying reference implementations to src/ ...
copy /Y reference\*.cpp src\
echo [use_reference] Done. You can now build with build_msvc.bat.
