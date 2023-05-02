@echo off
cls
set working=%cd%\
pushd %working%

rem PATHS
set _SOLUTION=%working%..\..\pneumatic_sdk_vs2022.sln
set _VSTUDIO_DIR=C:\Program Files\Microsoft Visual Studio\2022\Community\
set _VCVARS_DIR=%_VSTUDIO_DIR%VC\Auxiliary\Build\
set _MSBUILD_DIR=%_VSTUDIO_DIR%Msbuild\Current\Bin\

rem OPTIONS
set _PLATFORM=%1
set _CONFIGURATION=%2

rem DISPLAY INFORMATION
echo SOLUTION PATH: %_SOLUTION%
echo VCVARS PATH:   %_VCVARS_DIR%
echo MSBUILD PATH:  %_MSBUILD_DIR%
echo PLATFORM:      %_PLATFORM%
echo CONFIGURATION: %_CONFIGURATION%
echo

rem SETUP ENVIRONMENT
cd %_VCVARS_DIR%
if %_PLATFORM% == "x32" 	( call "vcvarsall.bat" x86 )
if %_PLATFORM% == "x86" 	( call "vcvarsall.bat" x86 )
if %_PLATFORM% == "x64" 	( call "vcvarsall.bat" x64 )
if %_PLATFORM% == "Win32" 	( call "vcvarsall.bat" x86 )
if %_PLATFORM% == "Win64" 	( call "vcvarsall.bat" x64 )

rem RUN BUILD
cd %_MSBUILD_DIR%
call msbuild.exe %_SOLUTION% /p:PlatformTarget=%_PLATFORM% /p:Configuration=%_CONFIGURATION%
pause