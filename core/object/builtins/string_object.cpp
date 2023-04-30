#include <core/object/builtins/string_object.hpp>
#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(StringObject, t)
	{
		t.tp_hash = (HashFunc)[](OBJ self) -> size_t { return ((STR &)self).hash_code(); };

		t.tp_len = (LenFunc)[](OBJ self) -> ssize_t { return (ssize_t)((STR &)self).size(); };

		t.tp_repr = (ReprFunc)[](OBJ self) -> STR { return (STR &)self; };

		t.tp_str = (ReprFunc)[](OBJ self) -> STR { return (STR &)self; };

		t.tp_cmp = (CmpFunc)[](OBJ self, OBJ other) -> i32
		{
			if (self.is(other)) { return 0; }

			else if (STR::check_(self) && STR::check_(other)) { return compare(***(STR &)self, ***(STR &)other); }

			else { return compare((intptr_t)*self, (intptr_t)*other); }
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}