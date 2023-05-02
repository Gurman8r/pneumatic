#ifndef _PNU_GENERIC_OBJECT_HPP_
#define _PNU_GENERIC_OBJECT_HPP_

// NYI / PLACEHOLDER

#include <core/object/builtins/type_object.hpp>

namespace Pnu
{
	// generic object
	class GenericTypeObject : public Object
	{
		DEFINE_CLASS(GenericTypeObject, Object);

		friend class GenericTypeRef;

	public:
	};

	// generic delete
	template <> struct DefaultDelete<GenericTypeObject> : DefaultDelete<Object> {};

	// generic check
#define OBJECT_CHECK_GENERIC(o) (isinstance<GenericTypeRef>(o))

	// generic ref
	class GenericTypeRef : public Ref<GenericTypeObject>
	{
		REF_CLASS(GenericTypeRef, OBJECT_CHECK_GENERIC);

	public:
	};
}

#endif // !_PNU_GENERIC_OBJECT_HPP_
