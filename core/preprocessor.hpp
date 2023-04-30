#ifndef _PN_PREPROCESSOR_HPP_
#define _PN_PREPROCESSOR_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// concat implementation
#define _IMPL_CAT(a, b) \
		a##b

// concat
#define CAT(a, b) \
		_IMPL_CAT(a, b)

// compare
#define CMP(a, b) \
		(((a) != (b)) ? (((a) < (b)) ? -1 : 1) : 0)

// minimum
#define MIN(a, b) \
		((a) < (b) ? (a) : (b))

// maximum
#define MAX(a, b) \
		((a) < (b) ? (b) : (a))

// token literal to string
#define TOSTR(expr) \
		#expr

// macro contents to string
#define XSTR(expr) \
		TOSTR(expr)

// wide string
#define WIDE(str) \
		CAT(L, str)

// size of static array
#define ARRAY_SIZE(arr) \
		(sizeof(arr) / sizeof(*arr))

// unused expression
#define UNUSED(expr) \
		((void)(expr))

// branchless if statement
#define BRANCHLESS_IF(m_condition, m_expr) \
		(UNUSED((!(m_condition)) || ((m_expr), 0)))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// map enum to array of constant values
#define ENUM_MAPPING(m_func, m_from, m_to, ...)					\
		static constexpr m_to _MAP_##m_from##_TO_##m_to##_[] =	\
		{														\
			##__VA_ARGS__										\
		};														\
		static constexpr m_to m_func(m_from i) noexcept			\
		{														\
			return _MAP_##m_from##_TO_##m_to##_[(size_t)i];		\
		}														\

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// anonymous variable name
#if defined(__COUNTER__)
#	define ANON(m_expr) CAT(_, CAT(m_expr, CAT(_, CAT(__COUNTER__, _))))
#elif defined(__LINE__)
#	define ANON(m_expr) CAT(_, CAT(m_expr, CAT(_, CAT(__LINE__, _))))
#else
#	define ANON(m_expr) CAT(_, CAT(m_expr, _))
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// return static
#define _IMPL_RETURN_STATIC(m_type, m_var, ...) \
		static m_type m_var{ ##__VA_ARGS__ }; \
		return (m_var)

#define RETURN_STATIC(m_type, ...) \
		_IMPL_RETURN_STATIC(m_type, ANON(static_return), ##__VA_ARGS__)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* ON SCOPE ENTER */

namespace pn::priv
{
	template <class F> struct OnScopeEnter final
	{
		explicit OnScopeEnter(F fn) noexcept { fn(); }
	};

	enum class OnScopeEnter_Tag {};

	template <class F
	> auto operator+(OnScopeEnter_Tag, F fn) noexcept { return OnScopeEnter<F>{ fn }; }
}

#define ON_SCOPE_ENTER(...) \
		auto ANON(temp) = (pn::priv::OnScopeEnter_Tag{}) + [##__VA_ARGS__]() noexcept -> void

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* ON SCOPE EXIT */

namespace pn::priv
{
	template <class F> struct OnScopeExit final
	{
		F const m_fn;

		explicit OnScopeExit(F fn) noexcept : m_fn{ fn } {}

		~OnScopeExit() noexcept { m_fn(); }
	};

	enum class OnScopeExit_Tag {};

	template <class F
	> auto operator+(OnScopeExit_Tag, F fn) noexcept { return OnScopeExit<F>{ fn }; }
}

#define ON_SCOPE_EXIT(...) \
		auto ANON(temp) = (pn::priv::OnScopeExit_Tag{}) + [##__VA_ARGS__]() noexcept -> void

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// singleton helpers

// singleton constructor helper
#define SINGLETON_CTOR2(singleton, self) \
		ASSERT(!singleton); \
		(singleton) = (self);

#define SINGLETON_CTOR() \
		SINGLETON_CTOR2(__singleton, this)

// singleton destructor helper
#define SINGLETON_DTOR2(singleton, self) \
		ASSERT((singleton) == (self)); \
		ON_SCOPE_EXIT() { (singleton) = nullptr; };

#define SINGLETON_DTOR() \
		SINGLETON_DTOR2(__singleton, this)

// singleton wrapper helper
#define SINGLETON_WRAPPER(T, F) \
		NODISCARD FORCE_INLINE T * F() noexcept { return T::get_singleton(); }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_PN_PREPROCESSOR_HPP_
