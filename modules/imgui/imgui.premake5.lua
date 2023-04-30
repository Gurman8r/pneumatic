-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- imgui
cpp_project_common("Modules", "imgui", "SharedLib")

targetdir("%{_BUILD_BIN}")

manifest("imgui")

dependson{ "glfw", }

links{ "glfw", }

links_graphics()

defines{
	"IMGUI_API=__declspec(dllexport)",
	"IMGUI_IMPL_OPENGL_LOADER_GLEW",
}

includedirs{
	"%{_SLN}thirdparty",
	"%{_THIRDPARTY}glfw/glfw/include",
	"%{_THIRDPARTY}imgui/imgui",
	"%{_THIRDPARTY}imgui/imgui-node-editor",
}

files{
	"%{_MODULES}%{prj.name}/%{prj.name}.premake5.lua",
	
	"%{_THIRDPARTY}imgui/*.h",
	"%{_THIRDPARTY}imgui/*.cpp",

	"%{_THIRDPARTY}imgui/backends/imgui_impl_glfw.h",
	"%{_THIRDPARTY}imgui/backends/imgui_impl_glfw.cpp",
	"%{_THIRDPARTY}imgui/backends/imgui_impl_opengl3.h",
	"%{_THIRDPARTY}imgui/backends/imgui_impl_opengl3.cpp",
	
	"%{_THIRDPARTY}imgui_club/imgui_memory_editor/imgui_memory_editor.h",
	"%{_THIRDPARTY}ImGuizmo/ImGuizmo.h",
	"%{_THIRDPARTY}ImGuizmo/ImGuizmo.cpp",
	--"%{_THIRDPARTY}ImGuiColorTextEdit/*.h",
	--"%{_THIRDPARTY}ImGuiColorTextEdit/*.cpp",
	--"%{_THIRDPARTY}imgui-node-editor/*.h",
	--"%{_THIRDPARTY}imgui-node-editor/*.cpp",
	--"%{_THIRDPARTY}implot/**.h",
	--"%{_THIRDPARTY}implot/**.cpp",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --