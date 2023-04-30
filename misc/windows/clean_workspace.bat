@echo off

rem CLEANUP PROJECT FILES

if exist ..\..\pneumatic_sdk_vs2017.sln del /q ..\..\pneumatic_sdk_vs2017.sln
if exist ..\..\pneumatic_sdk_vs2019.sln del /q ..\..\pneumatic_sdk_vs2019.sln
if exist ..\..\pneumatic_sdk_vs2022.sln del /q ..\..\pneumatic_sdk_vs2022.sln

if exist ..\..\.vs rd /s /q ..\..\.vs
if exist ..\..\project rd /s /q ..\..\project
if exist ..\..\workspace rd /s /q ..\..\workspace

if exist "..\..\Visual Studio 2019" rd /s /q "..\..\Visual Studio 2019"
if exist "..\..\Visual Studio 2019Templates" rd /s /q "..\..\Visual Studio 2019Templates"

if %ERRORLEVEL% NEQ 0 ( pause ) else ( echo clean projects OK )