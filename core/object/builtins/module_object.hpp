#ifndef _PNU_MODULE_OBJECT_HPP_
#define _PNU_MODULE_OBJECT_HPP_

#include <core/object/builtins/cppfunction_object.hpp>

// module
namespace Pnu
{
	// module object
	class PNU_API ModuleObject : public Object
	{
		DEFINE_CLASS(ModuleObject, Object);

		friend class ModuleRef;

	public:
		DictRef m_dict{};

		StringRef m_name{};

		ModuleObject() noexcept {}

		ModuleObject(StringRef const & name) : m_dict{ DictRef::new_() }, m_name{ name } {}

		template <class Value = ObjectRef
		> void add_object(cstring name, Value && value, bool overwrite = false)
		{
			StringRef str_name{ name };
			ASSERT(overwrite || !hasattr(this, str_name));
			m_dict[str_name] = FWD(value);
		}

		template <class Func, class ... Extra
		> ModuleObject & def(cstring name, Func && func, Extra && ... extra) noexcept
		{
			CppFunctionRef cf({
				FWD(func),
				attr::name(name),
				attr::scope(this),
				attr::sibling(getattr(this, name, nullptr)),
				FWD(extra)... });
			return add_object(name, cf, true), (*this);
		}

		ModuleRef def_submodule(cstring name);

		void reload();

	public:
		static ObjectRef module_getattro(ModuleRef m, ObjectRef name);
	};

	// module delete
	template <> struct DefaultDelete<ModuleObject> : DefaultDelete<Object> {};

	// module check
#define OBJECT_CHECK_MODULE(o) (isinstance<ModuleRef>(o))

	// module ref
	class ModuleRef : public Ref<ModuleObject>
	{
		REF_CLASS(ModuleRef, OBJECT_CHECK_MODULE);

	public:
		template <class Value = ObjectRef
		> void add_object(cstring name, Value && value, bool overwrite = false)
		{
			VALIDATE(m_ptr)->add_object(name, FWD(value), overwrite);
		}

		template <class Func, class ... Extra
		> ModuleRef & def(cstring name, Func && func, Extra && ... extra) noexcept
		{
			return VALIDATE(m_ptr)->def(name, FWD(func), FWD(extra)...), (*this);
		}

		ModuleRef def_submodule(cstring name)
		{
			return VALIDATE(m_ptr)->def_submodule(name);
		}

		void reload()
		{
			VALIDATE(m_ptr)->reload();
		}
	};
}

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Name = cstring
	> ModuleRef create_extension_module(Name && name) noexcept
	{
		DictRef modules{ get_internals()->get_module_dict() };
		StringRef str_name{ FWD_OBJ(name) };
		if (modules.contains(str_name)) { return nullptr; }
		return modules[str_name] = ModuleRef({ str_name });
	}

	template <class Name = cstring
	> ModuleRef import_module(Name && name) noexcept
	{
		DictRef modules{ get_internals()->get_module_dict() };
		StringRef str_name{ FWD_OBJ(name) };
		return modules.lookup(name);
	}

	inline DictRef globals() noexcept
	{
		STR_IDENTIFIER(__main__);
		STR_IDENTIFIER(__dict__);
		ModuleRef m{ import_module(&ID___main__) };
		if (!m) { return nullptr; }
		return getattr(m, &ID___dict__);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_MODULE_OBJECT_HPP_
