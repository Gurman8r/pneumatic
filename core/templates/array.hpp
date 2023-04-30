#ifndef _PN_ARRAY_HPP_
#define _PN_ARRAY_HPP_

#include <core/templates/utility.hpp>

namespace pn
{
	// fixed size array
	template <class _Ty, size_t _Size
	> class Array
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type				= typename _Ty;
		using self_type					= typename Array<value_type, _Size>;
		using storage_type				= typename value_type[_Size];
		using size_type					= typename size_t;
		using difference_type			= typename ptrdiff_t;
		using pointer					= typename value_type *;
		using const_pointer				= typename value_type const *;
		using reference					= typename value_type &;
		using const_reference			= typename value_type const &;
		using iterator					= typename pointer;
		using const_iterator			= typename const_pointer;
		using reverse_iterator			= typename std::reverse_iterator<iterator>;
		using const_reverse_iterator	= typename std::reverse_iterator<const_iterator>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		storage_type m_data; // aggregate initializer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr self_type & swap(self_type & other) noexcept
		{
			if (this != std::addressof(other))
			{
				for (size_t i{}; i < _Size; ++i)
				{
					util::swap(m_data[i], other.m_data[i]);
				}
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr bool empty() noexcept { return false; }
		static constexpr auto capacity() noexcept -> size_type { return _Size; }
		static constexpr auto max_size() noexcept -> size_type { return _Size; }
		static constexpr auto size() noexcept -> size_type { return _Size; }

		constexpr operator pointer() noexcept { return m_data; }
		constexpr operator const_pointer() const noexcept { return m_data; }
		constexpr auto data() noexcept -> pointer { return m_data; }
		constexpr auto data() const noexcept -> const_pointer { return m_data; }
		constexpr auto at(size_type const i) & noexcept -> reference { return m_data[i]; }
		constexpr auto at(size_type const i) const & noexcept -> const_reference { return m_data[i]; }
		constexpr auto at(size_type const i) && noexcept -> value_type && { return std::move(m_data[i]); }

		constexpr auto back() & noexcept -> reference { return m_data[_Size - 1]; }
		constexpr auto back() const & noexcept -> const_reference { return m_data[_Size - 1]; }
		constexpr auto back() && noexcept -> value_type && { return std::move(m_data[_Size - 1]); }
		constexpr auto front() & noexcept -> reference { return m_data[0]; }
		constexpr auto front() const & noexcept -> const_reference { return m_data[0]; }
		constexpr auto front() && noexcept -> value_type && { return std::move(m_data[0]); }

		constexpr auto begin() noexcept -> iterator { return m_data; }
		constexpr auto begin() const noexcept -> const_iterator { return m_data; }
		constexpr auto end() noexcept -> iterator { return m_data + _Size; }
		constexpr auto end() const noexcept -> const_iterator { return m_data + _Size; }
		constexpr auto cbegin() const noexcept -> const_iterator { return begin(); }
		constexpr auto cend() const noexcept -> const_iterator { return end(); }
		constexpr auto crbegin() const noexcept -> const_reverse_iterator { return rbegin(); }
		constexpr auto crend() const noexcept -> const_reverse_iterator { return rend(); }
		constexpr auto rbegin() noexcept -> reverse_iterator { return std::make_reverse_iterator(end()); }
		constexpr auto rbegin() const noexcept -> const_reverse_iterator { return std::make_reverse_iterator(cend()); }
		constexpr auto rend() noexcept -> reverse_iterator { return std::make_reverse_iterator(begin()); }
		constexpr auto rend() const noexcept -> const_reverse_iterator { return std::make_reverse_iterator(cbegin()); }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// zero size array
	template <class _Ty
	> class Array<_Ty, 0>
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using value_type				= typename _Ty;
		using self_type					= typename Array<value_type, 0>;
		using storage_type				= typename value_type[1];
		using size_type					= typename size_t;
		using difference_type			= typename ptrdiff_t;
		using pointer					= typename value_type *;
		using const_pointer				= typename value_type const *;
		using reference					= typename value_type &;
		using const_reference			= typename value_type const &;
		using iterator					= typename pointer;
		using const_iterator			= typename const_pointer;
		using reverse_iterator			= typename std::reverse_iterator<iterator>;
		using const_reverse_iterator	= typename std::reverse_iterator<const_iterator>;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		storage_type m_data;
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr bool empty() noexcept { return true; }
		static constexpr auto capacity() noexcept -> size_type { return 0; }
		static constexpr auto max_size() noexcept -> size_type { return 0; }
		static constexpr auto size() noexcept -> size_type { return 0; }

		constexpr operator pointer() noexcept { return m_data; }
		constexpr operator const_pointer() const noexcept { return m_data; }
		constexpr auto data() noexcept -> pointer { return &m_data[0]; }
		constexpr auto data() const noexcept -> const_pointer { return &m_data[0]; }
		constexpr auto at(size_type) & noexcept -> reference { return m_data[0]; }
		constexpr auto at(size_type) const & noexcept -> const_reference { return m_data[0]; }
		constexpr auto at(size_type) && noexcept -> value_type && { return std::move(m_data[0]); }

		constexpr auto back() & noexcept -> reference { return m_data[0]; }
		constexpr auto back() const & noexcept -> const_reference { return m_data[0]; }
		constexpr auto back() && noexcept -> value_type && { return std::move(m_data[0]); }
		constexpr auto front() & noexcept -> reference { return m_data[0]; }
		constexpr auto front() const & noexcept -> const_reference { return m_data[0]; }
		constexpr auto front() && noexcept -> value_type && { return std::move(m_data[0]); }

		constexpr auto begin() noexcept -> iterator { return &m_data[0]; }
		constexpr auto begin() const noexcept -> const_iterator { return &m_data[0]; }
		constexpr auto cbegin() const noexcept -> const_iterator { return &m_data[0]; }
		constexpr auto cend() const noexcept -> const_iterator { return &m_data[0]; }
		constexpr auto crbegin() const noexcept -> const_reverse_iterator { return &m_data[0]; }
		constexpr auto crend() const noexcept -> const_reverse_iterator { return &m_data[0]; }
		constexpr auto end() noexcept -> iterator { return &m_data[0]; }
		constexpr auto end() const noexcept -> const_iterator { return &m_data[0]; }
		constexpr auto rbegin() noexcept -> reverse_iterator { return &m_data[0]; }
		constexpr auto rbegin() const noexcept -> const_reverse_iterator { return &m_data[0]; }
		constexpr auto rend() noexcept -> reverse_iterator { return &m_data[0]; }
		constexpr auto rend() const noexcept -> const_reverse_iterator { return &m_data[0]; }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t N
	> constexpr bool operator==(Array<Tx, N> const & a, Array<Ty, N> const & b) noexcept
	{
		return std::addressof(a) == std::addressof(b) || range_eq(a.begin(), a.end(), b.begin(), b.end());
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator!=(Array<Tx, N> const & a, Array<Ty, N> const & b) noexcept
	{
		return std::addressof(a) != std::addressof(b) && range_neq(a.begin(), a.end(), b.begin(), b.end());
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator<(Array<Tx, N> const & a, Array<Ty, N> const & b) noexcept
	{
		return std::addressof(a) != std::addressof(b) && range_lt(a.begin(), a.end(), b.begin(), b.end());
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator<=(Array<Tx, N> const & a, Array<Ty, N> const & b) noexcept
	{
		return std::addressof(a) == std::addressof(b) || range_leq(a.begin(), a.end(), b.begin(), b.end());
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator>(Array<Tx, N> const & a, Array<Ty, N> const & b) noexcept
	{
		return std::addressof(a) != std::addressof(b) && range_gt(a.begin(), a.end(), b.begin(), b.end());
	}

	template <class Tx, class Ty, size_t N
	> constexpr bool operator>=(Array<Tx, N> const & a, Array<Ty, N> const & b) noexcept
	{
		return std::addressof(a) == std::addressof(b) || range_geq(a.begin(), a.end(), b.begin(), b.end());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_ARRAY_HPP_
