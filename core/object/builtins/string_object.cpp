#include <core/object/builtins/string_object.hpp>
#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(StringObject, t)
	{
		t.tp_hash = (HashFunc)[](ObjectRef self) -> size_t { return ((StringRef &)self).hash_code(); };

		t.tp_len = (LenFunc)[](ObjectRef self) -> ssize_t { return (ssize_t)((StringRef &)self).size(); };

		t.tp_repr = (ReprFunc)[](ObjectRef self) -> StringRef { return (StringRef &)self; };

		t.tp_str = (ReprFunc)[](ObjectRef self) -> StringRef { return (StringRef &)self; };

		t.tp_cmp = (CmpFunc)[](ObjectRef self, ObjectRef other) -> i32
		{
			if (self.is(other)) { return 0; }

			else if (StringRef::check_(self) && StringRef::check_(other)) { return compare(***(StringRef &)self, ***(StringRef &)other); }

			else { return compare((intptr_t)*self, (intptr_t)*other); }
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}