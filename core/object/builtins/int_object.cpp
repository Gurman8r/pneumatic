#include <core/object/builtins/int_object.hpp>
#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(IntObject, t)
	{
		t.tp_hash = (HashFunc)[](ObjectRef self) -> size_t { return Hasher<i64>()(***(IntRef &)self); };

		t.tp_repr = (ReprFunc)[](ObjectRef self) -> StringRef { return StringRef(util::to_string(***(IntRef &)self)); };

		t.tp_str = (ReprFunc)[](ObjectRef self) -> StringRef { return StringRef(util::to_string(***(IntRef &)self)); };

		t.tp_cmp = (CmpFunc)[](ObjectRef self, ObjectRef other) -> i32
		{
			if (self.is(other)) { return 0; }

			else if (IntRef::check_(self) && IntRef::check_(other)) { return compare(***(IntRef &)self, ***(IntRef &)other); }

			else { return compare((intptr_t)*self, (intptr_t)*other); }
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	IntObject IntObject::g_True{ true };

	IntObject IntObject::g_False{ false };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}