#ifndef _PNU_EDITOR_LOG_HPP_
#define _PNU_EDITOR_LOG_HPP_

#include <editor/editor_panel.hpp>

namespace Pnu
{
	// log component of the main editor
	class PNU_API EditorLog : public EditorPanel
	{
		DEFINE_CLASS(EditorLog, EditorPanel);

	public:
		EditorLog();

		virtual ~EditorLog();

	public:
		virtual void process(Duration const & delta_time) override;
	};
}

#endif // !_PNU_EDITOR_LOG_HPP_
