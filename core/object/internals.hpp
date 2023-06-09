#ifndef _PNU_INTERNALS_HPP_
#define _PNU_INTERNALS_HPP_

#include <core/object/builtins/base_object.hpp>

namespace Pnu
{
	// object system internals
	class PNU_API Internals final
	{
		static Internals * __singleton;

		Vector<ObjectRef> m_loader_stack; // loader life support stack
		ObjectRef m_modules; // module dictionary
		Batch<size_t, String, TypeObject *> m_classes; // class database

	public:
		Internals();
		~Internals();
		FORCE_INLINE static Internals * get_singleton() noexcept { return __singleton; }

	public:
		NODISCARD Vector<ObjectRef> & get_loader_stack() noexcept { return m_loader_stack; }
		NODISCARD ObjectRef & get_module_dict() noexcept { return m_modules; }

	public:
		template <class First, class ... Rest
		> void register_class() noexcept { mpl::for_types<First, Rest...>([&](auto tag) noexcept { TAG_TYPE(tag)::initialize_class(); }); }
#define REGISTER_CLASS(...) (Pnu::get_internals())->register_class<##__VA_ARGS__>()

		template <class First, class ... Rest
		> void unregister_class() noexcept { mpl::for_types<First, Rest...>([&](auto tag) { del_class(TAG_TYPE(tag)::get_class_name_static()); }); }
#define UNREGISTER_CLASS(...) (Pnu::get_internals())->unregister_class<##__VA_ARGS__>()

		void add_class(TypeObject * type);
		bool del_class(String const & name);
		NODISCARD TypeObject * get_class(String const & name) const;
	};

	SINGLETON_WRAPPER(Internals, get_internals);
}

#endif // !_PNU_INTERNALS_HPP_
