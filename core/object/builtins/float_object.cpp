#include <core/object/builtins/float_object.hpp>
#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(FloatObject, t)
	{
		t.tp_hash = (HashFunc)[](ObjectRef self) -> size_t { return Hasher<f64>()(***(FloatRef &)self); };

		t.tp_repr = (ReprFunc)[](ObjectRef self) -> StringRef { return StringRef(util::to_string(***(FloatRef &)self)); };

		t.tp_str = (ReprFunc)[](ObjectRef self) -> StringRef { return StringRef(util::to_string(***(FloatRef &)self)); };

		t.tp_cmp = (CmpFunc)[](ObjectRef self, ObjectRef other) -> i32
		{
			if (self.is(other)) { return 0; }

			else if (FloatRef::check_(self) && FloatRef::check_(other)) { return compare(***(FloatRef &)self, ***(FloatRef &)other); }

			else { return compare((intptr_t)*self, (intptr_t)*other); }
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}