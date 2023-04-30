#include <core/object/builtins/list_object.hpp>
#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ListObject, t)
	{
		t.tp_len = (LenFunc)[](OBJ self) -> ssize_t { return (ssize_t)LIST(self).size(); };

		t.tp_cmp = (CmpFunc)[](OBJ self, OBJ other) -> i32
		{
			if (self.is(other)) { return 0; }

			else if (LIST::check_(self) && LIST::check_(other)) { return compare(***(LIST &)self, ***(LIST &)other); }

			else { return compare((intptr_t)*self, (intptr_t)*other); }
		};

		t.tp_bind = BIND_CLASS(ListObject, t)
		{
			return t
				.def("__contains__", &ListObject::contains<OBJ const &>)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}