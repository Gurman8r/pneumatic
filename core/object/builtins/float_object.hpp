#ifndef _PNU_FLOAT_OBJECT_HPP_
#define _PNU_FLOAT_OBJECT_HPP_

#include <core/object/builtins/type_object.hpp>

// float
namespace Pnu
{
	// float object
	class PNU_API FloatObject : public Object
	{
		DEFINE_CLASS(FloatObject, Object);

		friend class FloatRef;

	public:
		f64 m_float{};

		using storage_type = decltype(m_float);

		auto & operator*() const { return (storage_type &)(m_float); }

		FloatObject() noexcept {}

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> explicit FloatObject(T const value) : FloatObject{} { m_float = static_cast<storage_type>(value); }

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> FloatObject & operator=(T const value) { m_float = static_cast<storage_type>(value); return (*this); }

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> operator T() const & { return static_cast<T>(m_float); }
	};

	// float delete
	template <> struct DefaultDelete<FloatObject> : DefaultDelete<Object> {};

	// float check
#define OBJECT_CHECK_FLOAT(o) (Pnu::typeof(o).has_feature(Pnu::TypeFlags_Float_Subclass))

	// float ref
	class FloatRef : public Ref<FloatObject>
	{
		REF_CLASS(FloatRef, OBJECT_CHECK_FLOAT);

	public:
		using storage_type = value_type::storage_type;

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> FloatRef(T const value) noexcept { instance(value); }

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> operator T () const { return (T)(**VALIDATE(m_ptr)); }

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> FloatRef & operator=(T const value) noexcept
		{
			if (m_ptr) { m_ptr->m_float = value; }
			else { instance(value); }
			return (*this);
		}
	};
}

#endif // !_PNU_FLOAT_OBJECT_HPP_
