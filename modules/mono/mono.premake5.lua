-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- mono
project			"mono"
kind			"SharedLib"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
targetdir		"%{_BUILD_BIN}"
objdir			"%{_TEMPS}"
location		"%{_PROJECT}"

manifest("mono-2.0-sgen")

dependson{ "pneumatic", }

links{ "assimp%{LIB}", "freetype", "glfw", "imgui", "IrrXML", "pneumatic", "mono-2.0-sgen", "zip", "zlibstatic", }

defines{
	"PN_MOD_API=PN_API_EXPORT",
}

includedirs{
	"%{wks.location}/thirdparty/mono/include/",
}

srcdirs(
	"%{wks.location}/modules/mono/"
)

files{
	"%{wks.location}/modules/mono/mono.premake5.lua",
}

prebuildcommands{
	"{COPYFILE} %{_VENDOR}/mono-2.0-sgen%{DLL} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/mono-2.0-sgen%{LIB} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/mono-2.0-sgen.pdb %{cfg.targetdir}",
}

postbuildcommands{
	"{COPYFILE} %{wks.location}/modules/mono/mono.ini %{_BUILD_CFG}",
}
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --