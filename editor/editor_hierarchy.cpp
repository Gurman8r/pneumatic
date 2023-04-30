#include <editor/editor_hierarchy.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(EditorHierarchy, t) {}

	EditorHierarchy::EditorHierarchy() : EditorPanel{ "Hierarchy##Editor" }
	{
	}

	EditorHierarchy::~EditorHierarchy()
	{
	}

	void EditorHierarchy::process(Duration const & delta_time)
	{
		if (!is_open()) { return; }
		ImGuiViewport * const main_viewport{ ImGui::GetMainViewport() };
		ImGui::SetNextWindowPos(main_viewport->GetWorkPos() + Vec2{ 32, 32 }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ 320.f, 180.f }, ImGuiCond_FirstUseEver);
		bool open; EDITOR_PANEL_SCOPE(open);
		if (!open) { return; }

		ImGui::Text("editor hierarchy goes here");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
