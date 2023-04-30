#ifndef _PN_EDITOR_FILESYSTEM_HPP_
#define _PN_EDITOR_FILESYSTEM_HPP_

#include <editor/editor_panel.hpp>

namespace pn
{
	// filesystem component of the main editor
	class PN_API EditorFileSystem : public EditorPanel
	{
		DEFINE_CLASS(EditorFileSystem, EditorPanel);

	public:
		EditorFileSystem();

		virtual ~EditorFileSystem();

		virtual void process(Duration const & delta_time) override;
	};
}

#endif // !_PN_EDITOR_FILESYSTEM_HPP_
