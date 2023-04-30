#ifndef _PN_HASH_MAP_HPP_
#define _PN_HASH_MAP_HPP_

#include <core/os/memory.hpp>

#include <unordered_map>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// hashmap base
	template <class K, class V, class H = Hasher<K>, class E = EqualTo<K>
	> using _HashMapBase = std::unordered_map<K, V, H, E, PolymorphicAllocator<Pair<K const, V>>>;

	// hashmap
	template <class K, class V, class H = Hasher<K>, class E = EqualTo<K>
	> class HashMap : public _HashMapBase<K, V, H, E>
	{
	public:
		using base_type = _HashMapBase<K, V, H, E>;
		using base_type::base_type;
		using base_type::operator=;

		NODISCARD operator void * () const noexcept { return (void *)(intptr_t)!empty(); }

		NODISCARD bool contains(K const & key) const { return end() != find(key); }
		NODISCARD bool contains(K && key) const noexcept { return end() != find(FWD(key)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// multi hashmap base
	template <class K, class V, class H = Hasher<K>, class E = EqualTo<K>
	> using _HashMultiMapBase = std::unordered_multimap<K, V, H, E, PolymorphicAllocator<Pair<K const, V>>>;

	// multi hashmap
	template <class K, class V, class H = Hasher<K>, class E = EqualTo<K>
	> class HashMultiMap : public _HashMultiMapBase<K, V, H, E>
	{
	public:
		using base_type = _HashMultiMapBase<K, V, H, E>;
		using base_type::base_type;
		using base_type::operator=;

		NODISCARD operator void * () const noexcept { return (void *)(intptr_t)!empty(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace util
	{
		template <class K, class V, class H = Hasher<K>, class E = EqualTo<K>, class T
		> auto getptr(HashMap<K, V, H, E> & m, T && value) noexcept
		{
			if (auto const it{ m.find(FWD(value)) }; it != m.end())
			{
				return std::addressof(it->second);
			}
			else
			{
				return (V *)nullptr;
			}
		}

		template <class K, class V, class H = Hasher<K>, class E = EqualTo<K>, class T
		> auto getptr(HashMap<K, V, H, E> const & m, T && value) noexcept
		{
			if (auto const it{ m.find(FWD(value)) }; it != m.end())
			{
				return std::addressof(it->second);
			}
			else
			{
				return (V const *)nullptr;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_HASH_MAP_HPP_
