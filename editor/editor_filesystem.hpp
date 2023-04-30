#ifndef _PNU_EDITOR_FILESYSTEM_HPP_
#define _PNU_EDITOR_FILESYSTEM_HPP_

#include <editor/editor_panel.hpp>

namespace Pnu
{
	// filesystem component of the main editor
	class PNU_API EditorFileSystem : public EditorPanel
	{
		DEFINE_CLASS(EditorFileSystem, EditorPanel);

	public:
		EditorFileSystem();

		virtual ~EditorFileSystem();

		virtual void process(Duration const & delta_time) override;
	};
}

#endif // !_PNU_EDITOR_FILESYSTEM_HPP_
