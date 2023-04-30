#ifndef _PN_MAIN_LOOP_HPP_
#define _PN_MAIN_LOOP_HPP_

#include <core/object/class.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PN_API MainLoop : public Object
	{
		DEFINE_CLASS(MainLoop, Object);

		OBJ m_script{};

	public:
		enum : Notification_
		{
			Notification_MemoryWarning = 2001,
			Notification_Crash,
			Notification_ApplicationResumed,
			Notification_ApplicationPaused,
			Notification_ApplicationFocusIn,
			Notification_ApplicationFocusOut,
		};

		MainLoop() noexcept {}
		virtual ~MainLoop() noexcept override {}

		virtual void initialize();
		virtual bool process(Duration const & dt);
		virtual void finalize();

		void set_startup_script(OBJ const & value) { m_script = value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_MAIN_LOOP_HPP_
