#ifndef _PN_STRING_OBJECT_HPP_
#define _PN_STRING_OBJECT_HPP_

#include <core/object/builtins/type_object.hpp>

// string
namespace pn
{
	// string object
	class PN_API StringObject : public Object
	{
		DEFINE_CLASS(StringObject, Object);

		friend class STR;

	public:
		String m_string{};

		using storage_type		= decltype(m_string);
		using iterator			= storage_type::iterator;
		using const_iterator	= storage_type::const_iterator;

		auto & operator*() const { return (storage_type &)(m_string); }

		auto * operator->() const { return (storage_type *)(&m_string); }

		operator storage_type & () const & noexcept { return (storage_type &)(m_string); }

		StringObject() noexcept : m_string{} {}

		StringObject(cstring value) : m_string{ value } {}

		StringObject(cstring value, size_t n) : m_string{ value, n } {}

		StringObject(cwstring value) : m_string{ Unicode{ value }.narrow() } {}

		StringObject(cwstring value, size_t n) : m_string{ Unicode{ value, n }.narrow() } {}

		StringObject(storage_type const & value) : m_string{ value } {}

		StringObject(storage_type && value) noexcept : m_string{ std::move(value) } {}

		StringObject(Unicode const & value) : m_string{ value.narrow() } {}

		StringObject(StringView const value) : m_string{ value.data(), value.size() } {}

		StringObject(std::initializer_list<char> init) : m_string{ init } {}

		template <class T> StringObject(Ref<T> const & value) : m_string{}
		{
			if (STR::check_(value))
			{
				m_string = (storage_type)(STR)value;
			}
			else if (TYPE t{ typeof(value) }; t->tp_str)
			{
				m_string = (storage_type)t->tp_str(value);
			}
		}

		void reserve(size_t count) { m_string.reserve(count); }

		void resize(size_t count) { m_string.resize(count); }

		auto & string() const { return (storage_type &)(m_string); }

		auto c_str() const { return m_string.c_str(); }

		auto data() const { return m_string.data(); }

		bool empty() const { return m_string.empty(); }

		auto size() const { return m_string.size(); }

		auto hash_code() const { return m_string.hash_code(); }

		auto begin() -> iterator { return m_string.begin(); }

		auto begin() const -> const_iterator { return m_string.begin(); }

		auto end() -> iterator { return m_string.end(); }

		auto end() const -> const_iterator { return m_string.end(); }
	};

	// string delete
	template <> struct DefaultDelete<StringObject> : DefaultDelete<Object> {};

	// string check
#define OBJECT_CHECK_STR(o) (pn::typeof(o).has_feature(pn::TypeFlags_Str_Subclass))

	// string ref
	class STR : public Ref<StringObject>
	{
		REF_CLASS(STR, OBJECT_CHECK_STR);

	public:
		using storage_type		= value_type::storage_type;
		using iterator			= value_type::iterator;
		using const_iterator	= value_type::const_iterator;

		operator storage_type & () const { return VALIDATE(m_ptr)->operator storage_type & (); }

		void reserve(size_t count) { VALIDATE(m_ptr)->reserve(count); }

		void resize(size_t count) { VALIDATE(m_ptr)->resize(count); }

		auto & string() const { return VALIDATE(m_ptr)->string(); }

		auto c_str() const { return VALIDATE(m_ptr)->c_str(); }

		auto data() const { return VALIDATE(m_ptr)->data(); }

		bool empty() const { return VALIDATE(m_ptr)->empty(); }

		auto size() const { return VALIDATE(m_ptr)->size(); }

		auto hash_code() const { return VALIDATE(m_ptr)->hash_code(); }

		auto begin() -> iterator { return VALIDATE(m_ptr)->begin(); }

		auto begin() const -> const_iterator { return VALIDATE(m_ptr)->begin(); }

		auto end() -> iterator { return VALIDATE(m_ptr)->end(); }

		auto end() const -> const_iterator { return VALIDATE(m_ptr)->end(); }

		template <class T, class = std::enable_if_t<mpl::is_string_v<T>> // std::is_convertible_v<T, storage_type>
		> STR & operator=(T && value) noexcept
		{
			if (VALIDATE(m_ptr)) { VALIDATE(m_ptr)->m_string = FWD(value); }
			else { instance(FWD(value)); }
			return (*this);
		}
	};
}

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> STR repr(Ref<T> const & o) noexcept
	{
		if (!o) { return nullptr; }

		TYPE t{ typeof(o) };

		return t->tp_repr ? t->tp_repr(o) : nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// "__x__"
	inline bool is_dunder_name(OBJ name)
	{
		if (!STR::check_(name)) { return false; }

		String const & s{ STR(name) };

		size_t const n{ s.size() };

		return (n >= 5) && (s[0] == '_') && (s[1] == '_') && (s[n - 2] == '_') && (s[n - 1] == '_');
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_STRING_OBJECT_HPP_
