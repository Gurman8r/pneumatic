#ifndef _PNU_INT_OBJECT_HPP_
#define _PNU_INT_OBJECT_HPP_

#include <core/object/builtins/type_object.hpp>

// bool
#define TrueRef		(Pnu::IntRef(&Pnu::IntObject::g_True))
#define FalseRef	(Pnu::IntRef(&Pnu::IntObject::g_False))
#define BoolRef(b)	((b) ? TrueRef : FalseRef)

// int
namespace Pnu
{
	// int object
	class PNU_API IntObject : public Object
	{
		DEFINE_CLASS(IntObject, Object);

		friend class IntRef;

	public:
		i64 m_int{};

		using storage_type = decltype(m_int);

		auto & operator*() const { return (storage_type &)(m_int); }

		IntObject() noexcept {}

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> explicit IntObject(T const value) : IntObject{} { m_int = static_cast<storage_type>(value); }

		template <class T, class = std::enable_if_t<mpl::is_number_v<T>>
		> IntObject & operator=(T const value) { m_int = static_cast<storage_type>(value); return (*this); }

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> operator T() const & { return static_cast<T>(m_int); }

	public:
		static IntObject g_True; // global true constant

		static IntObject g_False; // global false constant
	};

	// int delete
	template <> struct DefaultDelete<IntObject> : DefaultDelete<Object> {};

	// int check
#define OBJECT_CHECK_INT(o) (Pnu::typeof(o).has_feature(Pnu::TypeFlags_Int_Subclass))

	// int ref
	class IntRef : public Ref<IntObject>
	{
		REF_CLASS(IntRef, OBJECT_CHECK_INT);

	public:
		using storage_type = value_type::storage_type;

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> IntRef(T const value) noexcept { instance(value); }

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> operator T () const { return (T)(**VALIDATE(m_ptr)); }

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> IntRef & operator=(T const value) noexcept
		{
			if (m_ptr) { m_ptr->m_int = value; }
			else { instance(value); }
			return (*this);
		}
	};
}

#endif // !_PNU_INT_OBJECT_HPP_
