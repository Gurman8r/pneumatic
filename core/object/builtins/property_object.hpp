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

		friend class PropertyRef;

	public:
		ObjectRef m_get{}, m_set{};

		PropertyObject() {}

		PropertyObject(ObjectRef const & fget, ObjectRef const & fset) : PropertyObject{} { m_get = fget; m_set = fset; }

		PropertyObject(ObjectRef const & fget) : PropertyObject{} { m_get = fget; m_set = nullptr; }

		template <class Getter, class Setter
		> PropertyObject(Getter const & fget, Setter const & fset) : PropertyObject{} { m_get = CppFunctionRef({ fget }); m_set = CppFunctionRef({ fset }); }

		template <class Getter
		> PropertyObject(Getter const & fget, ObjectRef const & fset) : PropertyObject{} { m_get = CppFunctionRef({ fget }); m_set = fset; }

		template <class Setter
		> PropertyObject(ObjectRef const & fget, Setter const & fset) : PropertyObject{} { m_get = fget; m_set = CppFunctionRef({ fset }); }

		template <class Getter
		> PropertyObject(Getter const & fget) : PropertyObject{} { m_get = CppFunctionRef({ fget }); m_set = nullptr; }
	};

	// property delete
	template <> struct DefaultDelete<PropertyObject> : DefaultDelete<Object> {};

	// property check
#define OBJECT_CHECK_PROPERTY(o) (isinstance<PropertyRef>(o))

	// property ref
	class PropertyRef : public Ref<PropertyObject>
	{
		REF_CLASS(PropertyRef, OBJECT_CHECK_PROPERTY);

	public:
		ObjectRef get(ObjectRef const & self) const { return VALIDATE(m_ptr)->m_get(self); }

		Error_ set(ObjectRef const & self, ObjectRef const & value) const { return VALIDATE(m_ptr)->m_set(self, value), Error_OK; }
	};
}

#endif // !_PNU_PROPERTY_OBJECT_HPP_
