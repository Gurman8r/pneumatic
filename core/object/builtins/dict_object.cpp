#include <core/object/builtins/dict_object.hpp>
#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(DictObject, t)
	{
		t.tp_len = (LenFunc)[](ObjectRef self) -> ssize_t { return (ssize_t)DictRef(self).size(); };

		t.tp_bind = BIND_CLASS(DictObject, t)
		{
			return t
				.def("__contains__", &DictObject::contains<ObjectRef const &>)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}