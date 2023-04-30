-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- assimp
cpp_project_common("Modules", "assimp", "Utility", "%{_BUILD_BIN}")

manifest("assimp")

includedirs{ "%{_THIRDPARTY}assimp/include", }

files{ "%{_MODULES}assimp/assimp.premake5.lua", }

postbuildcommands{
	"{COPYFILE} %{_VENDOR}assimp%{DLL} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}assimp%{LIB} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}IrrXML%{LIB} %{cfg.targetdir}",
	"{COPYFILE} %{_VENDOR}zlibstatic%{LIB} %{cfg.targetdir}",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --