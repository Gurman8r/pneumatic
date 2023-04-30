-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- glew
group			"Modules"
project			"glew"
kind			"Utility"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
targetdir		"%{_BUILD_BIN}"
objdir			"%{_TEMPS}"
location		"%{_PROJECT}"

manifest("glew32", "glew32d")

files{
	"%{wks.location}/modules/%{prj.name}/%{prj.name}.premake5.lua",
}

filter{ "configurations:Debug" } postbuildcommands{ "{COPYFILE} %{_VENDOR}/glew32d%{DLL} %{cfg.targetdir}", "{COPYFILE} %{_VENDOR}/glew32d%{LIB} %{cfg.targetdir}", }
filter{ "configurations:Release" } postbuildcommands{ "{COPYFILE} %{_VENDOR}/glew32%{DLL} %{cfg.targetdir}", "{COPYFILE} %{_VENDOR}/glew32%{LIB} %{cfg.targetdir}", }
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --