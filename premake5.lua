-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

workspace "pneumatic_sdk_%{_ACTION}"
startproject "demo"

configurations{ "Debug", "Release" }
filter{ "configurations:Debug" } symbols "On" optimize "Off"
filter{ "configurations:Release" } symbols "Off" optimize "Speed"
filter{}

platforms{ "x86", "x64", }
filter{ "platforms:*32", "platforms:*86" } architecture "x86"
filter{ "platforms:*64" } architecture "x86_64"
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

solution_items{
	"README.md",
	"premake5.lua",
	"premake5-system.lua",
	"engine.ini",
	"editor.ini",
	"extensions.cfg",
	"TODO.txt",
}

-- modules
include "modules/assimp/assimp.premake5.lua"
include "modules/freetype2/freetype2.premake5.lua"
include "modules/glew/glew.premake5.lua"
include "modules/glfw/glfw.premake5.lua"
include "modules/imgui/imgui.premake5.lua"
include "modules/lua/lua.premake5.lua"
include "modules/mono/mono.premake5.lua"
include "modules/zip/zip.premake5.lua"

-- engine
include "pneumatic.premake5.lua"

-- programs
include "demo/demo.premake5.lua"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- global project settings
project "*"

debugdir "%{_BUILD}"

debugenvs{ "%{_BUILD}" }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- C++ project settings
filter{ "language:C++" }

	libdirs{ "%{_BUILD_BIN}", }

	includedirs{
		"%{wks.location}",
		"%{_THIRDPARTY}/",
		"%{_THIRDPARTY}/assimp/include/",
		"%{_THIRDPARTY}/entt/src/",
		"%{_THIRDPARTY}/freetype2/include/",
		"%{_THIRDPARTY}/freetype2/include/freetype/",
		"%{_THIRDPARTY}/gcem/include/",
		"%{_THIRDPARTY}/glfw/include/",
		"%{_THIRDPARTY}/imgui/",
		"%{_THIRDPARTY}/json/include/",
		"%{_THIRDPARTY}/mono/include/",
		"%{_THIRDPARTY}/lua/",
		"%{_THIRDPARTY}/zlib/",
		"%{_THIRDPARTY}/minizip/",
	}

	prebuildcommands{
		"{MKDIR} %{_BUILD}",
		"{MKDIR} %{_BUILD_BIN}",
		"{MKDIR} %{_BUILD_DAT}",
		"{MKDIR} %{_BUILD_CFG}",
		"{MKDIR} %{_BUILD_RES}",
		"{MKDIR} %{_BUILD_USR}",
	}

	if _TARGET_OS=="windows" then
		links_win32()
		generate_manifest("platform/windows/binaries.manifest")
	end

filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- C# project settings
filter{ "language:C#" }
	-- nothing to do here
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --