-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

include "./misc/premake5_solution_items.lua"

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

newoption{
	trigger		= "gfxapi",
	value		= "api",
	description	= "graphics api",
	default		= "opengl",
	category	= "Build Options",
	allowed		= { { "opengl", "OpenGL" }, { "vulkan", "Vulkan" }, { "directx", "DirectX" }, }
}

newoption{
	trigger		= "glapi",
	value		= "api",
	description	= "opengl loader",
	default		= "glew",
	category	= "Build Options",
	allowed		= { { "glew", "GLEW" }, { "glad", "GLAD" }, }
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

_BUILD="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/"
_BUILD_BIN="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/binaries/"
_BUILD_CFG="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/config/"
_BUILD_DAT="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/data/"
_BUILD_RES="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/resources/"
_BUILD_USR="%{wks.location}/build_%{_TARGET_OS}_%{cfg.platform}_%{cfg.buildcfg}/user/"
_MODULES="%{wks.location}/modules/"
_PROJECT="%{wks.location}/workspace/%{_ACTION}/%{prj.name}/"
_TEMPS="%{wks.location}/temporary/%{_ACTION}/%{_TARGET_OS}/"
_THIRDPARTY="%{wks.location}/thirdparty/"
_VENDOR="%{wks.location}/misc/%{_TARGET_OS}/vendor/%{cfg.platform}/%{cfg.buildcfg}/"

-- platform specific extensions
LIB=".a" DLL=".so" EXE=""
if _TARGET_OS=="windows" then
LIB=".lib" DLL=".dll" EXE=".exe"
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

function srcdirs(...)
	local arg = { ... }
	for i, v in ipairs(arg) do
		files{
			""..v.."/**.hpp",
			""..v.."/**.cpp",
			""..v.."/**.h",
			""..v.."/**.c",
			""..v.."/**.inl",
			""..v.."/**.ini",
			""..v.."/**.cfg",
		}
	end
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- graphics linker settings
function links_graphics()
	filter{ "language:C++", "options:gfxapi=opengl" } links{ "opengl32" } defines{ "OPENGL_ENABLED=1" }
	filter{ "language:C++", "options:gfxapi=opengl", "options:glapi=glad" } dependson{ "glad" } links{ "glad" } defines{ "OPENGL_LOADER_GLAD=1" }
	filter{ "language:C++", "options:gfxapi=opengl", "options:glapi=glew" } dependson{ "glew" } defines{ "OPENGL_LOADER_GLEW=1" }
	filter{ "language:C++", "options:gfxapi=opengl", "options:glapi=glew", "configurations:Debug" } links{ "glew32d" }
	filter{ "language:C++", "options:gfxapi=opengl", "options:glapi=glew", "configurations:Release" } links{ "glew32" }
	filter{ "language:C++", "options:gfxapi=vulkan" } -- WIP --
	filter{ "language:C++", "options:gfxapi=directx" } -- WIP --
	filter{}
end

-- windows linker settings
function links_win32()
	filter{ "language:C++", "system:windows" } links{ "dwmapi", } buildoptions{ "/bigobj" } defines{ "_CRT_SECURE_NO_WARNINGS" } undefines{ "NDEBUG" }
	filter{ "language:C++", "system:windows", "configurations:Debug" } linkoptions{ "/NODEFAULTLIB:MSVCRT.lib", "/NODEFAULTLIB:LIBCMT.lib", "/NODEFAULTLIB:LIBCMTD.lib" }
	filter{ "language:C++", "system:windows", "configurations:Release" } linkoptions{ "/NODEFAULTLIB:LIBCMT.lib" }
	filter{}
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

_MANIFEST={}

function manifest(...)
	local args={...}
	for i, v in ipairs(args) do
		table.insert(_MANIFEST, v)
	end
end

function generate_manifest(path)
	local text="<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
	text=text.."<assembly xmlns=\"urn:schemas-microsoft-com:asm.v1\" manifestVersion=\"1.0\">\n"
	text=text.."<assemblyIdentity type=\"win32\" name=\"binaries\" version=\"1.0.0.0\"/>\n"
	for i, v in ipairs(_MANIFEST) do
		text=text..string.format("\t<file name=\"%s%s\"/>\n", v, DLL)
	end
	text=text.."</assembly>"
	io.writefile(path, text);
end

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --