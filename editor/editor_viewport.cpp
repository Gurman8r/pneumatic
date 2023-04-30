#include <editor/editor_viewport.hpp>
#include <editor/editor_node.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(EditorViewport, t) {}

	EditorViewport::EditorViewport()
		: EditorPanel{ "Viewport##Editor", true, ImGuiWindowFlags_NoScrollbar }
		, m_main_texture{}
		, m_grid_enabled{ true }
		, m_grid_matrix{ identity_v<Mat4> }
		, m_grid_size{ 100.f }
	{
	}

	EditorViewport::~EditorViewport()
	{
	}

	void EditorViewport::process(Duration const & delta_time)
	{
		if (!is_open()) { return; }
		ImGuiViewport * const main_viewport{ ImGui::GetMainViewport() };
		ImGui::SetNextWindowPos(main_viewport->GetWorkPos() + Vec2{ 32, 32 }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ 1280.f, 720.f }, ImGuiCond_FirstUseEver);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowPadding, { 0.f, 0.f });
		bool open; EDITOR_PANEL_SCOPE(open);
		ImGui::PopStyleVar(1);
		if (!open) { return; }

		ImGuiWindow * const window{ get_window() };
		ImRect const view_rect{ window->InnerRect };

		if (ImGui::ItemAdd(view_rect, NULL)) {
			void * texture_handle{ RENDERING_DEVICE->texture_get_handle(m_main_texture) };
			window->DrawList->AddImage(texture_handle, view_rect.Min, view_rect.Max, { 0, 1 }, { 1, 0 });
		}

		bool const nav_enabled{ window == ImGui::GetCurrentContext()->NavWindow };
		m_is_dragging_view = nav_enabled && !ImGuizmo::IsUsing() && ImGui::IsItemHovered() && ImGui::IsMouseDragging(0);

		// gizmos
		ImGuizmo::SetDrawlist(window->DrawList);
		ImGuizmo::SetRect(view_rect.Min.x, view_rect.Min.y, view_rect.GetWidth(), view_rect.GetHeight());
		if (m_grid_enabled) { ImGuizmo::DrawGrid(m_camera_view, m_camera_proj, m_grid_matrix, m_grid_size); }
		//if (0 < m_object_count) { ImGuizmo::DrawCubes(view_matrix, proj_matrix, &m_object_matrix[0][0], m_object_count); }
		//for (i32 i = 0; i < m_object_count; ++i) {
		//	ImGuizmo::SetID(i);
		//	if (m_xeditor.Manipulate(view_matrix, proj_matrix, m_object_matrix[i])) { m_object_index = i; }
		//}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}