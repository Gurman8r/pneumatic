@echo off
cd ..\..\
call .\misc\windows\premake5.exe vs2017
if %ERRORLEVEL% NEQ 0 ( pause )
rem pause