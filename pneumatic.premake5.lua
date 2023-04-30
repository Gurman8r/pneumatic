-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- pneumatic
cpp_project_common("Engine", "pneumatic", "SharedLib", "%{_BUILD_BIN}")

manifest("pneumatic")

links_graphics()

dependson{ "assimp", "freetype2", "glfw", "imgui", "zip", }

links{ "assimp%{LIB}", "freetype", "glfw", "imgui", "IrrXML", "zip", "zlibstatic", }

defines{
	"DYNAMIC_BUILD_ENABLED=1",
	"TOOLS_ENABLED=1",
	"IMGUI_API=PNU_API_IMPORT",
}

files{
	"%{_SLN}pneumatic.premake5.lua",
	"%{_SLN}engine.ini",
	"%{_SLN}editor.ini",
	"%{_SLN}extensions.cfg",

	"%{_CORE}**.hpp",
	"%{_CORE}**.cpp",
	"%{_CORE}**.inl",
	
	"%{_DRIVERS}register_driver_types.hpp",
	"%{_DRIVERS}register_driver_types.cpp",
	"%{_DRIVERS}%{_TARGET_OS}/**.hpp",
	"%{_DRIVERS}%{_TARGET_OS}/**.cpp",
	"%{_DRIVERS}".._OPTIONS["gfxapi"].."/**.hpp",
	"%{_DRIVERS}".._OPTIONS["gfxapi"].."/**.cpp",
	
	"%{_EDITOR}**.hpp",
	"%{_EDITOR}**.cpp",
	
	"%{_MAIN}**.hpp",
	"%{_MAIN}**.cpp",

	"%{_MODULES}register_module_types.hpp",
	"%{_MODULES}register_module_types.cpp",

	"%{_PLATFORM}register_platform_apis.cpp",
	"%{_PLATFORM}register_platform_apis.hpp",
	"%{_PLATFORM}%{_TARGET_OS}/**.hpp",
	"%{_PLATFORM}%{_TARGET_OS}/**.cpp",
	
	"%{_SCENE}**.hpp",
	"%{_SCENE}**.cpp",
	
	"%{_SERVERS}**.hpp",
	"%{_SERVERS}**.cpp",
}

postbuildcommands{
	"{COPYFILE} %{_SLN}engine.ini %{_BUILD_CFG}",
	"{COPYFILE} %{_SLN}editor.ini %{_BUILD_CFG}",
	"{COPYFILE} %{_SLN}extensions.cfg %{_BUILD_CFG}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --