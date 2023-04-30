#ifndef _PN_TUPLE_OBJECT_HPP_
#define _PN_TUPLE_OBJECT_HPP_

// NYI / PLACEHOLDER

#include <core/object/builtins/type_object.hpp>

namespace pn
{
	// tuple object
	class TupleObject : public Object
	{
		DEFINE_CLASS(TupleObject, Object);

		friend class TUPLE;

	public:
	};

	// tuple delete
	template <> struct DefaultDelete<TupleObject> : DefaultDelete<Object> {};

	// tuple check
#define OBJECT_CHECK_TUPLE(o) (pn::typeof(o).has_feature(pn::TypeFlags_Tuple_Subclass))

	// tuple ref
	class TUPLE : public Ref<TupleObject>
	{
		REF_CLASS(TUPLE, OBJECT_CHECK_TUPLE);

	public:
	};
}

#endif // !_PN_TUPLE_OBJECT_HPP_
