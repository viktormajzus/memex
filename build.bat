@echo off
set PRESET=%1
if "%PRESET%"=="" set PRESET=x64-release

echo Building with preset %PRESET%...
cmake --preset %PRESET%
cmake --build --preset %PRESET%
