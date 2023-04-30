-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- pneumatic
group			""
project			"pneumatic"
kind			"SharedLib"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
rtti			"On"
targetname		"%{prj.name}"
targetdir		"%{_BUILD_BIN}"
objdir			"%{_TEMPS}"
location		"%{_PROJECT}"

manifest("pneumatic")

dependson{ "assimp", "freetype2", "glfw", "imgui", "zip", }

links{ "assimp%{LIB}", "freetype", "glfw", "imgui", "IrrXML", "zip", "zlibstatic", }

links_graphics()

defines{
	"DYNAMIC_BUILD_ENABLED=1",
	"TOOLS_ENABLED=1",
	"IMGUI_API=PN_API_IMPORT",
	--"IOWIN32_USING_WINRT_API=1",
}

srcdirs(
	"%{wks.location}/core/",
	"%{wks.location}/drivers/".._OPTIONS["gfxapi"].."/",
	"%{wks.location}/drivers/%{_TARGET_OS}/",
	"%{wks.location}/editor/",
	"%{wks.location}/main/",
	"%{wks.location}/platform/%{_TARGET_OS}/",
	"%{wks.location}/scene/",
	"%{wks.location}/servers/"
)

files{
	"%{wks.location}/pneumatic.premake5.lua",
	"%{wks.location}/drivers/register_driver_types.hpp",
	"%{wks.location}/drivers/register_driver_types.cpp",
	"%{wks.location}/modules/register_module_types.hpp",
	"%{wks.location}/modules/register_module_types.cpp",
	"%{wks.location}/platform/register_platform_apis.hpp",
	"%{wks.location}/platform/register_platform_apis.cpp",
}

postbuildcommands{
	"{COPYFILE} %{wks.location}/engine.ini %{_BUILD_CFG}",
	"{COPYFILE} %{wks.location}/editor.ini %{_BUILD_CFG}",
	"{COPYFILE} %{wks.location}/extensions.cfg %{_BUILD_CFG}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --



-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --