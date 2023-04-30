#ifndef _PNU_ITERATOR_OBJECT_HPP_
#define _PNU_ITERATOR_OBJECT_HPP_

// NYI / PLACEHOLDER

#include <core/object/builtins/type_object.hpp>

namespace Pnu
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
#define OBJECT_CHECK_TUPLE(o) (Pnu::typeof(o).has_feature(Pnu::TypeFlags_Tuple_Subclass))

	// iterator ref
	class ITERATOR : public Ref<IteratorObject>
	{
		REF_CLASS(ITERATOR, OBJECT_CHECK_TUPLE);

	public:
	};
}

#endif // !_PNU_ITERATOR_OBJECT_HPP_
