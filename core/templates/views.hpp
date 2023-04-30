#ifndef _PN_VIEWS_HPP_
#define _PN_VIEWS_HPP_

#include <core/templates/utility.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// view
	template <class T
	> class View
	{
	public:
		using self_type					= View<T>;
		using value_type				= mpl::intrinsic_t<T>;
		using pointer					= value_type *;
		using const_pointer				= value_type const *;
		using reference					= value_type &;
		using const_reference			= value_type const &;
		using size_type					= size_t;
		using difference_type			= ptrdiff_t;
		using iterator					= pointer;
		using const_iterator			= const_pointer;
		using reverse_iterator			= std::reverse_iterator<iterator>;
		using const_reverse_iterator	= std::reverse_iterator<const_iterator>;

		static constexpr bool is_const{ std::is_same_v<T, value_type const> };

		constexpr View(nullptr_t) noexcept : m_data{}, m_size{} {}

		constexpr View(const_pointer data, size_type size) noexcept : m_data{ data }, m_size{ size } {}
		
		constexpr View() noexcept = default;

		constexpr View(self_type const &) = delete;

		constexpr View(self_type &&) noexcept = default;

		constexpr self_type & operator=(self_type const &) noexcept = delete;

		constexpr self_type & operator=(self_type &&) noexcept = default;

		constexpr self_type & swap(self_type & value) noexcept
		{
			if (this != std::addressof(value)) {
				util::swap(m_data, value.m_data);
				util::swap(m_size, value.m_size);
			}
			return (*this);
		}

		NODISCARD constexpr operator void * () const noexcept { return (0 < m_size) ? (void *)m_data : nullptr; }
		NODISCARD constexpr bool empty() const noexcept { return m_size == 0; }
		NODISCARD constexpr auto size() const noexcept -> size_type { return m_size; }

		template <class = std::enable_if_t<!is_const>
		> NODISCARD constexpr operator pointer () noexcept { return (pointer)m_data; }
		NODISCARD constexpr operator const_pointer () const noexcept { return m_data; }

		template <class = std::enable_if_t<!is_const>
		> NODISCARD constexpr auto data() noexcept -> pointer { return (pointer)m_data; }
		NODISCARD constexpr auto data() const noexcept -> const_pointer { return m_data; }

		template <class = std::enable_if_t<!is_const>
		> NODISCARD constexpr auto front() -> reference { return *((pointer)m_data + m_size); }
		NODISCARD constexpr auto front() const -> const_reference { return *(m_data + m_size); }

		template <class = std::enable_if_t<!is_const>
		> NODISCARD constexpr auto back() -> reference { return *((pointer)m_data + m_size - 1); }
		NODISCARD constexpr auto back() const -> const_reference { return *(m_data + m_size - 1); }

		template <class = std::enable_if_t<!is_const>
		> NODISCARD constexpr auto begin() noexcept -> iterator { return (pointer)m_data; }
		NODISCARD constexpr auto begin() const noexcept -> const_iterator { return m_data; }
		NODISCARD constexpr auto cbegin() const noexcept -> const_iterator { return m_data; }

		template <class = std::enable_if_t<!is_const>
		> NODISCARD constexpr auto end() noexcept -> iterator { return (pointer)m_data + m_size; }
		NODISCARD constexpr auto end() const noexcept -> const_iterator { return m_data + m_size; }
		NODISCARD constexpr auto cend() const noexcept -> const_iterator { return m_data + m_size; }

		template <class = std::enable_if_t<!is_const>
		> NODISCARD constexpr auto rbegin() noexcept -> reverse_iterator { return (pointer)m_data; }
		NODISCARD constexpr auto rbegin() const noexcept -> const_reverse_iterator { return m_data; }
		NODISCARD constexpr auto crbegin() const noexcept -> const_reverse_iterator { return m_data; }

		template <class = std::enable_if_t<!is_const>
		> NODISCARD constexpr auto rend() noexcept -> reverse_iterator { return (pointer)m_data + m_size; }
		NODISCARD constexpr auto rend() const noexcept -> const_reverse_iterator { return m_data + m_size; }
		NODISCARD constexpr auto crend() const noexcept -> const_reverse_iterator { return m_data + m_size; }

		NODISCARD constexpr auto compare(self_type const & value) const noexcept
		{
			if ((this == std::addressof(value)) || (m_data == value.m_data && m_size == value.m_size))
			{
				return 0;
			}
#if (CXX_LANG >= CXX_17)
			else if (auto const cmp{ __builtin_memcmp(m_data, value.m_data, minimum(m_size, value.m_size)) }; cmp != 0)
			{
				return cmp;
			}
#else
			else if (auto const cmp{ std::memcmp(m_data, value.m_data, minimum(m_size, value.m_size)) }; cmp != 0)
			{
				return cmp;
			}
#endif
			else if (m_size < value.m_size)
			{
				return -1;
			}
			else if (value.m_size < m_size)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}

	private:
		const_pointer m_data{};
		size_type m_size{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD constexpr bool operator==(View<T> const & a, View<T> const & b) { return a.compare(b) == 0; }

	template <class T
	> NODISCARD constexpr bool operator!=(View<T> const & a, View<T> const & b) { return a.compare(b) != 0; }

	template <class T
	> NODISCARD constexpr bool operator<(View<T> const & a, View<T> const & b) { return a.compare(b) < 0; }

	template <class T
	> NODISCARD constexpr bool operator<=(View<T> const & a, View<T> const & b) { return a.compare(b) <= 0; }

	template <class T
	> NODISCARD constexpr bool operator>(View<T> const & a, View<T> const & b) { return a.compare(b) > 0; }

	template <class T
	> NODISCARD constexpr bool operator>=(View<T> const & a, View<T> const & b) { return a.compare(b) >= 0; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_VIEWS_HPP_
