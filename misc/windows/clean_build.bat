@echo off

rem CLEANUP BUILDS AND TEMPORARIES

if exist ..\..\bin rd /s /q ..\..\bin
if exist ..\..\bin-lib rd /s /q ..\..\bin-lib
if exist ..\..\bin-obj rd /s /q ..\..\bin-obj
if exist ..\..\build rd /s /q ..\..\build
if exist ..\..\temps rd /s /q ..\..\temps
if exist ..\..\temporary rd /s /q ..\..\temporary
if exist ..\..\object rd /s /q ..\..\object

if exist ..\..\build_windows_x86_Debug rd /s /q ..\..\build_windows_x86_Debug
if exist ..\..\build_windows_x64_Debug rd /s /q ..\..\build_windows_x64_Debug
if exist ..\..\build_windows_x86_Release rd /s /q ..\..\build_windows_x86_Release
if exist ..\..\build_windows_x64_Release rd /s /q ..\..\build_windows_x64_Release

if %ERRORLEVEL% NEQ 0 ( pause ) else ( echo clean binaries OK )