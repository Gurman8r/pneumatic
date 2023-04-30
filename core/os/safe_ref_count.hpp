#ifndef _PN_SAFE_REFCOUNT_HPP_
#define _PN_SAFE_REFCOUNT_HPP_

#include <core/templates/utility.hpp>

#include <atomic>

#undef MULTICORE_ENABLED
#define MULTICORE_ENABLED 0
static_assert(!MULTICORE_ENABLED, "thread support not implemented");

#if !MULTICORE_ENABLED
namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> FORCE_INLINE static T atomic_conditional_increment(T volatile * value)
	{
		if (*value == 0) { return 0; }
		(*value)++;
		return *value;
	}

	template <class T
	> FORCE_INLINE static T atomic_decrement(T volatile * value)
	{
		(*value)--;
		return *value;
	}

	template <class T
	> FORCE_INLINE static T atomic_increment(T volatile * value)
	{
		(*value)++;
		return *value;
	}

	template <class T, class V
	> FORCE_INLINE static T atomic_sub(T volatile * value, V volatile amount)
	{
		(*value) -= amount;
		return *value;
	}

	template <class T, class V
	> FORCE_INLINE static T atomic_add(T volatile * value, V volatile amount)
	{
		(*value) += amount;
		return *value;
	}

	template <class T, class V
	> FORCE_INLINE static T atomic_exchange_if_greater(T volatile * value, V volatile amount)
	{
		if (amount > *value) { *value = amount; }
		return *value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
#else
namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	u32 atomic_conditional_increment(u32 volatile * value)
	{
		return 0;
	}

	u32 atomic_decrement(u32 volatile * value)
	{
		return 0;
	}

	u32 atomic_increment(u32 volatile * value)
	{
		return 0;
	}

	u32 atomic_sub(u32 volatile * value, u32 volatile amount)
	{
		return 0;
	}

	u32 atomic_add(u32 volatile * value, u32 volatile amount)
	{
		return 0;
	}

	u32 atomic_exchange_if_greater(u32 volatile * value, u32 volatile amount)
	{
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	u64 atomic_conditional_increment(u64 volatile * value)
	{
		return 0;
	}

	u64 atomic_decrement(u64 volatile * value)
	{
		return 0;
	}

	u64 atomic_increment(u64 volatile * value)
	{
		return 0;
	}

	u64 atomic_sub(u64 volatile * value, u64 volatile amount)
	{
		return 0;
	}

	u64 atomic_add(u64 volatile * value, u64 volatile amount)
	{
		return 0;
	}

	u64 atomic_exchange_if_greater(u64 volatile * value, u64 volatile amount)
	{
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
#endif

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PN_API SafeRefCount final
	{
		u32 m_count{};

	public:
		FORCE_INLINE void init(u32 p_value = 1) { m_count = p_value; }

		FORCE_INLINE bool inc_ref() { return atomic_conditional_increment(&m_count) != 0; } // true on success

		FORCE_INLINE bool dec_ref() { return atomic_decrement(&m_count) == 0; } // true if must be disposed of

		FORCE_INLINE auto refval() -> u32 { return atomic_conditional_increment(&m_count); } // nullptr-zero on success

		FORCE_INLINE auto unrefval() -> u32 { return atomic_decrement(&m_count); } // 0 if must be disposed of

		FORCE_INLINE auto get() const -> u32 { return m_count; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_SAFE_REFCOUNT_HPP_
