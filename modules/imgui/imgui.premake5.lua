-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- imgui
group			"Modules"
project			"imgui"
kind			"SharedLib"
language		"C++"
cppdialect 		"C++17"
systemversion	"latest"
staticruntime	"Off"
targetname		"%{prj.name}"
targetdir		"%{_BUILD_BIN}"
objdir			"%{_TEMPS}"
location		"%{_PROJECT}"

manifest("imgui")

dependson{ "glfw", }

links{ "glfw", }

links_graphics()

defines{
	"IMGUI_API=__declspec(dllexport)",
	"IMGUI_IMPL_OPENGL_LOADER_GLEW",
}

includedirs{
	"%{wks.location}/thirdparty",
	"%{wks.location}/thirdparty/glfw/glfw/include",
	"%{wks.location}/thirdparty/imgui/imgui",
	"%{wks.location}/thirdparty/imgui/imgui-node-editor",
}

files{
	"%{wks.location}/modules/%{prj.name}/%{prj.name}.premake5.lua",
	
	"%{wks.location}/thirdparty/imgui/*.h",
	"%{wks.location}/thirdparty/imgui/*.cpp",

	"%{wks.location}/thirdparty/imgui/backends/imgui_impl_glfw.h",
	"%{wks.location}/thirdparty/imgui/backends/imgui_impl_glfw.cpp",
	"%{wks.location}/thirdparty/imgui/backends/imgui_impl_opengl3.h",
	"%{wks.location}/thirdparty/imgui/backends/imgui_impl_opengl3.cpp",
	
	"%{wks.location}/thirdparty/imgui_club/imgui_memory_editor/imgui_memory_editor.h",
	"%{wks.location}/thirdparty/ImGuizmo/ImGuizmo.h",
	"%{wks.location}/thirdparty/ImGuizmo/ImGuizmo.cpp",
	--"%{wks.location}/thirdparty/ImGuiColorTextEdit/*.h",
	--"%{wks.location}/thirdparty/ImGuiColorTextEdit/*.cpp",
	--"%{wks.location}/thirdparty/imgui-node-editor/*.h",
	--"%{wks.location}/thirdparty/imgui-node-editor/*.cpp",
	--"%{wks.location}/thirdparty/implot/**.h",
	--"%{wks.location}/thirdparty/implot/**.cpp",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --