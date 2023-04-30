#ifndef _PN_MODULE_OBJECT_HPP_
#define _PN_MODULE_OBJECT_HPP_

#include <core/object/builtins/cppfunction_object.hpp>

// module
namespace pn
{
	// module object
	class PN_API ModuleObject : public Object
	{
		DEFINE_CLASS(ModuleObject, Object);

		friend class MODULE;

	public:
		DICT m_dict{};

		STR m_name{};

		ModuleObject() noexcept {}

		ModuleObject(STR const & name) : m_dict{ DICT::new_() }, m_name{ name } {}

		template <class Value = OBJ
		> void add_object(cstring name, Value && value, bool overwrite = false)
		{
			STR str_name{ name };
			ASSERT(overwrite || !hasattr(this, str_name));
			m_dict[str_name] = FWD(value);
		}

		template <class Func, class ... Extra
		> ModuleObject & def(cstring name, Func && func, Extra && ... extra) noexcept
		{
			CPP_FUNCTION cf({
				FWD(func),
				attr::name(name),
				attr::scope(this),
				attr::sibling(getattr(this, name, nullptr)),
				FWD(extra)... });
			return add_object(name, cf, true), (*this);
		}

		MODULE def_submodule(cstring name);

		void reload();

	public:
		static OBJ module_getattro(MODULE m, OBJ name);
	};

	// module delete
	template <> struct DefaultDelete<ModuleObject> : DefaultDelete<Object> {};

	// module check
#define OBJECT_CHECK_MODULE(o) (isinstance<MODULE>(o))

	// module ref
	class MODULE : public Ref<ModuleObject>
	{
		REF_CLASS(MODULE, OBJECT_CHECK_MODULE);

	public:
		template <class Value = OBJ
		> void add_object(cstring name, Value && value, bool overwrite = false)
		{
			VALIDATE(m_ptr)->add_object(name, FWD(value), overwrite);
		}

		template <class Func, class ... Extra
		> MODULE & def(cstring name, Func && func, Extra && ... extra) noexcept
		{
			return VALIDATE(m_ptr)->def(name, FWD(func), FWD(extra)...), (*this);
		}

		MODULE def_submodule(cstring name)
		{
			return VALIDATE(m_ptr)->def_submodule(name);
		}

		void reload()
		{
			VALIDATE(m_ptr)->reload();
		}
	};
}

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Name = cstring
	> MODULE create_extension_module(Name && name) noexcept
	{
		DICT modules{ get_internals()->get_module_dict() };
		STR str_name{ FWD_OBJ(name) };
		if (modules.contains(str_name)) { return nullptr; }
		return modules[str_name] = MODULE({ str_name });
	}

	template <class Name = cstring
	> MODULE import_module(Name && name) noexcept
	{
		DICT modules{ get_internals()->get_module_dict() };
		STR str_name{ FWD_OBJ(name) };
		return modules.lookup(name);
	}

	inline DICT globals() noexcept
	{
		STR_IDENTIFIER(__main__);
		STR_IDENTIFIER(__dict__);
		MODULE m{ import_module(&ID___main__) };
		if (!m) { return nullptr; }
		return getattr(m, &ID___dict__);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_MODULE_OBJECT_HPP_
