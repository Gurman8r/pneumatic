#include <core/os/main_loop.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(MainLoop, t)
	{
		t.tp_bind = BIND_CLASS(MainLoop, t)
		{
			return t
				.def("initialize", &MainLoop::initialize)
				.def("process", &MainLoop::process)
				.def("finalize", &MainLoop::finalize)
				.def("set_startup_script", &MainLoop::set_startup_script)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void MainLoop::initialize()
	{
		STR_IDENTIFIER(_initialize);

		if (m_script)
		{
			if (OBJ callback{ getattr(m_script, &ID__initialize) })
			{
				call_object(callback);
			}
		}
	}

	bool MainLoop::process(Duration const & dt)
	{
		STR_IDENTIFIER(_process);

		bool should_close{};

		if (m_script)
		{
			if (OBJ callback{ getattr(m_script, &ID__process) })
			{
				static FloatObject arg0; arg0 = dt.count();
				static ListObject args{ &arg0, };
				OBJ result{ call_object(callback, &args) };
				if (result && result.cast<bool>())
				{
					should_close = true;
				}
			}
		}

		return should_close;
	}

	void MainLoop::finalize()
	{
		STR_IDENTIFIER(_finalize);

		if (m_script)
		{
			if (OBJ callback{ getattr(m_script, &ID__finalize) })
			{
				call_object(callback);
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}