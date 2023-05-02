#include <core/object/builtins/method_object.hpp>
#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(MethodObject, t)
	{
		t.tp_dictoffset = offsetof(MethodObject, m_dict);

		t.tp_vectorcalloffset = offsetof(MethodObject, m_vectorcall);

		t.tp_descr_get = (DescrGetFunc)[](ObjectRef self, ObjectRef obj, ObjectRef cls) { return self; };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ObjectRef MethodObject::method_vectorcall(ObjectRef callable, ObjectRef const * argv, size_t argc)
	{
		if (!MethodRef::check_(callable)) { return nullptr; }
		ObjectRef & self{ ((MethodRef &)callable)->m_self };
		ObjectRef & func{ ((MethodRef &)callable)->m_func };
		VectorCallFunc vcall{ get_vectorcall_func(func) };
		if (argc == 0)
		{
			return vcall(func, &self, 1);
		}
		else
		{
			ASSERT("too many arguments" && (argc + 1) < MAX_ARGUMENTS);

			ObjectRef stack[MAX_ARGUMENTS]{};

			stack[0] = self;

			std::copy(argv, argv + 1, stack + 1);

			return vcall(func, stack, argc + 1);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}