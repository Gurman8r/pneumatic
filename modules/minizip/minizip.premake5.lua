-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- minizip
project			"minizip"
kind			"StaticLib"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
targetdir		"%{_BUILD_BIN}"
objdir			"%{_TEMPS}"
location		"%{_PROJECT}"

dependson{ "zlib" }

links{ "zlib" }

defines{ "ZLIB_DLL" }

includedirs{ "%{_THIRDPARTY}/minizip/", "%{_THIRDPARTY}/zlib/" }

files{ "%{wks.location}/modules/minizip/minizip.premake5.lua" }

srcdirs("%{_THIRDPARTY}/minizip/")

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --