#ifndef _PNU_PROPERTY_OBJECT_HPP_
#define _PNU_PROPERTY_OBJECT_HPP_

#include <core/object/builtins/type_object.hpp>

// property
namespace Pnu
{
	// property object
	class PNU_API PropertyObject : public Object
	{
		DEFINE_CLASS(PropertyObject, Object);

		friend class PROPERTY;

	public:
		OBJ m_get{}, m_set{};

		PropertyObject() {}

		PropertyObject(OBJ const & fget, OBJ const & fset) : PropertyObject{} { m_get = fget; m_set = fset; }

		PropertyObject(OBJ const & fget) : PropertyObject{} { m_get = fget; m_set = nullptr; }

		template <class Getter, class Setter
		> PropertyObject(Getter const & fget, Setter const & fset) : PropertyObject{} { m_get = CPP_FUNCTION({ fget }); m_set = CPP_FUNCTION({ fset }); }

		template <class Getter
		> PropertyObject(Getter const & fget, OBJ const & fset) : PropertyObject{} { m_get = CPP_FUNCTION({ fget }); m_set = fset; }

		template <class Setter
		> PropertyObject(OBJ const & fget, Setter const & fset) : PropertyObject{} { m_get = fget; m_set = CPP_FUNCTION({ fset }); }

		template <class Getter
		> PropertyObject(Getter const & fget) : PropertyObject{} { m_get = CPP_FUNCTION({ fget }); m_set = nullptr; }
	};

	// property delete
	template <> struct DefaultDelete<PropertyObject> : DefaultDelete<Object> {};

	// property check
#define OBJECT_CHECK_PROPERTY(o) (isinstance<PROPERTY>(o))

	// property ref
	class PROPERTY : public Ref<PropertyObject>
	{
		REF_CLASS(PROPERTY, OBJECT_CHECK_PROPERTY);

	public:
		OBJ get(OBJ const & self) const { return VALIDATE(m_ptr)->m_get(self); }

		Error_ set(OBJ const & self, OBJ const & value) const { return VALIDATE(m_ptr)->m_set(self, value), Error_OK; }
	};
}

#endif // !_PNU_PROPERTY_OBJECT_HPP_
