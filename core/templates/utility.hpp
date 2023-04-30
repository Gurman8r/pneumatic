#ifndef _PN_UTILITY_HPP_
#define _PN_UTILITY_HPP_

#include <core/os/copymem.hpp>
#include <core/math/math.hpp>

#include <stdarg.h>
#include <algorithm>
#include <functional>
#include <utility>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// round down size m_size to be a multiple of m_alignment, where m_alignment is a power of 2
#define SIZE_ROUND_DOWN(m_size, m_alignment) \
		((size_t)(m_size) & ~(size_t)((m_alignment) - 1))

// round up size m_size to be m_alignment multiple of m_alignment, where m_alignment is a power of 2
#define SIZE_ROUND_UP(m_size, m_alignment) \
		(((size_t)(m_size) + (size_t)((m_alignment) - 1)) & ~(size_t)((m_alignment) - 1))

// round pointer m_ptr down to the closest m_alignment-aligned address <= m_ptr, where m_alignment is a power of 2
#define ALIGN_DOWN(m_ptr, m_alignment) \
		((void *)((uintptr_t)(m_ptr) & ~(uintptr_t)((m_alignment) - 1)))

// round pointer m_ptr up to the closest m_alignment-aligned address >= m_ptr, where m_alignment is a power of 2
#define ALIGN_UP(m_ptr, m_alignment) \
		((void *)(((uintptr_t)(m_ptr) + (uintptr_t)((m_alignment) - 1)) & ~(uintptr_t)((m_alignment) - 1)))

