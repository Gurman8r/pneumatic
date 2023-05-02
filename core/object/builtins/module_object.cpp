#include <core/object/builtins/module_object.hpp>
#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ModuleObject, t)
	{
		t.tp_dictoffset = offsetof(ModuleObject, m_dict);

		t.tp_getattro = (GetAttrOFunc)&ModuleObject::module_getattro;

		t.tp_new = (NewFunc)[](TypeRef type, ObjectRef args) -> ObjectRef
		{
			return memnew(ModuleObject(StringRef(args[0])));
		};

		t.tp_cmp = (CmpFunc)[](ObjectRef self, ObjectRef other) -> i32
		{
			if (ModuleRef::check_(other))
			{
				return (*self == *other) ? 0 : compare(ModuleRef(self)->m_name, ModuleRef(other)->m_name);
			}
			else
			{
				return compare(*self, *other);
			}
		};

		t.tp_bind = BIND_CLASS(ModuleObject, t)
		{
			return t
				.def("__contains__", [](ModuleObject const & self, ObjectRef const & value) { return self.m_dict.contains(value); })
				.def_readonly("__name__", &ModuleObject::m_name)
				.def_readonly("__dict__", &ModuleObject::m_dict)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ModuleRef ModuleObject::def_submodule(cstring name)
	{
		return nullptr;
	}

	void ModuleObject::reload()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ObjectRef ModuleObject::module_getattro(ModuleRef m, ObjectRef name)
	{
		return generic_getattr(m, name);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}