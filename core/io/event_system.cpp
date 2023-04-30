#include <core/io/event_system.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Event, t, TypeFlags_IsAbstract)
	{
		t.tp_bind = BIND_CLASS(Event, t)
		{
			return t
				.def("get_event_id", &Event::get_event_id)
				.def("__int__", &Event::operator EventID)
				.def("__eq__", &Event::operator==)
				.def("__ne__", &Event::operator!=)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(EventListener, t, TypeFlags_IsAbstract)
	{
		t.tp_bind = BIND_CLASS(EventListener, t)
		{
			return t
				.def("handle_event", &EventListener::handle_event)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(DummyListener, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(EventDelegate<Event>, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EventBus * EventBus::__singleton{};

	EMBED_CLASS(EventBus, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}