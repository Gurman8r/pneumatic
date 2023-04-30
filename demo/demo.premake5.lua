-- demo
cpp_project_common("Demo", "demo", "ConsoleApp", "%{_BUILD}")

links_graphics()

dependson{ "pneumatic", "demo-CS", "lua", "mono", }

links{ "assimp%{LIB}", "freetype", "glfw", "imgui", "IrrXML", "pneumatic", "mono-2.0-sgen", "zip", "zlibstatic", }

defines{
	"MAIN_ENABLED=1",
	"TOOLS_ENABLED=1",
	"IMGUI_API=ISM_API_IMPORT",
}

files{
	"%{_SLN}demo/demo.premake5.lua",
	"%{_SLN}assets/**.**",
	"%{_PLATFORM}%{_TARGET_OS}/main_%{_TARGET_OS}.cpp",
	"%{_PLATFORM}windows/pneumatic.rc",
	"%{_PLATFORM}windows/pneumatic.ico",
}

prebuildcommands{
	"{COPYFILE} %{_PLATFORM}windows/binaries.manifest %{_BUILD_BIN}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- demo-CS
csharp_project_common("Demo", "demo-CS", "SharedLib", "%{_BUILD_BIN}")

files{ "%{_SLN}demo/**.cs" }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --