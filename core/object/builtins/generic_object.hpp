#ifndef _PN_GENERIC_OBJECT_HPP_
#define _PN_GENERIC_OBJECT_HPP_

// NYI / PLACEHOLDER

#include <core/object/builtins/type_object.hpp>

namespace pn
{
	// generic object
	class GenericObject : public Object
	{
		DEFINE_CLASS(GenericObject, Object);

		friend class GENERIC;

	public:
	};

	// generic delete
	template <> struct DefaultDelete<GenericObject> : DefaultDelete<Object> {};

	// generic check
#define OBJECT_CHECK_GENERIC(o) (isinstance<GENERIC>(o))

	// generic ref
	class GENERIC : public Ref<GenericObject>
	{
		REF_CLASS(GENERIC, OBJECT_CHECK_GENERIC);

	public:
	};
}

#endif // !_PN_GENERIC_OBJECT_HPP_
