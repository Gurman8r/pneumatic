#ifndef _PNU_FUNCTION_OBJECT_HPP_
#define _PNU_FUNCTION_OBJECT_HPP_

#include <core/object/builtins/type_object.hpp>

// function
namespace Pnu
{
	// function object
	class PNU_API FunctionObject : public Object
	{
		DEFINE_CLASS(FunctionObject, Object);

		friend class FunctionRef;

	public:
		ObjectRef m_dict{};

		VectorCallFunc m_vectorcall{};

	public:
		virtual ~FunctionObject() noexcept override = default;
		FunctionObject(VectorCallFunc vectorcall = nullptr);

		NODISCARD ObjectRef cpp_function() const noexcept; // in cppfunction_object.hpp
		NODISCARD bool is_cpp_function() const noexcept { return cpp_function().is_valid(); }
	};

	// function delete
	template <> struct DefaultDelete<FunctionObject> : DefaultDelete<Object> {};

	// function check
#define OBJECT_CHECK_FUNCTION(o) (isinstance<FunctionRef>(o))

	// function ref
	class FunctionRef : public Ref<FunctionObject>
	{
		REF_CLASS(FunctionRef, OBJECT_CHECK_FUNCTION);

	public:
		NODISCARD ObjectRef cpp_function() const noexcept { return VALIDATE(m_ptr)->cpp_function(); }
		NODISCARD bool is_cpp_function() const noexcept { return VALIDATE(m_ptr)->is_cpp_function(); }
	};
}

#endif // !_PNU_FUNCTION_OBJECT_HPP_
