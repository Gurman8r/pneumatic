-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- lua
cpp_project_common("Modules", "lua", "SharedLib")

targetdir("%{_BUILD_BIN}")

dependson{ "pneumatic", }

links{ "assimp%{LIB}", "freetype", "glfw", "imgui", "IrrXML", "pneumatic", "zip", "zlibstatic", }

defines{
	"LUA_BUILD_AS_DLL",
	"PNU_MOD_API=PNU_API_EXPORT",
}

includedirs{
	"%{_THIRDPARTY}lua/include/",
}

files{
	"%{_MODULES}lua/lua.premake5.lua",
	"%{_THIRDPARTY}lua/lapi.h", "%{_THIRDPARTY}lua/lauxlib.h", "%{_THIRDPARTY}lua/lcode.h", "%{_THIRDPARTY}lua/lctype.h",
	"%{_THIRDPARTY}lua/ldebug.h", "%{_THIRDPARTY}lua/ldo.h", "%{_THIRDPARTY}lua/lfunc.h", "%{_THIRDPARTY}lua/lgc.h",
	"%{_THIRDPARTY}lua/ljumptab.h", "%{_THIRDPARTY}lua/llex.h", "%{_THIRDPARTY}lua/llimits.h", "%{_THIRDPARTY}lua/lmem.h",
	"%{_THIRDPARTY}lua/lobject.h", "%{_THIRDPARTY}lua/lopcodes.h", "%{_THIRDPARTY}lua/lopnames.h", "%{_THIRDPARTY}lua/lparser.h",
	"%{_THIRDPARTY}lua/lprefix.h", "%{_THIRDPARTY}lua/lstate.h", "%{_THIRDPARTY}lua/lstring.h", "%{_THIRDPARTY}lua/ltable.h",
	"%{_THIRDPARTY}lua/ltests.h", "%{_THIRDPARTY}lua/ltm.h", "%{_THIRDPARTY}lua/luaconf.h", "%{_THIRDPARTY}lua/lualib.h",
	"%{_THIRDPARTY}lua/lundump.h", "%{_THIRDPARTY}lua/lvm.h", "%{_THIRDPARTY}lua/lzio.h", "%{_THIRDPARTY}lua/lapi.c",
	"%{_THIRDPARTY}lua/lauxlib.c", "%{_THIRDPARTY}lua/lbaselib.c", "%{_THIRDPARTY}lua/lcode.c", "%{_THIRDPARTY}lua/lcorolib.c",
	"%{_THIRDPARTY}lua/lctype.c", "%{_THIRDPARTY}lua/ldblib.c", "%{_THIRDPARTY}lua/ldebug.c", "%{_THIRDPARTY}lua/ldo.c",
	"%{_THIRDPARTY}lua/ldump.c", "%{_THIRDPARTY}lua/lfunc.c", "%{_THIRDPARTY}lua/lgc.c", "%{_THIRDPARTY}lua/linit.c",
	"%{_THIRDPARTY}lua/liolib.c", "%{_THIRDPARTY}lua/llex.c", "%{_THIRDPARTY}lua/lmathlib.c", "%{_THIRDPARTY}lua/lmem.c",
	"%{_THIRDPARTY}lua/loadlib.c", "%{_THIRDPARTY}lua/lobject.c", "%{_THIRDPARTY}lua/lopcodes.c", "%{_THIRDPARTY}lua/loslib.c",
	"%{_THIRDPARTY}lua/lparser.c", "%{_THIRDPARTY}lua/lstate.c", "%{_THIRDPARTY}lua/lstring.c", "%{_THIRDPARTY}lua/lstrlib.c",
	"%{_THIRDPARTY}lua/ltable.c", "%{_THIRDPARTY}lua/ltablib.c", "%{_THIRDPARTY}lua/ltests.c", "%{_THIRDPARTY}lua/ltm.c",
	"%{_THIRDPARTY}lua/lundump.c", "%{_THIRDPARTY}lua/lutf8lib.c", "%{_THIRDPARTY}lua/lvm.c", "%{_THIRDPARTY}lua/lzio.c",
}

postbuildcommands{
	"{COPYFILE} %{_MODULES}lua/lua.ini %{_BUILD_CFG}",
}
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --