-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- zip
group			"Modules"
project			"zip"
kind			"StaticLib"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
targetdir		"%{_BUILD_BIN}"
objdir			"%{_TEMPS}"
location		"%{_PROJECT}"

--manifest("zlib")

--defines{ "ZLIB_DLL" }

includedirs{ "%{_THIRDPARTY}/minizip/", "%{_THIRDPARTY}/zlib/", }

files{ "%{wks.location}/modules/zip/zip.premake5.lua" }

srcdirs("%{_THIRDPARTY}/minizip/", "%{_THIRDPARTY}/zlib/")

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --