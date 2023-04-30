#include <core/object/builtins/iterator_object.hpp>
#include <core/object/class.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(IteratorObject, t)
	{
		t.tp_cmp = (CmpFunc)[](OBJ self, OBJ other) -> i32
		{
			return compare(*self, *other);
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}