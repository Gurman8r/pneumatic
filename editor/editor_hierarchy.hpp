#ifndef _PNU_EDITOR_HIERARCHY_HPP_
#define _PNU_EDITOR_HIERARCHY_HPP_

#include <editor/editor_panel.hpp>

namespace Pnu
{
	// hierarchy component of the main editor
	class PNU_API EditorHierarchy : public EditorPanel
	{
		DEFINE_CLASS(EditorHierarchy, EditorPanel);

	public:
		EditorHierarchy();

		virtual ~EditorHierarchy();

	public:
		virtual void process(Duration const & delta_time) override;
	};
}

#endif // !_PNU_EDITOR_HIERARCHY_HPP_
