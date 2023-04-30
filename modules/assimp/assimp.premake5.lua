-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- assimp
group			"Modules"
project			"assimp"
kind			"Utility"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
targetdir		"%{_BUILD_BIN}"
objdir			"%{_TEMPS}"
location		"%{_PROJECT}"

manifest("assimp")

includedirs{
	"%{wks.location}/thirdparty/assimp/include",
}

files{
	"%{wks.location}/modules/%{prj.name}/%{prj.name}.premake5.lua",
}

postbuildcommands{
	"{COPYFILE} %{_VENDOR}/assimp%{DLL} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/assimp%{LIB} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/IrrXML%{LIB} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/zlibstatic%{LIB} %{cfg.targetdir}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --