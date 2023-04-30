#ifndef _PNU_FLAT_SET_HPP_
#define _PNU_FLAT_SET_HPP_

#include <core/templates/vector.hpp>

namespace Pnu
{
	// BASIC FLAT SET
	template <
		class	_Ty,	// value type
		class	_Pr,	// comparator predicate type
		bool	_Mt,	// true if multiple equivalent values are permitted
		size_t	_Th		// search heuristic
	> struct BasicFlatSet
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type			= typename BasicFlatSet<_Ty, _Pr, _Mt, _Th>;
		using value_type		= typename _Ty;
		using compare_type		= typename _Pr;
		using allocator_type	= typename PolymorphicAllocator<u8>;
		using difference_type	= typename ptrdiff_t;
		using size_type			= typename size_t;

		static constexpr bool multi{ _Mt };

		static constexpr size_type thresh{ _Th };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using storage_type				= typename Vector<value_type>;
		using init_type					= typename std::initializer_list<value_type>;
		using pointer					= typename storage_type::pointer;
		using const_pointer				= typename storage_type::const_pointer;
		using reference					= typename storage_type::reference;
		using const_reference			= typename storage_type::const_reference;
		using rvalue					= typename value_type &&;
	
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		BasicFlatSet(allocator_type alloc = {}) noexcept
			: m_data{ alloc }
		{
		}

		BasicFlatSet(init_type value, allocator_type alloc = {})
			: m_data{ value, alloc }
		{
			this->impl_sort();
		}

		template <class It
		> BasicFlatSet(It first, It last, allocator_type alloc = {})
			: m_data{ first, last, alloc }
		{
			this->impl_sort();
		}

		BasicFlatSet(storage_type const & value, allocator_type alloc = {})
			: m_data{ value, alloc }
		{
			this->impl_sort();
		}

		BasicFlatSet(storage_type && value, allocator_type alloc = {}) noexcept
			: m_data{ std::move(value), alloc }
		{
			this->impl_sort();
		}

		BasicFlatSet(self_type const & value, allocator_type alloc = {})
			: m_data{ value.m_data, alloc }
		{
		}

		BasicFlatSet(self_type && value, allocator_type alloc = {}) noexcept
			: m_data{ std::move(value.m_data), alloc }
		{
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

		self_type & operator=(storage_type const & value)
		{
			self_type temp{ value };
			this->swap(temp);
			return (*this);
		}

		self_type & operator=(self_type && value) noexcept
		{
			this->swap(value);
			return (*this);
		}

		self_type & operator=(storage_type && value) noexcept
		{
			self_type temp{ std::move(value) };
			this->swap(temp);
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class It
		> void assign(It first, It last)
		{
			m_data.assign(first, last);

			this->impl_sort();
		}

		void assign(init_type value)
		{
			m_data.assign(value);

			this->impl_sort();
		}

		void assign(storage_type const & value)
		{
			if (std::addressof(m_data) != std::addressof(value))
			{
				m_data = value;

				this->impl_sort();
			}
		}

		void assign(self_type const & value)
		{
			if (this != std::addressof(value))
			{
				m_data = value.m_data;
			}
		}

		void clear() noexcept
		{
			m_data.clear();
		}

		void pop_back() noexcept
		{
			m_data.pop_back();
		}

		void reserve(size_type const cap)
		{
			m_data.reserve(cap);
		}

		void resize(size_type const cap)
		{
			m_data.resize(cap);
		}

		void shrink_to_fit()
		{
			m_data.shrink_to_fit();
		}

		void swap(self_type & value) noexcept
		{
			if (this != std::addressof(value))
			{
				m_data.swap(value.m_data);
			}
		}

		void swap(storage_type & value) noexcept
		{
			if (std::addressof(m_data) != std::addressof(value))
			{
				m_data.swap(value);

				this->impl_sort();
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Out = difference_type
		> Out distance(const_iterator it) const noexcept
		{
			return static_cast<Out>(std::distance(cbegin(), it));
		}

		template <class Out = difference_type
		> Out distance(const_reverse_iterator it) const noexcept
		{
			return static_cast<Out>(std::distance(crbegin(), it));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		iterator erase(iterator it)
		{
			return m_data.erase(it);
		}

		iterator erase(iterator first, iterator last)
		{
			return m_data.erase(first, last);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Value = value_type
		> bool contains(Value && value) const noexcept
		{
			return self_type::impl_contains(begin(), end(), FWD(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Value = value_type
		> iterator find(Value && value) noexcept
		{
			return self_type::impl_find(begin(), end(), FWD(value));
		}

		template <class Value = value_type
		> const_iterator find(Value && value) const noexcept
		{
			return self_type::impl_find(cbegin(), cend(), FWD(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn
		> iterator find_if(Fn && fn) noexcept
		{
			return std::find_if(begin(), end(), FWD(fn));
		}

		template <class Fn
		> const_iterator find_if(Fn && fn) const noexcept
		{
			return std::find_if(cbegin(), cend(), FWD(fn));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Value = value_type
		> auto insert(Value && value) noexcept
		{
			return this->impl_insert(FWD(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Value = value_type
		> auto & find_or_add(Value && value) noexcept
		{
			if (auto const it{ this->find(FWD(value)) }; it != this->end())
			{
				return (*it);
			}
			else if constexpr (multi)
			{
				return *this->insert(FWD(value));
			}
			else
			{
				return *this->insert(FWD(value)).first;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		auto operator[](size_type const i) noexcept -> reference { return m_data[i]; }

		auto operator[](size_type const i) const noexcept -> const_reference { return m_data[i]; }

		auto at(size_type const i) -> reference { return m_data.at(i); }

		auto at(size_type const i) const -> const_reference { return m_data.at(i); }

		auto back() noexcept -> reference { return m_data.back(); }

		auto back() const noexcept -> const_reference { return m_data.back(); }

		auto begin() noexcept -> iterator { return m_data.begin(); }

		auto begin() const noexcept -> const_iterator { return m_data.begin(); }

		auto capacity() const noexcept -> size_type { return m_data.capacity(); }

		auto cbegin() const noexcept -> const_iterator { return m_data.cbegin(); }

		auto cend() const noexcept -> const_iterator { return m_data.cend(); }

		auto crbegin() const noexcept -> const_reverse_iterator { return m_data.crbegin(); }

		auto crend() const noexcept -> const_reverse_iterator { return m_data.crend(); }

		auto data() noexcept -> pointer { return m_data.data(); }

		auto data() const noexcept -> const_pointer { return m_data.data(); }

		auto elements() const noexcept -> storage_type const & { return m_data; }

		bool empty() const noexcept { return m_data.empty(); }

		auto end() noexcept -> iterator { return m_data.end(); }

		auto end() const noexcept -> const_iterator { return m_data.end(); }

		auto front() noexcept -> reference { return m_data.front(); }

		auto front() const noexcept -> const_reference { return m_data.front(); }
		
		auto get_allocator() const noexcept -> allocator_type { return {}; }

		auto max_size() const noexcept -> size_type { return m_data.max_size(); }

		auto rbegin() noexcept -> reverse_iterator { return m_data.rbegin(); }

		auto rbegin() const noexcept -> const_reverse_iterator { return m_data.rbegin(); }

		auto rend() noexcept -> reverse_iterator { return m_data.rend(); }

		auto rend() const noexcept -> const_reverse_iterator { return m_data.rend(); }

		auto size() const noexcept -> size_type { return m_data.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U = self_type
		> auto compare(U const & value) const noexcept
		{
			if constexpr (std::is_same_v<U, self_type>)
			{
				return this->compare(value.m_data);
			}
			else
			{
				static_assert(std::is_same_v<U, storage_type>);
				return ML_compare(m_data, value.m_data);
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

		// contains implementation
		template <class It, class Value
		> static bool impl_contains(It first, It last, Value && value) noexcept
		{
			auto linear = [&
			]() noexcept { return std::find(first, last, FWD(value)) != last; };

			auto binary = [&
			]() noexcept { return std::binary_search(first, last, FWD(value), compare_type{}); };

			if constexpr (thresh == 0)
			{
				return linear(); // always linear
			}
			else if ((size_t)std::distance(first, last) < thresh)
			{
				return linear(); // linear
			}
			else
			{
				return binary(); // binary
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// find implementation
		template <class It, class Value
		> static auto impl_find(It first, It last, Value && value) noexcept
		{
			auto linear = [&
			]() noexcept { return std::find(first, last, FWD(value)); };

			auto binary = [&, it = std::equal_range(first, last, FWD(value), compare_type{})
			]() noexcept { return (it.first != it.second) ? it.first : last; };

			if constexpr (thresh == 0)
			{
				return linear(); // always linear
			}
			else if ((size_t)std::distance(first, last) < thresh)
			{
				return linear(); // linear
			}
			else
			{
				return binary(); // binary
			}
		}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// sort implementation
		void impl_sort() noexcept
		{
			// empty
			if (this->empty()) { return; }

			// sort
			if constexpr (0 < thresh)
			{
				std::sort(begin(), end(), compare_type{});
			}

			// remove duplicates
			if constexpr (!multi)
			{
				this->erase(std::unique(begin(), end()), end());
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// insert implementation
		template <class Value
		> auto impl_insert(Value && value) noexcept -> std::conditional_t<multi,
			iterator,
			Pair<iterator, bool>
		>
		{
			if constexpr (multi)
			{
				// insert multi
				return m_data.emplace(
					std::upper_bound(begin(), end(), FWD(value), compare_type{}),
					FWD(value));
			}
			else
			{
				// insert unique
				if (auto const it{ std::equal_range(begin(), end(), FWD(value), compare_type{}) }
				; it.first == it.second)
				{
					return { m_data.emplace(it.second, FWD(value)), true };
				}
				else
				{
					return { it.second, false };
				}
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		storage_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Ty, class _Pr = Less<_Ty>, size_t _Th = 42
	> using FlatSet = BasicFlatSet<_Ty, _Pr, false, _Th>;

	template <class _Ty, class _Pr = Less<_Ty>, size_t _Th = 42
	> using FlatMultiset = BasicFlatSet<_Ty, _Pr, true, _Th>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_FLAT_SET_HPP_
