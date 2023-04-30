#ifndef _PN_EDITOR_VIEWPORT_HPP_
#define _PN_EDITOR_VIEWPORT_HPP_

#include <editor/editor_panel.hpp>

namespace pn
{
	// viewport component of the main editor
	class PN_API EditorViewport : public EditorPanel
	{
		DEFINE_CLASS(EditorViewport, EditorPanel);

		friend class EditorNode;

		RID m_viewport{};

		RID m_main_texture{};

		Mat4 m_camera_view{}, m_camera_proj{};

		bool m_is_dragging_view{};

		bool	m_grid_enabled;
		Mat4	m_grid_matrix;
		f32		m_grid_size;

	public:
		EditorViewport();

		virtual ~EditorViewport();

		virtual void process(Duration const & delta_time) override;
	};
}

#endif // !_PN_EDITOR_VIEWPORT_HPP_
