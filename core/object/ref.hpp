#ifndef _PNU_REF_HPP_
#define _PNU_REF_HPP_

#include <core/object/object_api.hpp>

// ref
namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Ref<Object> 
	template <class T
	> class NOVTABLE Ref : public _Ref_Tag, public ObjectAPI<Ref<T>>
	{
	public:
		using value_type = typename T;

	protected:
		value_type * m_ptr{};

		void ref(Ref const & value)
		{
			if (value.m_ptr == m_ptr) { return; }
			unref();
			m_ptr = value.m_ptr;
			if (m_ptr) { m_ptr->inc_ref(); }
		}

		void ref_pointer(value_type * value)
		{
			ASSERT("invalid ref pointer" && value);

			if (value->init_ref()) { m_ptr = value; }
		}

	public:
		~Ref() noexcept { unref(); }

		Ref() noexcept = default;

		Ref(nullptr_t) noexcept {}

		Ref(value_type * value) { if (value) { ref_pointer(value); } }

		Ref(Ref const & value) { ref(value); }

		template <class U
		> Ref(Ref<U> const & value) { reset(value); }

		Ref(value_type && value) noexcept { instance(std::move(value)); }

		Ref & operator=(nullptr_t) { return unref(), (*this); }

		Ref & operator=(Ref const & value) { return reset(value), (*this); }

		template <class U
		> Ref & operator=(Ref<U> const & value) { return reset(value), (*this); }

		Ref & operator=(value_type && value) noexcept { return instance(std::move(value)), (*this); }

	public:
		template <class ... Args
		> static auto new_(Args && ... args) { return Ref<value_type>{ value_type{ FWD(args)... } }; }

		static constexpr auto get_class_static() noexcept { return value_type::get_class_static(); }

		static auto get_type_static() noexcept { return value_type::get_type_static(); }

		template <class U> U cast() const &;

		template <class U> U cast() &&;

	public:
		template <class U = value_type, class ... Args, std::enable_if_t<std::is_base_of_v<value_type, U>, int> = 0
		> void instance(Args && ... args)
		{
			if constexpr (std::is_constructible_v<U, Args...>)
			{
				ref(memnew(U(FWD(args)...)));
			}
			else
			{
				ref(memnew(U{ FWD(args)... }));
			}
		}

		void unref()
		{
			if (m_ptr && m_ptr->dec_ref()) {
				Pnu::call_default_delete(m_ptr);
			}
			m_ptr = nullptr;
		}

		void reset(Ref const & value)
		{
			ref(value);
		}

		template <class U> void reset(U * value)
		{
			if (m_ptr == value) { return; }
			unref();
			value_type * r{ dynamic_cast<value_type *>(value) };
			if (r) { ref_pointer(r); }
		}

		template <class U> void reset(Ref<U> const & value)
		{
			Object * other{ static_cast<Object *>(value.ptr()) };
			if (!other) { unref(); return; }
			Ref r;
			r.m_ptr = dynamic_cast<value_type *>(other);
			ref(r);
			r.m_ptr = nullptr;
		}

	public:
		operator bool() const noexcept { return m_ptr != nullptr; }
		
		bool is_null() const noexcept { return m_ptr == nullptr; }
		
		bool is_valid() const noexcept { return m_ptr != nullptr; }
		
		auto ptr() const noexcept { return (value_type *)m_ptr; }

		auto operator*() noexcept { return m_ptr; }
		
		auto operator*() const noexcept { return m_ptr; }

		auto operator->() noexcept { return m_ptr; }
		
		auto operator->() const noexcept { return m_ptr; }

		template <class U> friend bool operator==(Ref const & a, U const * b) noexcept { return (void *)a.m_ptr == (void *)b; }
		
		template <class U> friend bool operator!=(Ref const & a, U const * b) noexcept { return (void *)a.m_ptr != (void *)b; }

		template <class U> friend bool operator==(U const * a, Ref const & b) noexcept { return (void *)a == (void *)b.m_ptr; }
		
		template <class U> friend bool operator!=(U const * a, Ref const & b) noexcept { return (void *)a != (void *)b.m_ptr; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Hasher<Ref<T>> : Hasher<ObjectAPI<Ref<T>>> {};
	template <class T> struct EqualTo<Ref<T>> : EqualTo<ObjectAPI<Ref<T>>> {};
	template <class T> struct NotEqualTo<Ref<T>> : NotEqualTo<ObjectAPI<Ref<T>>> {};
	template <class T> struct Less<Ref<T>> : Less<ObjectAPI<Ref<T>>> {};
	template <class T> struct Greater<Ref<T>> : Greater<ObjectAPI<Ref<T>>> {};
	template <class T> struct LessEqual<Ref<T>> : LessEqual<ObjectAPI<Ref<T>>> {};
	template <class T> struct GreaterEqual<Ref<T>> : GreaterEqual<ObjectAPI<Ref<T>>> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// ref class
#define REF_CLASS(m_class, m_check)																		\
public:																									\
	using base_type = Pnu::Ref<value_type>;																\
																										\
	static bool check_(Pnu::Ref<Object> const & o) { return o && (bool)(m_check(o)); }					\
																										\
	~m_class() noexcept = default;																		\
																										\
	m_class() noexcept = default;																		\
																										\
	m_class(nullptr_t) noexcept {}																		\
																										\
	m_class(value_type * value) { if (value) { ref_pointer(value); } }									\
																										\
	m_class(base_type const & value) { ref(value); }													\
																										\
	template <class U> m_class(Pnu::Ref<U> const & value) { reset(value); }								\
																										\
	m_class(value_type && value) noexcept { instance(std::move(value)); }								\
																										\
	m_class & operator=(nullptr_t) { return unref(), (*this); }											\
																										\
	m_class & operator=(base_type const & value) { return reset(value), (*this); }						\
																										\
	template <class U> m_class & operator=(Pnu::Ref<U> const & value) { return reset(value), (*this); }	\
																										\
	m_class & operator=(value_type && value) noexcept { return instance(std::move(value)), (*this); }	\
																										\

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_REF_HPP_