// check if pointer m_ptr is aligned to m_alignment-bytes boundary, where m_alignment is a power of 2
#define IS_ALIGNED(m_ptr, m_alignment) \
		(!((uintptr_t)(m_ptr) & (uintptr_t)((m_alignment) - 1)))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// ignore that a variable is unused in compiler warnings
	inline void ignore_unused(i32 const *) {}

	// sink implementation
	#define _IMPL_SINK_(var, ...) \
		int var[] = { ##__VA_ARGS__ }; \
		ignore_unused(var);

	// sink
	#define SINK(...) \
		_IMPL_SINK_(ANON(temp), ##__VA_ARGS__)

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// bit_cast
	template <class To, class From, class = std::enable_if_t<
		sizeof(To) == sizeof(From) &&
		std::is_trivially_copyable_v<From> &&
		std::is_trivial_v<To> &&
		(std::is_copy_constructible_v<To> || std::is_move_constructible_v<To>)>
	> To bit_cast(From const & value) noexcept
	{
		To temp;
		copymem(&temp, &value, sizeof(To));
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// plus
	template <class T> struct Plus
	{
		using result_type = T;
		using first_argument_type = T;
		using second_argument_type = T;
		constexpr T operator()(T const & a, T const & b) const { return a + b; }
	};

	// minus
	template <class T> struct Minus
	{
		using result_type = T;
		using first_argument_type = T;
		using second_argument_type = T;
		constexpr T operator()(T const & a, T const & b) const { return a - b; }
	};

	// multiplies
	template <class T> struct Multiplies
	{
		using result_type = T;
		using first_argument_type = T;
		using second_argument_type = T;
		constexpr T operator()(T const & a, T const & b) const { return a * b; }
	};

	// equal to
	template <class T> struct EqualTo
	{
		using result_type = bool;
		using first_argument_type = T;
		using second_argument_type = T;
		constexpr bool operator()(T const & a, T const & b) const { return a == b; }
	};

	// not equal to
	template <class T> struct NotEqualTo
	{
		using result_type = bool;
		using first_argument_type = T;
		using second_argument_type = T;
		constexpr bool operator()(T const & a, T const & b) const { return a != b; }
	};

	// less
	template <class T> struct Less
	{
		using result_type = bool;
		using first_argument_type = T;
		using second_argument_type = T;
		constexpr bool operator()(T const & a, T const & b) const { return a < b; }
	};

	// greater
	template <class T> struct Greater
	{
		using result_type = bool;
		using first_argument_type = T;
		using second_argument_type = T;
		constexpr bool operator()(T const & a, T const & b) const { return a > b; }
	};

	// less or equal
	template <class T> struct LessEqual
	{
		using result_type = bool;
		using first_argument_type = T;
		using second_argument_type = T;
		constexpr bool operator()(T const & a, T const & b) const { return a <= b; }
	};

	// greater or equal
	template <class T> struct GreaterEqual
	{
		using result_type = bool;
		using first_argument_type = T;
		using second_argument_type = T;
		constexpr bool operator()(T const & a, T const & b) const { return a >= b; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// pair
	template <class First, class Second
	> using Pair = std::pair<First, Second>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr auto compare(A const & a, B const & b) noexcept
	{
		return (a != b) ? ((a < b) ? -1 : 1) : 0;
	}

	template <class T
	> constexpr T const & constrain(T const & x, T const & min, T const & max) noexcept
	{
		return (x < min) ? min : (max < x) ? max : x;
	}

	template <class T
	> constexpr T map(T const x, T const in_min, T const in_max, T const out_min, T const out_max)
	{
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

	namespace util
	{
		template <class T
		> constexpr void swap(T & a, T & b) noexcept
		{
			T c{ std::move(a) };
			a = std::move(b);
			b = std::move(c);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class First, class Second, class ... Rest
	> constexpr decltype(auto) minimum(First && first, Second && second, Rest && ... rest) noexcept
	{
		return first < second ? minimum(FWD(first), FWD(rest)...) : minimum(FWD(second), FWD(rest)...);
	}

	template <class Only
	> constexpr decltype(auto) minimum(Only && only) noexcept
	{
		return FWD(only);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class First, class Second, class ... Rest
	> constexpr decltype(auto) maximum(First && first, Second && second, Rest && ... rest) noexcept
	{
		return first > second ? maximum(FWD(first), FWD(rest)...) : maximum(FWD(second), FWD(rest)...);
	}

	template <class Only
	> constexpr decltype(auto) maximum(Only && only) noexcept
	{
		return FWD(only);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class It0, class It1
	> constexpr bool range_eq(It0 first0, It0 last0, It1 first1, It1 last1)
	{
		return (first0 != last0 && first1 != last1) ? ((*first0 == *first1) && range_eq(first0 + 1, last0, first1 + 1, last1)) : (first0 == last0 && first1 == last1);
	}

	template <class It0, class It1
	> constexpr bool range_neq(It0 first0, It0 last0, It1 first1, It1 last1)
	{
		return (first0 != last0 && first1 != last1) ? ((*first0 != *first1) && range_neq(first0 + 1, last0, first1 + 1, last1)) : (first0 == last0 && first1 == last1);
	}

	template <class It0, class It1
	> constexpr bool range_lt(It0 first0, It0 last0, It1 first1, It1 last1)
	{
		return (first0 != last0 && first1 != last1) ? ((*first0 < *first1) && range_lt(first0 + 1, last0, first1 + 1, last1)) : (first0 == last0 && first1 == last1);
	}

	template <class It0, class It1
	> constexpr bool range_gt(It0 first0, It0 last0, It1 first1, It1 last1)
	{
		return (first0 != last0 && first1 != last1) ? ((*first0 > *first1) && range_gt(first0 + 1, last0, first1 + 1, last1)) : (first0 == last0 && first1 == last1);
	}

	template <class It0, class It1
	> constexpr bool range_leq(It0 first0, It0 last0, It1 first1, It1 last1)
	{
		return (first0 != last0 && first1 != last1) ? ((*first0 <= *first1) && range_leq(first0 + 1, last0, first1 + 1, last1)) : (first0 == last0 && first1 == last1);
	}

	template <class It0, class It1
	> constexpr bool range_geq(It0 first0, It0 last0, It1 first1, It1 last1)
	{
		return (first0 != last0 && first1 != last1) ? ((*first0 >= *first1) && range_geq(first0 + 1, last0, first1 + 1, last1)) : (first0 == last0 && first1 == last1);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class It, class T
	> constexpr bool range_eq(It first, It last, T const & value)
	{
		return (first != last) ? ((*first == value) && range_eq(first + 1, last, value)) : (first == last);
	}

	template <class It, class T
	> constexpr bool range_neq(It first, It last, T const & value)
	{
		return (first != last) ? ((*first != value) && range_neq(first + 1, last, value)) : (first == last);
	}

	template <class It, class T
	> constexpr bool range_lt(It first, It last, T const & value)
	{
		return (first != last) ? ((*first < value) && range_lt(first + 1, last, value)) : (first == last);
	}

	template <class It, class T
	> constexpr bool range_gt(It first, It last, T const & value)
	{
		return (first != last) ? ((*first > value) && range_gt(first + 1, last, value)) : (first == last);
	}

	template <class It, class T
	> constexpr bool range_leq(It first, It last, T const & value)
	{
		return (first != last) ? ((*first <= value) && range_leq(first + 1, last, value)) : (first == last);
	}

	template <class It, class T
	> constexpr bool range_geq(It first, It last, T const & value)
	{
		return (first != last) ? ((*first >= value) && range_geq(first + 1, last, value)) : (first == last);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_UTILITY_HPP_
