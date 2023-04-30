-- demo
group			"Demo"
project			"demo"
kind			"ConsoleApp"
language		"C++"
cppdialect 		"C++17"
rtti			"On"
staticruntime	"Off"
systemversion	"latest"
targetname		"%{prj.name}"
targetdir		"%{_BUILD}"
objdir			"%{_TEMPS}"
location		"%{_PROJECT}"

dependson{ "pneumatic", "demo-CS", "lua", "mono", }

links{ "assimp%{LIB}", "freetype", "glfw", "imgui", "IrrXML", "pneumatic", "mono-2.0-sgen", "zip", "zlibstatic", }

links_graphics()

defines{
	"MAIN_ENABLED=1",
	"TOOLS_ENABLED=1",
	"IMGUI_API=ISM_API_IMPORT",
}

files{
	"%{wks.location}/demo/demo.premake5.lua",
	"%{wks.location}/assets/**.**",
	"%{wks.location}/platform/%{_TARGET_OS}/main_%{_TARGET_OS}.cpp",
}

srcdirs(
	"%{wks.location}/demo/"
)

postbuildcommands{
	--"{COPYFILE} %{wks.location}/assets/test.zip %{_BUILD_RES}",
}

filter{ "system:windows" }
	files{
		"%{wks.location}/platform/windows/ism.rc",
		"%{wks.location}/platform/windows/ism.ico",
	}
	postbuildcommands{
		"{COPYFILE} %{wks.location}/platform/windows/binaries.manifest %{_BUILD_BIN}",
		"{COPYFILE} %{wks.location}/platform/windows/ism.ico %{_BUILD_RES}",
		"{COPYFILE} %{wks.location}/assets/ism.png %{_BUILD_RES}",
	}
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- demo-CS
group			"Demo"
project			"demo-CS"
kind			"SharedLib"
language		"C#"
systemversion	"latest"
targetname		"%{prj.name}"
targetdir		"%{_BUILD_BIN}"
objdir			"%{_TEMPS}"
location		"%{_PROJECT}"
files			{ "%{wks.location}/demo/**.cs" }

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --