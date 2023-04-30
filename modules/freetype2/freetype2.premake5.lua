-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- freetype2
cpp_project_common("Modules", "freetype2", "Utility")

targetdir("%{_BUILD_BIN}")

files{
	"%{_MODULES}%{prj.name}/%{prj.name}.premake5.lua",
}

postbuildcommands{
	"{COPYFILE} %{_VENDOR}freetype%{LIB} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}freetype.pdb %{cfg.targetdir}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --