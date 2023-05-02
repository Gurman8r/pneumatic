#include <core/object/builtins/list_object.hpp>
#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ListObject, t)
	{
		t.tp_len = (LenFunc)[](ObjectRef self) -> ssize_t { return (ssize_t)ListRef(self).size(); };

		t.tp_cmp = (CmpFunc)[](ObjectRef self, ObjectRef other) -> i32
		{
			if (self.is(other)) { return 0; }

			else if (ListRef::check_(self) && ListRef::check_(other)) { return compare(***(ListRef &)self, ***(ListRef &)other); }

			else { return compare((intptr_t)*self, (intptr_t)*other); }
		};

		t.tp_bind = BIND_CLASS(ListObject, t)
		{
			return t
				.def("__contains__", &ListObject::contains<ObjectRef const &>)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}