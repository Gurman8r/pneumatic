#ifndef _PNU_METHOD_OBJECT_HPP_
#define _PNU_METHOD_OBJECT_HPP_

#include <core/object/builtins/function_object.hpp>

// method
namespace Pnu
{
	// method object
	class PNU_API MethodObject : public FunctionObject
	{
		DEFINE_CLASS(MethodObject, FunctionObject);

		friend class MethodRef;

	public:
		ObjectRef m_func{}, m_self{};

		MethodObject() noexcept : FunctionObject{} {}

		MethodObject(ObjectRef const & func, ObjectRef const & self, VectorCallFunc vectorcall = &method_vectorcall)
			: FunctionObject{ vectorcall }
			, m_func		{ func }
			, m_self		{ self }
		{
		}

	public:
		static ObjectRef method_vectorcall(ObjectRef callable, ObjectRef const * argv, size_t argc);
	};

	// method delete
	template <> struct DefaultDelete<MethodObject> : DefaultDelete<Object> {};

	// method check
#define OBJECT_CHECK_METHOD(o) (isinstance<MethodRef>(o))

	// method ref
	class MethodRef : public Ref<MethodObject>
	{
		REF_CLASS(MethodRef, OBJECT_CHECK_METHOD);
	};
}

#endif // !_PNU_METHOD_OBJECT_HPP_
