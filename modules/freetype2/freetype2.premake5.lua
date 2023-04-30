-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- freetype2
group			"Modules"
project			"freetype2"
kind			"Utility"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
targetdir		"%{_BUILD_BIN}"
objdir			"%{_TEMPS}"
location		"%{_PROJECT}"

files{
	"%{wks.location}/modules/%{prj.name}/%{prj.name}.premake5.lua",
}

postbuildcommands{
	"{COPYFILE} %{_VENDOR}/freetype%{LIB} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}/freetype.pdb %{cfg.targetdir}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --