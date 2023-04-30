#ifndef _PN_ITERATOR_OBJECT_HPP_
#define _PN_ITERATOR_OBJECT_HPP_

// NYI / PLACEHOLDER

#include <core/object/builtins/type_object.hpp>

namespace pn
{
	// iterator object
	class IteratorObject : public Object
	{
		DEFINE_CLASS(IteratorObject, Object);

		friend class ITERATOR;

	public:
	};

	// iterator delete
	template <> struct DefaultDelete<IteratorObject> : DefaultDelete<Object> {};

	// iterator check
#define OBJECT_CHECK_TUPLE(o) (pn::typeof(o).has_feature(pn::TypeFlags_Tuple_Subclass))

	// iterator ref
	class ITERATOR : public Ref<IteratorObject>
	{
		REF_CLASS(ITERATOR, OBJECT_CHECK_TUPLE);

	public:
	};
}

#endif // !_PN_ITERATOR_OBJECT_HPP_
