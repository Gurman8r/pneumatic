#ifndef _PN_HASH_SET_HPP_
#define _PN_HASH_SET_HPP_

#include <core/os/memory.hpp>

#include <unordered_set>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// hashset base
	template <class T, class H = Hasher<T>, class E = EqualTo<T>
	> using _HashSetBase = std::unordered_set<T, H, E, PolymorphicAllocator<T>>;

	// hashset
	template <class T, class H = Hasher<T>, class E = EqualTo<T>
	> class HashSet : public _HashSetBase<T, H, E>
	{
	public:
		using base_type = _HashSetBase<T, H, E>;
		using base_type::base_type;
		using base_type::operator=;

		NODISCARD operator void * () const noexcept { return (void *)(intptr_t)!empty(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// multi hashset base
	template <class T, class H = Hasher<T>, class E = EqualTo<T>
	> using _HashMultiSetBase = std::unordered_multiset<T, H, E, PolymorphicAllocator<T>>;

	// multi hashset
	template <class T, class H = Hasher<T>, class E = EqualTo<T>
	> class HashMultiSet : public _HashMultiSetBase<T, H, E>
	{
	public:
		using base_type = _HashMultiSetBase<T, H, E>;
		using base_type::base_type;
		using base_type::operator=;

		NODISCARD operator void * () const noexcept { return (void *)(intptr_t)!empty(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_HASH_SET_HPP_
