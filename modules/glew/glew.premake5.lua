-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- glew
cpp_project_common("Modules", "glew", "Utility")

targetdir("%{_BUILD_BIN}")

manifest("glew32", "glew32d")

files{
	"%{_MODULES}%{prj.name}/%{prj.name}.premake5.lua",
}

filter{ "configurations:Debug" } postbuildcommands{ "{COPYFILE} %{_VENDOR}glew32d%{DLL} %{cfg.targetdir}", "{COPYFILE} %{_VENDOR}glew32d%{LIB} %{cfg.targetdir}", }
filter{ "configurations:Release" } postbuildcommands{ "{COPYFILE} %{_VENDOR}glew32%{DLL} %{cfg.targetdir}", "{COPYFILE} %{_VENDOR}glew32%{LIB} %{cfg.targetdir}", }
filter{}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --