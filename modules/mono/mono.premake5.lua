-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- mono
cpp_project_common("Modules", "mono", "SharedLib")

targetdir("%{_BUILD_BIN}")

manifest("mono-2.0-sgen")

dependson{ "pneumatic", }

links{ "assimp%{LIB}", "freetype", "glfw", "imgui", "IrrXML", "pneumatic", "mono-2.0-sgen", "zip", "zlibstatic", }

defines{
	"PNU_MOD_API=PNU_API_EXPORT",
}

includedirs{
	"%{_THIRDPARTY}mono/include/",
}

files{
	"%{_MODULES}mono/mono.premake5.lua",
	"%{_MODULES}mono/**.hpp",
	"%{_MODULES}mono/**.cpp",
}

prebuildcommands{
	"{COPYFILE} %{_VENDOR}mono-2.0-sgen%{DLL} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}mono-2.0-sgen%{LIB} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}mono-2.0-sgen.pdb %{cfg.targetdir}",
}

postbuildcommands{
	"{COPYFILE} %{_MODULES}mono/mono.ini %{_BUILD_CFG}",
}
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --