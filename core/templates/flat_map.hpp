#ifndef _PNU_FLAT_MAP_HPP_
#define _PNU_FLAT_MAP_HPP_

#include <core/templates/flat_set.hpp>
#include <core/templates/optional.hpp>

namespace Pnu
{
	// BASIC FLAT MAP
	template <
		class	_Kt,	// key type
		class	_Vt,	// value type
		class	_Pr,	// key comparator predicate type
		size_t	_Th		// search heuristic
	> struct BasicFlatMap
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type			= typename BasicFlatMap<_Kt, _Vt, _Pr, _Th>;
		using key_type			= typename _Kt;
		using value_type		= typename _Vt;
		using compare_type		= typename _Pr;
		using allocator_type	= typename PolymorphicAllocator<u8>;
		using difference_type	= typename ptrdiff_t;
		using size_type			= typename size_t;

		static constexpr size_type thresh{ _Th };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using key_storage						= typename FlatSet<key_type, compare_type, thresh>;
		using key_pointer						= typename key_storage::pointer;
		using key_const_pointer					= typename key_storage::const_pointer;
		using key_reference						= typename key_storage::reference;
		using key_const_reference				= typename key_storage::const_reference;
		using key_rvalue						= typename key_storage::rvalue;
		using key_iterator						= typename key_storage::iterator;
		using key_const_iterator				= typename key_storage::const_iterator;
		using key_reverse_iterator				= typename key_storage::reverse_iterator;
		using key_const_reverse_iterator		= typename key_storage::const_reverse_iterator;

		using value_storage						= typename Vector<value_type>;
		using value_pointer						= typename value_storage::pointer;
		using value_const_pointer				= typename value_storage::const_pointer;
		using value_reference					= typename value_storage::reference;
		using value_const_reference				= typename value_storage::const_reference;
		using value_rvalue						= typename value_type &&;
		using value_iterator					= typename value_storage::iterator;
		using value_const_iterator				= typename value_storage::const_iterator;
		using value_reverse_iterator			= typename value_storage::reverse_iterator;
		using value_const_reverse_iterator		= typename value_storage::const_reverse_iterator;

		using storage_type						= typename Pair<key_storage, value_storage>;
		using keyval_pair						= typename Pair<key_type, value_type>;
		using init_type							= typename std::initializer_list<keyval_pair>;

		using pointer_pair						= typename Pair<key_pointer, value_pointer>;
		using const_pointer_pair				= typename Pair<key_const_pointer, value_const_pointer>;
		using reference_pair					= typename Pair<key_reference, value_reference>;
		using const_reference_pair				= typename Pair<key_const_reference, value_const_reference>;
		using rvalue_pair						= typename Pair<key_rvalue, value_rvalue>;

		using iterator_pair						= typename Pair<key_iterator, value_iterator>;
		using const_iterator_pair				= typename Pair<key_const_iterator, value_const_iterator>;
		using reverse_iterator_pair				= typename Pair<key_reverse_iterator, value_reverse_iterator>;
		using const_reverse_iterator_pair		= typename Pair<key_const_reverse_iterator, value_const_reverse_iterator>;

		using optl_iterator_pair				= typename Optional<iterator_pair>;
		using optl_const_iterator_pair			= typename Optional<const_iterator_pair>;
		using optl_reverse_iterator_pair		= typename Optional<iterator_pair>;
		using optl_const_reverse_iterator_pair	= typename Optional<const_iterator_pair>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		BasicFlatMap(allocator_type alloc = {})
			: m_pair{ key_storage{ alloc }, value_storage{ alloc } }
		{
		}

		BasicFlatMap(Vector<key_type> && k, Vector<value_type> && v, allocator_type alloc = {}) noexcept
			: self_type{ alloc }
		{
			for (size_t i = 0, imax = ML_min(k.size(), v.size()); i < imax; ++i)
			{
				this->insert(FWD(k[i]), FWD(v[i]));
			}
		}

		BasicFlatMap(init_type value, allocator_type alloc = {})
			: self_type{ alloc }
		{
			this->assign(value);
		}

		BasicFlatMap(self_type const & value, allocator_type alloc = {})
			: self_type{ alloc }
		{
			this->assign(value);
		}

		BasicFlatMap(self_type && value, allocator_type alloc = {}) noexcept
			: self_type{ alloc }
		{
			this->swap(std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		self_type & operator=(init_type value)
		{
			self_type temp{ value };
			this->swap(temp);
			return (*this);
		}

		self_type & operator=(self_type const & value)
		{
			self_type temp{ value };
			this->swap(temp);
			return (*this);
		}

		self_type & operator=(self_type && value) noexcept
		{
			this->swap(std::move(value));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void assign(init_type value)
		{
			this->clear();

			for (auto it = value.begin(); it != value.end(); ++it)
			{
				this->insert(it->first, it->second);
			}
		}

		void assign(self_type const & value)
		{
			if (this != std::addressof(value))
			{
				m_pair = value.m_pair;
			}
		}

		void clear() noexcept
		{
			m_pair.first.clear();
			m_pair.second.clear();
		}

		void pop_back() noexcept
		{
			m_pair.first.pop_back();
			m_pair.second.pop_back();
		}

		void reserve(size_type const count)
		{
			m_pair.first.reserve(count);
			m_pair.second.reserve(count);
		}

		void resize(size_type const count)
		{
			m_pair.first.resize(count);
			m_pair.second.resize(count);
		}

		void resize(size_type const count, value_type const & value)
		{
			m_pair.first.resize(count);
			m_pair.second.resize(count, value);
		}

		void shrink_to_fit() noexcept
		{
			m_pair.first.shrink_to_fit();
			m_pair.second.shrink_to_fit();
		}

		void swap(self_type & value) noexcept
		{
			if (this != std::addressof(value))
			{
				m_pair.swap(value.m_pair);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		key_storage const & keys() const & noexcept
		{
			return m_pair.first;
		}

		value_storage const & values() const & noexcept
		{
			return m_pair.second;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		size_type capacity() const noexcept
		{
			return m_pair.first.capacity();
		}

		bool empty() const noexcept
		{
			return m_pair.first.empty();
		}

		size_type max_size() const noexcept
		{
			return m_pair.first.max_size();
		}

		size_type size() const noexcept
		{
			return m_pair.first.size();
		}

		allocator_type get_allocator() const noexcept
		{
			return m_pair.first.get_allocator();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		reference_pair back() noexcept
		{
			return { m_pair.first.back(), m_pair.second.back() };
		}

		const_reference_pair back() const noexcept
		{
			return { m_pair.first.back(), m_pair.second.back() };
		}

		reference_pair front() noexcept
		{
			return { m_pair.first.front(), m_pair.second.front() };
		}

		const_reference_pair front() const noexcept
		{
			return { m_pair.first.front(), m_pair.second.front() };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Out = difference_type
		> Out distance(key_const_iterator it) const noexcept
		{
			return m_pair.first.distance<Out>(it);
		}

		template <class Out = difference_type
		> Out distance(key_const_reverse_iterator it) const noexcept
		{
			return m_pair.first.distance<Out>(it);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		value_iterator fetch(key_const_iterator it) noexcept
		{
			return std::next(m_pair.second.begin(), this->distance(it));
		}

		value_const_iterator fetch(key_const_iterator it) const noexcept
		{
			return std::next(m_pair.second.cbegin(), this->distance(it));
		}

		value_const_iterator cfetch(key_const_iterator it) const noexcept
		{
			return this->fetch(it);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		value_reverse_iterator rfetch(key_const_reverse_iterator it) noexcept
		{
			return std::next(m_pair.second.rbegin(), this->distance(it));
		}

		value_const_reverse_iterator rfetch(key_const_reverse_iterator it) const noexcept
		{
			return std::next(m_pair.second.crbegin(), this->distance(it));
		}

		value_const_reverse_iterator crfetch(key_const_reverse_iterator it) const noexcept
		{
			return this->rfetch(it);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		iterator_pair erase(key_iterator it)
		{
			auto const result
			{
				m_pair.second.erase(this->fetch(it))
			};
			return { m_pair.first.erase(it), result };
		}

		iterator_pair erase(key_iterator first, key_iterator last)
		{
			auto const result
			{
				m_pair.second.erase(this->fetch(first), this->fetch(last))
			};
			return { m_pair.first.erase(first, last), result };
		}

		iterator_pair erase(optl_iterator_pair const & iter)
		{
			return this->erase(VALIDATE(iter)->first);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Key = key_type
		> bool contains(Key && key) const noexcept
		{
			return m_pair.first.contains(FWD(key));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Key = key_type
		> optl_iterator_pair find(Key && key) noexcept
		{
			if (auto const k{ m_pair.first.find(FWD(key)) }; k != m_pair.first.end())
			{
				return std::make_optional(iterator_pair{ k, this->fetch(k) });
			}
			else
			{
				return nullopt;
			}
		}

		template <class Key = key_type
		> optl_const_iterator_pair find(Key && key) const noexcept
		{
			if (auto const k{ m_pair.first.find(FWD(key)) }; k != m_pair.first.cend())
			{
				return std::make_optional(const_iterator_pair{ k, this->fetch(k) });
			}
			else
			{
				return nullopt;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Key, class ... Args
		> Pair<iterator_pair, bool> try_emplace(Key && key, Args && ... args) noexcept
		{
			if (auto const k{ m_pair.first.insert(FWD(key)) }; k.second)
			{
				return { this->impl_emplace_hint(k.first, FWD(args)...), true };
			}
			else
			{
				return { { k.first, this->fetch(k.first) }, false };
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Key = key_type, class Value = value_type
		> iterator_pair insert(Key && key, Value && value) noexcept
		{
			return this->try_emplace(FWD(key), FWD(value)).first;
		}

		iterator_pair insert(keyval_pair const & kvp) noexcept
		{
			return this->try_emplace(kvp.first, kvp.second).first;
		}

		iterator_pair insert(keyval_pair && kvp) noexcept
		{
			return this->try_emplace(std::move(kvp.first), std::move(kvp.second)).first;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Key = key_type
		> value_type & operator[](Key && key) & noexcept
		{
			return this->at(FWD(key));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Key = key_type
		> value_type & at(Key && key) noexcept
		{
			return this->find_or_add(FWD(key), value_type{});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Key = key_type, class Value = value_type
		> value_type & find_or_add(Key && key, Value && value) noexcept
		{
			if (auto const it{ this->find(FWD(key)) })
			{
				return (*it->second);
			}
			else
			{
				return *this->insert(FWD(key), FWD(value)).second;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Key, class Fn, class ... Args
		> value_type & find_or_add_fn(Key && key, Fn && fn, Args && ... args) noexcept
		{
			if (auto const it{ this->find(FWD(key)) })
			{
				return (*it->second);
			}
			else
			{
				return *this->insert(FWD(key), std::invoke(FWD(fn), FWD(args)...)).second;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> void for_each(key_const_iterator first, key_const_iterator last, Fn && fn)
		{
			for (; first != last; ++first)
			{
				std::invoke(fn, *first, *this->fetch(first));
			}
		}

		template <class Fn
		> void for_each(key_const_iterator first, Fn && fn)
		{
			this->for_each(first, m_pair.first.cend(), FWD(fn));
		}

		template <class Fn
		> void for_each(Fn && fn)
		{
			this->for_each(m_pair.first.cbegin(), FWD(fn));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> void for_each(key_const_iterator first, key_const_iterator last, Fn && fn) const
		{
			for (; first != last; ++first)
			{
				std::invoke(FWD(fn), *first, *this->cfetch(first));
			}
		}

		template <class Fn
		> void for_each(key_const_iterator first, Fn && fn) const
		{
			this->for_each(first, m_pair.first.cend(), FWD(fn));
		}

		template <class Fn
		> void for_each(Fn && fn) const
		{
			this->for_each(m_pair.first.cbegin(), FWD(fn));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> void for_each_n(key_const_iterator first, difference_type count, Fn && fn)
		{
			if (0 < count)
			{
				do
				{
					std::invoke(FWD(fn), *first, *this->fetch(first));
					
					--count; ++first;
				}
				while (0 < count);
			}
		}

		template <class Fn
		> void for_each_n(difference_type count, Fn && fn)
		{
			this->for_each_n(m_pair.first.cbegin(), count, FWD(fn));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> void for_each_n(key_const_iterator first, difference_type count, Fn && fn) const
		{
			if (0 < count)
			{
				do
				{
					std::invoke(FWD(fn), *first, *this->cfetch(first));

					--count; ++first;
				}
				while (0 < count);
			}
		}

		template <class Fn
		> void for_each_n(difference_type count, Fn && fn) const
		{
			this->for_each_n(m_pair.first.cbegin(), count, FWD(fn));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U = self_type
		> auto compare(U const & value) const noexcept
		{
			if constexpr (std::is_same_v<U, self_type>)
			{
				return this->compare(value.m_pair);
			}
			else
			{
				static_assert(std::is_same_v<U, storage_type>);
				return ML_compare(m_pair, value.m_pair);
			}
		}

		template <class U = self_type
		> bool operator==(U const & value) const noexcept { return this->compare(value) == 0; }

		template <class U = self_type
		> bool operator!=(U const & value) const noexcept { return this->compare(value) != 0; }

		template <class U = self_type
		> bool operator<(U const & value) const noexcept { return this->compare(value) < 0; }

		template <class U = self_type
		> bool operator>(U const & value) const noexcept { return this->compare(value) > 0; }

		template <class U = self_type
		> bool operator<=(U const & value) const noexcept { return this->compare(value) <= 0; }

		template <class U = self_type
		> bool operator>=(U const & value) const noexcept { return this->compare(value) >= 0; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:

		// emplace_hint implementation
		template <class ... Args
		> iterator_pair impl_emplace_hint(key_const_iterator it, Args && ... args)
		{
			return // must be private or the map could become unsorted
			{
				std::next(m_pair.first.begin(), this->distance(it)),
				m_pair.second.emplace(this->fetch(it), FWD(args)...)
			};
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_pair;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Kt, class _Vt, class _Pr = Less<_Kt>, size_t _Th = 42
	> using FlatMap = BasicFlatMap<_Kt, _Vt, _Pr, _Th>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_FLAT_MAP_HPP_
