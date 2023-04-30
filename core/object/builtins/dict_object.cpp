#include <core/object/builtins/dict_object.hpp>
#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(DictObject, t)
	{
		t.tp_len = (LenFunc)[](OBJ self) -> ssize_t { return (ssize_t)DICT(self).size(); };

		t.tp_bind = BIND_CLASS(DictObject, t)
		{
			return t
				.def("__contains__", &DictObject::contains<OBJ const &>)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}