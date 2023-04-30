#ifndef _PNU_RECT_HPP_
#define _PNU_RECT_HPP_

#include <core/math/matrix.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class _Type> struct Rect
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename _Type;
		using self_type			= typename Rect;
		using coord_type		= typename TVec2<value_type>;
		using storage_type		= typename TVec4<value_type>;
		using pointer			= typename storage_type::pointer;
		using reference			= typename storage_type::reference;
		using const_pointer		= typename storage_type::const_pointer;
		using const_reference	= typename storage_type::const_reference;
		using iterator			= typename storage_type::iterator;
		using const_iterator	= typename storage_type::const_iterator;

		storage_type m_data{};

		constexpr Rect() noexcept = default;
		constexpr Rect(self_type const &) = default;
		constexpr Rect(self_type &&) noexcept = default;
		constexpr self_type & operator=(self_type const &) = default;
		constexpr self_type & operator=(self_type &&) noexcept = default;
		NODISCARD constexpr operator pointer() noexcept { return m_data; }
		NODISCARD constexpr operator const_pointer() const noexcept { return m_data; }
		NODISCARD constexpr operator storage_type & () & noexcept { return m_data; }
		NODISCARD constexpr operator storage_type const & () const & noexcept { return m_data; }
		NODISCARD constexpr operator storage_type && () && noexcept { return std::move(m_data); }
		NODISCARD constexpr auto begin() noexcept -> iterator { return m_data.begin(); }
		NODISCARD constexpr auto begin() const noexcept -> const_iterator { return m_data.begin(); }
		NODISCARD constexpr auto end() noexcept -> iterator { return m_data.end(); }
		NODISCARD constexpr auto end() const noexcept -> const_iterator { return m_data.end(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class X1, class Y1, class X2, class Y2
		> constexpr Rect(X1 l, Y1 t, X2 r, Y2 b)
			: m_data{ (value_type)l, (value_type)t, (value_type)r, (value_type)b }
		{
		}

		template <class A, class B = A
		> constexpr Rect(TVec2<A> const & min, TVec2<B> const & max)
			: m_data{ (value_type)min[0], (value_type)min[1], (value_type)max[0], (value_type)max[1] }
		{
		}

		template <class T
		> constexpr Rect(TVec4<T> const & value)
			: m_data{ value }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr value_type & at(size_t i) & noexcept { return m_data[i]; }
		NODISCARD constexpr value_type & left() & noexcept { return m_data[0]; }
		NODISCARD constexpr value_type & top() & noexcept { return m_data[1]; }
		NODISCARD constexpr value_type & right() & noexcept { return m_data[2]; }
		NODISCARD constexpr value_type & bottom() & noexcept { return m_data[3]; }
		NODISCARD constexpr coord_type & min() & noexcept { return *(coord_type *)&m_data[0]; }
		NODISCARD constexpr coord_type & max() & noexcept { return *(coord_type *)&m_data[2]; }

		NODISCARD constexpr value_type const & at(size_t i) const & noexcept { return m_data[i]; }
		NODISCARD constexpr value_type const & left() const & noexcept { return m_data[0]; }
		NODISCARD constexpr value_type const & top() const & noexcept { return m_data[1]; }
		NODISCARD constexpr value_type const & right() const & noexcept { return m_data[2]; }
		NODISCARD constexpr value_type const & bottom() const & noexcept { return m_data[3]; }
		NODISCARD constexpr coord_type const & min() const & noexcept { return *(coord_type const *)&m_data[0]; }
		NODISCARD constexpr coord_type const & max() const & noexcept { return *(coord_type const *)&m_data[2]; }

		NODISCARD constexpr value_type width() const noexcept { return m_data[2] - m_data[0]; }
		NODISCARD constexpr value_type height() const noexcept { return m_data[3] - m_data[1]; }
		NODISCARD constexpr coord_type size() const noexcept { return { m_data[2] - m_data[0], m_data[3] - m_data[1] }; }
		NODISCARD constexpr coord_type center() const noexcept { return (min() + max()) / 2; }
		NODISCARD constexpr coord_type position() const noexcept { return min(); }
		NODISCARD constexpr coord_type top_left() const noexcept { return min(); }
		NODISCARD constexpr coord_type top_right() const noexcept { return { m_data[2], m_data[1] }; }
		NODISCARD constexpr coord_type bottom_left() const noexcept { return { m_data[0], m_data[3] }; }
		NODISCARD constexpr coord_type bottom_right() const noexcept { return max(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using FloatRect =	Rect<f32>;
	using DoubleRect =	Rect<f64>;
	using IntRect =		Rect<i32>;
	using UintRect =		Rect<u32>;
	using SizeRect =		Rect<size_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_RECT_HPP_
