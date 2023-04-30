#ifndef _PN_FUNCTION_OBJECT_HPP_
#define _PN_FUNCTION_OBJECT_HPP_

#include <core/object/builtins/type_object.hpp>

// function
namespace pn
{
	// function object
	class PN_API FunctionObject : public Object
	{
		DEFINE_CLASS(FunctionObject, Object);

		friend class FUNCTION;

	public:
		OBJ m_dict{};

		VectorCallFunc m_vectorcall{};

	public:
		virtual ~FunctionObject() noexcept override = default;
		FunctionObject(VectorCallFunc vectorcall = nullptr);

		NODISCARD OBJ cpp_function() const noexcept; // in cppfunction_object.hpp
		NODISCARD bool is_cpp_function() const noexcept { return cpp_function().is_valid(); }
	};

	// function delete
	template <> struct DefaultDelete<FunctionObject> : DefaultDelete<Object> {};

	// function check
#define OBJECT_CHECK_FUNCTION(o) (isinstance<FUNCTION>(o))

	// function ref
	class FUNCTION : public Ref<FunctionObject>
	{
		REF_CLASS(FUNCTION, OBJECT_CHECK_FUNCTION);

	public:
		NODISCARD OBJ cpp_function() const noexcept { return VALIDATE(m_ptr)->cpp_function(); }
		NODISCARD bool is_cpp_function() const noexcept { return VALIDATE(m_ptr)->is_cpp_function(); }
	};
}

#endif // !_PN_FUNCTION_OBJECT_HPP_
