#ifndef _PN_EDITOR_LOG_HPP_
#define _PN_EDITOR_LOG_HPP_

#include <editor/editor_panel.hpp>

namespace pn
{
	// log component of the main editor
	class PN_API EditorLog : public EditorPanel
	{
		DEFINE_CLASS(EditorLog, EditorPanel);

	public:
		EditorLog();

		virtual ~EditorLog();

	public:
		virtual void process(Duration const & delta_time) override;
	};
}

#endif // !_PN_EDITOR_LOG_HPP_
