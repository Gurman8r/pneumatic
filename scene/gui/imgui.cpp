#include <scene/gui/imgui.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if SYSTEM_WINDOWS
#include <glfw/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#define IMGUI_PLATFORM_INIT() ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true)
#define IMGUI_PLATFORM_SHUTDOWN() ImGui_ImplGlfw_Shutdown()
#define IMGUI_PLATFORM_NEWFRAME() ImGui_ImplGlfw_NewFrame()

#else
#error "imgui platform implementation undefined"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if OPENGL_ENABLED
#include <imgui/backends/imgui_impl_opengl3.h>
#define IMGUI_RENDERER_INIT() ImGui_ImplOpenGL3_Init("#version 130")
#define IMGUI_RENDERER_SHUTDOWN() ImGui_ImplOpenGL3_Shutdown()
#define IMGUI_RENDERER_NEWFRAME() ImGui_ImplOpenGL3_NewFrame()
#define IMGUI_RENDER_DRAW_DATA(draw_data) ImGui_ImplOpenGL3_RenderDrawData(draw_data)

#else
#error "imgui renderer implementation undefined"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace Pnu
{
	ImGuiContext * ImGui_Initialize()
	{
		ImGuiContext * ctx{ ImGui::CreateContext() };
		ctx->IO.LogFilename = nullptr;
		ctx->IO.IniFilename = nullptr;
		ctx->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ctx->IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ctx->IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ASSERT(IMGUI_PLATFORM_INIT());
		ASSERT(IMGUI_RENDERER_INIT());
		return ctx;
	}

	void ImGui_Finalize(ImGuiContext * ctx)
	{
		IMGUI_RENDERER_SHUTDOWN();
		IMGUI_PLATFORM_SHUTDOWN();
		if (ctx) { ImGui::DestroyContext(ctx); }
	}

	void ImGui_BeginFrame(ImGuiContext * ctx)
	{
		IMGUI_RENDERER_NEWFRAME();
		IMGUI_PLATFORM_NEWFRAME();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGui_RenderDrawData(ImDrawData * draw_data)
	{
		IMGUI_RENDER_DRAW_DATA(draw_data);
	}

	void ImGui_EndFrame(ImGuiContext * ctx)
	{
		if (ctx->IO.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */