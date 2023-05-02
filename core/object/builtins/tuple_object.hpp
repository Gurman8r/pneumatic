#ifndef _PNU_TUPLE_OBJECT_HPP_
#define _PNU_TUPLE_OBJECT_HPP_

// NYI / PLACEHOLDER

#include <core/object/builtins/type_object.hpp>

namespace Pnu
{
	// tuple object
	class TupleObject : public Object
	{
		DEFINE_CLASS(TupleObject, Object);

		friend class TupleRef;

	public:
	};

	// tuple delete
	template <> struct DefaultDelete<TupleObject> : DefaultDelete<Object> {};

	// tuple check
#define OBJECT_CHECK_TUPLE(o) (Pnu::typeof(o).has_feature(Pnu::TypeFlags_Tuple_Subclass))

	// tuple ref
	class TupleRef : public Ref<TupleObject>
	{
		REF_CLASS(TupleRef, OBJECT_CHECK_TUPLE);

	public:
	};
}

#endif // !_PNU_TUPLE_OBJECT_HPP_
