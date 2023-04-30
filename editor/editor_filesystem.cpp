#include <editor/editor_filesystem.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(EditorFileSystem, t) {}

	EditorFileSystem::EditorFileSystem() : EditorPanel{ "FileSystem##Editor" }
	{
	}

	EditorFileSystem::~EditorFileSystem()
	{
	}

	void EditorFileSystem::process(Duration const & delta_time)
	{
		if (!is_open()) { return; }
		ImGuiViewport * const main_viewport{ ImGui::GetMainViewport() };
		ImGui::SetNextWindowPos(main_viewport->GetWorkPos() + Vec2{ 32, 32 }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ 320.f, 180.f }, ImGuiCond_FirstUseEver);
		bool open; EDITOR_PANEL_SCOPE(open);
		if (!open) { return; }

		ImGui::Text("editor filesystem goes here");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
