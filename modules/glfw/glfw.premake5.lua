-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- glfw
group			"Modules"
project			"glfw"
kind			"SharedLib"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
targetdir		"%{_BUILD_BIN}"
objdir			"%{_TEMPS}"
location		"%{_PROJECT}"

manifest("glfw")

defines{ "_GLFW_BUILD_DLL", }

undefines{ "__NULL_IMPORT_DESCRIPTOR", }

links_graphics()

files{
	"%{wks.location}/modules/%{prj.name}/%{prj.name}.premake5.lua",
	
	"%{wks.location}/thirdparty/glfw/include/glfw/**.h",
	"%{wks.location}/thirdparty/glfw/src/context.c", 
	"%{wks.location}/thirdparty/glfw/src/egl_context.c",
	"%{wks.location}/thirdparty/glfw/src/egl_context.h",
	"%{wks.location}/thirdparty/glfw/src/init.c", 
	"%{wks.location}/thirdparty/glfw/src/input.c", 
	"%{wks.location}/thirdparty/glfw/src/internal.h", 
	"%{wks.location}/thirdparty/glfw/src/mappings.h", 
	"%{wks.location}/thirdparty/glfw/src/monitor.c", 
	"%{wks.location}/thirdparty/glfw/src/osmesa_context.c",
	"%{wks.location}/thirdparty/glfw/src/osmesa_context.h",
	"%{wks.location}/thirdparty/glfw/src/vulkan.c",
	"%{wks.location}/thirdparty/glfw/src/wgl_context.c",
	"%{wks.location}/thirdparty/glfw/src/wgl_context.h",
	"%{wks.location}/thirdparty/glfw/src/window.c",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

filter{ "system:windows" }
	defines{ "_GLFW_WIN32=1", }
	files{ "%{wks.location}/thirdparty/glfw/src/win32_**.c", }
filter{}
	
-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --