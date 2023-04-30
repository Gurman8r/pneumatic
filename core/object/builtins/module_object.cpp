#include <core/object/builtins/module_object.hpp>
#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ModuleObject, t)
	{
		t.tp_dictoffset = offsetof(ModuleObject, m_dict);

		t.tp_getattro = (GetAttrOFunc)&ModuleObject::module_getattro;

		t.tp_new = (NewFunc)[](TYPE type, OBJ args) -> OBJ
		{
			return memnew(ModuleObject(STR(args[0])));
		};

		t.tp_cmp = (CmpFunc)[](OBJ self, OBJ other) -> i32
		{
			if (MODULE::check_(other))
			{
				return (*self == *other) ? 0 : compare(MODULE(self)->m_name, MODULE(other)->m_name);
			}
			else
			{
				return compare(*self, *other);
			}
		};

		t.tp_bind = BIND_CLASS(ModuleObject, t)
		{
			return t
				.def("__contains__", [](ModuleObject const & self, OBJ const & value) { return self.m_dict.contains(value); })
				.def_readonly("__name__", &ModuleObject::m_name)
				.def_readonly("__dict__", &ModuleObject::m_dict)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MODULE ModuleObject::def_submodule(cstring name)
	{
		return nullptr;
	}

	void ModuleObject::reload()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJ ModuleObject::module_getattro(MODULE m, OBJ name)
	{
		return generic_getattr(m, name);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}