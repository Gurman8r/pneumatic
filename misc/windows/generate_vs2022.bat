@echo off
cd ..\..\
call .\misc\windows\premake5.exe vs2022
if %ERRORLEVEL% NEQ 0 ( pause )
rem pause