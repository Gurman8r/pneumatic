#ifndef _PNU_MAIN_LOOP_HPP_
#define _PNU_MAIN_LOOP_HPP_

#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PNU_API MainLoop : public Object
	{
		DEFINE_CLASS(MainLoop, Object);

		ObjectRef m_script{};

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

		void set_startup_script(ObjectRef const & value) { m_script = value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_MAIN_LOOP_HPP_
