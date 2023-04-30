#ifndef _PNU_EVENT_SYSTEM_HPP_
#define _PNU_EVENT_SYSTEM_HPP_

#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Event;

	template <class> class EventClass;

	class EventListener;

	class DummyListener;

	template <class> class EventDelegate;

	class EventBus;

	using EventID = size_t;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event
	class PNU_API Event : public Object
	{
		DEFINE_CLASS(Event, Object);

	protected:
		Event() noexcept = default;

	public:
		enum : EventID { ID = static_cast<EventID>(-1) };

		virtual ~Event() noexcept override = default;

		virtual EventID get_event_id() const = 0;

		operator EventID() const { return get_event_id(); }

		bool operator==(EventID const id) const noexcept { return id == get_event_id(); }

		bool operator!=(EventID const id) const noexcept { return id != get_event_id(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// event class
#define EVENT_CLASS(m_class, m_inherits)										\
private:																		\
	static_assert(std::is_base_of_v<Pnu::Event, m_inherits>);					\
																				\
	DEFINE_CLASS(m_class, m_inherits);											\
																				\
public:																			\
	enum : Pnu::EventID { ID = m_class::get_class_static().hash_code() };		\
																				\
	virtual Pnu::EventID get_event_id() const override { return m_class::ID; }	\
																				\
private:

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// listener
	class PNU_API EventListener : public Object
	{
		DEFINE_CLASS(EventListener, Object);

		friend struct Less<EventListener *>;

		EventBus * const m_bus;
		
		i32 const m_dispatch_order;

	public:
		virtual ~EventListener() noexcept override { unsubscribe(); }

		auto get_bus() const noexcept -> EventBus * { return m_bus; }

	protected:
		friend class EventBus;

		explicit EventListener(EventBus * bus = nullptr) noexcept;

		template <class Event0, class ... Events
		> void subscribe() noexcept;

		template <class ... Events
		> void unsubscribe() noexcept;

		virtual void handle_event(Event const &) = 0; // <- HANDLE EVENT
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// dummy listener
	class PNU_API DummyListener final : public EventListener
	{
		DEFINE_CLASS(DummyListener, EventListener);

		std::function<void(Event const &)> m_callback{};

	public:
		template <class Fn
		> DummyListener(EventBus * bus, Fn && fn) noexcept : EventListener{ bus }, m_callback{ FWD(fn) } {}

		auto get_callback() const noexcept -> auto const & { return m_callback; }

		template <class Fn
		> void set_callback(Fn && fn) noexcept { m_callback = FWD(fn); }

		using EventListener::subscribe;

		using EventListener::unsubscribe;

	protected:
		void handle_event(Event const & event) final { m_callback(event); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// delegate base
	template <> class PNU_API EventDelegate<Event> : public EventListener
	{
		DEFINE_CLASS(EventDelegate<Event>, EventListener);

	public:
		enum : EventID { ID = Event::ID };

		using Callback = typename void;

		virtual ~EventDelegate() noexcept override = default;

	protected:
		explicit EventDelegate(EventBus * bus) noexcept : EventListener{ bus } {}

		virtual void handle_event(Event const & event) override = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// delegate
	template <class Ev> class EventDelegate final : public EventDelegate<Event>
	{
		Vector<std::function<void(Ev const &)>> m_callbacks{};

	public:
		enum : EventID { ID = Ev::ID };

		using Callback = typename decltype(m_callbacks)::value_type;

		explicit EventDelegate(EventBus * bus) noexcept : EventDelegate<Event>{ bus } { this->subscribe<Ev>(); }

		auto operator[](size_t i) noexcept -> Callback & { return m_callbacks[i]; }

		auto operator[](size_t i) const noexcept -> Callback const & { return m_callbacks[i]; }

		void clear() noexcept { m_callbacks.clear(); }

		void erase(size_t i, size_t n = 1) { m_callbacks.erase(m_callbacks.begin() + i, m_callbacks.begin() + i + n); }

		void reserve(size_t count) { m_callbacks.reserve(count); }
		
		template <class Fn
		> auto add(Fn && fn) noexcept -> Callback & { return m_callbacks.emplace_back(VALIDATE(Callback{ FWD(fn) })); }

		template <class Fn
		> auto insert(size_t i, Fn && fn) noexcept -> Callback & { return m_callbacks.emplace(m_callbacks.begin() + i, VALIDATE(Callback{ FWD(fn) })); }

		template <class Fn
		> auto operator+=(Fn && fn) noexcept -> EventDelegate & { return this->add(FWD(fn)), (*this); }

	protected:
		void handle_event(Event const & ev) final
		{
			ASSERT((EventID)ev == ID);

			for (Callback const & callback : m_callbacks)
			{
				callback(static_cast<Ev const &>(ev));
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct Less<EventListener *>
	{
		bool operator()(EventListener * a, EventListener * b) const noexcept
		{
			return (a != b) && ((a && b) ? (a->m_dispatch_order < b->m_dispatch_order) : (a < b));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// bus
	class PNU_API EventBus : public Object
	{
		DEFINE_CLASS(EventBus, Object);

		static EventBus * __singleton;

		FlatMap<EventID, FlatSet<EventListener *>> m_listeners{};
		FlatMap<EventID, Ref<EventDelegate<Event>>> m_delegates{};
		Vector<Ref<DummyListener>> m_dummies{};

	protected:
		friend class EventListener;
		i32 m_next_index{};

	public:
		EventBus() noexcept { SINGLETON_CTOR(); }
		virtual ~EventBus() noexcept override { SINGLETON_DTOR(); }
		FORCE_INLINE static EventBus * get_singleton() noexcept { return __singleton; }

	public:
		void fire_event(Event const & value) noexcept
		{
			if (auto const group{ m_listeners.find((EventID)value) })
			{
				for (EventListener * const listener : (*group->second))
				{
					VALIDATE(listener)->handle_event(value);
				}
			}
		}

		template <class Ev> bool add_listener(EventListener * value) noexcept
		{
			return value && m_listeners[Ev::ID].insert(value).second;
		}

		template <class Ev> void remove_listener(EventListener * value) noexcept
		{
			if (auto const group{ m_listeners.find(Ev::ID) })
			{
				if (auto const it{ group->second->find(value) }; it != group->second->end())
				{
					group->second->erase(it);
				}
			}
		}

		void remove_listener(EventListener * value) noexcept
		{
			m_listeners.for_each([&](auto, FlatSet<EventListener *> & group) noexcept
			{
				if (auto const it{ group.find(value) }; it != group.end())
				{
					group.erase(it);
				}
			});
		}

	public:
		NODISCARD Vector<Ref<DummyListener>> const & get_all_dummies() const noexcept { return m_dummies; }

		template <class ... Events, class Fn
		> Ref<DummyListener> add_dummy(Fn && fn) noexcept
		{
			Ref<DummyListener> dummy{ memnew(DummyListener(this, FWD(fn))) };

			if constexpr (0 < sizeof...(Events)) { dummy->subscribe<Events...>(); }

			return m_dummies.emplace_back(dummy);
		}

		auto remove_dummy(Ref<DummyListener> const & value) noexcept
		{
			if (auto const it{ m_dummies.find(value) }; it != m_dummies.end())
			{
				return m_dummies.erase(it);
			}
			else
			{
				return it;
			}
		}

		void remove_all_dummys() noexcept
		{
			while (!m_dummies.empty()) { m_dummies.pop_back(); }
		}

	public:
		/* DELEGATES */

		NODISCARD auto get_all_delegates() const noexcept { return m_delegates; }

		template <class Ev
		> EventDelegate<Ev> & get_delegate() noexcept
		{
			return **((Ref<EventDelegate<Ev>> &)m_delegates.find_or_add_fn(Ev::ID, [&
			]() noexcept { return memnew(EventDelegate<Ev>(this)); }));
		}

		template <class ... Events
		> void remove_delegates() noexcept
		{
			if constexpr (0 < sizeof...(Events))
			{
				mpl::for_types<Events...>([&](auto tag) noexcept
				{
					using T = TAG_TYPE(tag);

					if (auto const it{ m_delegates.find(hashof_v<T>) })
					{
						m_delegates.erase(it);
					}
				});
			}
			else
			{
				m_delegates.clear();
			}
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_WRAPPER(EventBus, get_event_bus);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline EventListener::EventListener(EventBus * bus) noexcept
		: m_bus{ bus ? bus : VALIDATE(get_event_bus()) }
		, m_dispatch_order{ ++m_bus->m_next_index }
	{
	}

	template <class Event0, class ... Events
	> void EventListener::subscribe() noexcept
	{
		ASSERT(m_bus);

		mpl::for_types<Event0, Events...>([&](auto tag) noexcept
		{
			m_bus->add_listener<TAG_TYPE(tag)>(this);
		});
	}

	template <class ... Events
	> void EventListener::unsubscribe() noexcept
	{
		ASSERT(m_bus);

		if constexpr (0 < sizeof...(Events))
		{
			mpl::for_types<Events...>([&](auto tag) noexcept
			{
				m_bus->remove_listener<TAG_TYPE(tag)>(this);
			});
		}
		else
		{
			m_bus->remove_listener(this); // remove from all events
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_EVENT_SYSTEM_HPP_
