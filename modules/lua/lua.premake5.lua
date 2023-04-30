-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- lua
group			"Modules"
project			"lua"
kind			"SharedLib"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
targetdir		"%{_BUILD_BIN}"
objdir			"%{_TEMPS}"
location		"%{_PROJECT}"

dependson{ "pneumatic", }

links{ "assimp%{LIB}", "freetype", "glfw", "imgui", "IrrXML", "pneumatic", "zip", "zlibstatic", }

defines{
	"LUA_BUILD_AS_DLL",
	"PN_MOD_API=PN_API_EXPORT",
}

includedirs{
	"%{wks.location}/thirdparty/lua/include/",
}

srcdirs(
	"%{wks.location}/modules/lua/"
)

files{
	"%{wks.location}/modules/lua/lua.premake5.lua",
	"%{wks.location}/thirdparty/lua/lapi.h", "%{wks.location}/thirdparty/lua/lauxlib.h", "%{wks.location}/thirdparty/lua/lcode.h", "%{wks.location}/thirdparty/lua/lctype.h",
	"%{wks.location}/thirdparty/lua/ldebug.h", "%{wks.location}/thirdparty/lua/ldo.h", "%{wks.location}/thirdparty/lua/lfunc.h", "%{wks.location}/thirdparty/lua/lgc.h",
	"%{wks.location}/thirdparty/lua/ljumptab.h", "%{wks.location}/thirdparty/lua/llex.h", "%{wks.location}/thirdparty/lua/llimits.h", "%{wks.location}/thirdparty/lua/lmem.h",
	"%{wks.location}/thirdparty/lua/lobject.h", "%{wks.location}/thirdparty/lua/lopcodes.h", "%{wks.location}/thirdparty/lua/lopnames.h", "%{wks.location}/thirdparty/lua/lparser.h",
	"%{wks.location}/thirdparty/lua/lprefix.h", "%{wks.location}/thirdparty/lua/lstate.h", "%{wks.location}/thirdparty/lua/lstring.h", "%{wks.location}/thirdparty/lua/ltable.h",
	"%{wks.location}/thirdparty/lua/ltests.h", "%{wks.location}/thirdparty/lua/ltm.h", "%{wks.location}/thirdparty/lua/luaconf.h", "%{wks.location}/thirdparty/lua/lualib.h",
	"%{wks.location}/thirdparty/lua/lundump.h", "%{wks.location}/thirdparty/lua/lvm.h", "%{wks.location}/thirdparty/lua/lzio.h", "%{wks.location}/thirdparty/lua/lapi.c",
	"%{wks.location}/thirdparty/lua/lauxlib.c", "%{wks.location}/thirdparty/lua/lbaselib.c", "%{wks.location}/thirdparty/lua/lcode.c", "%{wks.location}/thirdparty/lua/lcorolib.c",
	"%{wks.location}/thirdparty/lua/lctype.c", "%{wks.location}/thirdparty/lua/ldblib.c", "%{wks.location}/thirdparty/lua/ldebug.c", "%{wks.location}/thirdparty/lua/ldo.c",
	"%{wks.location}/thirdparty/lua/ldump.c", "%{wks.location}/thirdparty/lua/lfunc.c", "%{wks.location}/thirdparty/lua/lgc.c", "%{wks.location}/thirdparty/lua/linit.c",
	"%{wks.location}/thirdparty/lua/liolib.c", "%{wks.location}/thirdparty/lua/llex.c", "%{wks.location}/thirdparty/lua/lmathlib.c", "%{wks.location}/thirdparty/lua/lmem.c",
	"%{wks.location}/thirdparty/lua/loadlib.c", "%{wks.location}/thirdparty/lua/lobject.c", "%{wks.location}/thirdparty/lua/lopcodes.c", "%{wks.location}/thirdparty/lua/loslib.c",
	"%{wks.location}/thirdparty/lua/lparser.c", "%{wks.location}/thirdparty/lua/lstate.c", "%{wks.location}/thirdparty/lua/lstring.c", "%{wks.location}/thirdparty/lua/lstrlib.c",
	"%{wks.location}/thirdparty/lua/ltable.c", "%{wks.location}/thirdparty/lua/ltablib.c", "%{wks.location}/thirdparty/lua/ltests.c", "%{wks.location}/thirdparty/lua/ltm.c",
	"%{wks.location}/thirdparty/lua/lundump.c", "%{wks.location}/thirdparty/lua/lutf8lib.c", "%{wks.location}/thirdparty/lua/lvm.c", "%{wks.location}/thirdparty/lua/lzio.c",
}

postbuildcommands{
	"{COPYFILE} %{wks.location}/modules/lua/lua.ini %{_BUILD_CFG}",
}
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --