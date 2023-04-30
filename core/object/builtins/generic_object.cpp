#include <core/object/builtins/generic_object.hpp>
#include <core/object/class.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(GenericObject, t)
	{
		t.tp_cmp = (CmpFunc)[](OBJ self, OBJ other) -> i32
		{
			return compare(*self, *other);
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}