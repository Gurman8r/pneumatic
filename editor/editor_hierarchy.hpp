#ifndef _PN_EDITOR_HIERARCHY_HPP_
#define _PN_EDITOR_HIERARCHY_HPP_

#include <editor/editor_panel.hpp>

namespace pn
{
	// hierarchy component of the main editor
	class PN_API EditorHierarchy : public EditorPanel
	{
		DEFINE_CLASS(EditorHierarchy, EditorPanel);

	public:
		EditorHierarchy();

		virtual ~EditorHierarchy();

	public:
		virtual void process(Duration const & delta_time) override;
	};
}

#endif // !_PN_EDITOR_HIERARCHY_HPP_
