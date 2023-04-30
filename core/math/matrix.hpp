#ifndef _PNU_MATRIX_HPP_
#define _PNU_MATRIX_HPP_

#include <core/templates/array.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Mat
	template <class _Type, size_t _Width, size_t _Height
	> struct Mat;

	// Mat<T, N, N>
	template <class T, size_t N
	> using TMaxNxN = Mat<T, N, N>;

	// Mat<T, 2, 2>
	template <class T
	> using TMat2x2 = TMaxNxN<T, 2>;

	// Mat<T, 3, 3>
	template <class T
	> using TMat3x3 = TMaxNxN<T, 3>;

	// Mat<T, 4 ,4>
	template <class T
	> using TMat4x4 = TMaxNxN<T, 4>;

	// Mat<T, N, 1>
	template <class T, size_t N
	> using TVec = Mat<T, N, 1>;

	// Mat<T, 2, 1>
	template <class T
	> using TVec2 = TVec<T, 2>;

	// Mat<T, 3, 1>
	template <class T
	> using TVec3 = TVec<T, 3>;

	// Mat<T, 4, 1>
	template <class T
	> using TVec4 = TVec<T, 4>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Mat2
	using Mat2b = TMat2x2<u8>;
	using Mat2i = TMat2x2<i32>;
	using Mat2u = TMat2x2<u32>;
	using Mat2f = TMat2x2<f32>;
	using Mat2d = TMat2x2<f64>;
	using Mat2s = TMat2x2<size_t>;
	using Mat2 = Mat2f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Mat3
	using Mat3b = TMat3x3<u8>;
	using Mat3i = TMat3x3<i32>;
	using Mat3u = TMat3x3<u32>;
	using Mat3f = TMat3x3<f32>;
	using Mat3d = TMat3x3<f64>;
	using Mat3s = TMat3x3<size_t>;
	using Mat3 = Mat3f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Mat4
	using Mat4b = TMat4x4<u8>;
	using Mat4i = TMat4x4<i32>;
	using Mat4u = TMat4x4<u32>;
	using Mat4f = TMat4x4<f32>;
	using Mat4d = TMat4x4<f64>;
	using Mat4s = TMat4x4<size_t>;
	using Mat4 = Mat4f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Vec2
	using Vec2b = TVec2<u8>;
	using Vec2i = TVec2<i32>;
	using Vec2u = TVec2<u32>;
	using Vec2f = TVec2<f32>;
	using Vec2d = TVec2<f64>;
	using Vec2s = TVec2<size_t>;
	using Vec2 = Vec2f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Vec3
	using Vec3b = TVec3<u8>;
	using Vec3i = TVec3<i32>;
	using Vec3u = TVec3<u32>;
	using Vec3f = TVec3<f32>;
	using Vec3d = TVec3<f64>;
	using Vec3s = TVec3<size_t>;
	using Vec3 = Vec3f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Vec4
	using Vec4b = TVec4<u8>;
	using Vec4i = TVec4<i32>;
	using Vec4u = TVec4<u32>;
	using Vec4f = TVec4<f32>;
	using Vec4d = TVec4<f64>;
	using Vec4s = TVec4<size_t>;
	using Vec4 = Vec4f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Mat
	template <class _Type, size_t _Width, size_t _Height
	> struct Mat
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static_assert(0 < _Width, "width negative or zero");
		static_assert(0 < _Height, "height negative or zero");
		static_assert(mpl::is_number_v<_Type>, "unsupported type");

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		using value_type		= typename _Type;
		using self_type			= typename Mat<value_type, _Width, _Height>;
		using storage_type		= typename Array<value_type, _Width * _Height>;
		using row_type			= typename TVec<value_type, _Height>;
		using col_type			= typename TVec<value_type, _Width>;
		using size_type			= typename storage_type::size_type;
		using difference_type	= typename storage_type::difference_type;
		using pointer			= typename storage_type::pointer;
		using reference			= typename storage_type::reference;
		using const_pointer		= typename storage_type::const_pointer;
		using const_reference	= typename storage_type::const_reference;
		using iterator			= typename storage_type::iterator;
		using const_iterator	= typename storage_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		storage_type m_data; // aggregate initializer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr operator pointer() noexcept { return m_data; }
		NODISCARD constexpr operator const_pointer() const noexcept { return m_data; }
		NODISCARD constexpr operator storage_type & () & noexcept { return m_data; }
		NODISCARD constexpr operator storage_type const & () const & noexcept { return m_data; }
		NODISCARD constexpr operator storage_type && () && noexcept { return std::move(m_data); }
		NODISCARD constexpr auto begin() noexcept -> iterator { return m_data.begin(); }
		NODISCARD constexpr auto begin() const noexcept -> const_iterator { return m_data.begin(); }
		NODISCARD constexpr auto end() noexcept -> iterator { return m_data.end(); }
		NODISCARD constexpr auto end() const noexcept -> const_iterator { return m_data.end(); }
		constexpr self_type & swap(self_type & value) noexcept { return m_data.swap(value.m_data), (*this); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD static constexpr auto width() noexcept -> size_type { return _Width; }
		NODISCARD static constexpr auto height() noexcept -> size_type { return _Height; }
		NODISCARD static constexpr auto size() noexcept -> size_type { return _Width * _Height; }
		NODISCARD static constexpr auto cols() noexcept -> size_type { return _Width; }
		NODISCARD static constexpr auto rows() noexcept -> size_type { return _Height; }
		NODISCARD static constexpr auto map_x(size_type const i) noexcept -> size_type { return i % _Width; }
		NODISCARD static constexpr auto map_y(size_type const i) noexcept -> size_type { return i / _Width; }
		NODISCARD static constexpr auto map_xy(size_type const x, size_type const y) noexcept -> size_type { return y * _Width + x; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U = value_type
		> NODISCARD constexpr auto at(size_type const i) noexcept -> reference { return static_cast<U>(m_data.at(i)); }
		
		template <class U = value_type
		> NODISCARD constexpr auto at(size_type const i) const noexcept -> const_reference { return static_cast<U>(m_data.at(i)); }

		template <class U = value_type
		> NODISCARD constexpr auto at(size_type const x, size_type const y) noexcept -> reference { return at<U>(map_xy(x, y)); }
		
		template <class U = value_type
		> NODISCARD constexpr auto at(size_type const x, size_type const y) const noexcept -> const_reference { return at<U>(map_xy(x, y)); }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD constexpr row_type row(size_type const y) const noexcept
		{
			row_type r{};
			for (size_type x{}; x < width(); ++x) { r[x] = at(x, y); }
			return r;
		}

		NODISCARD constexpr col_type col(size_type const x) const noexcept
		{
			col_type c{};
			for (size_type y{}; y < height(); ++y) { c[y] = at(x, y); }
			return c;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T, size_type W, size_type H
		> NODISCARD constexpr operator Mat<T, W, H>() const noexcept
		{
			static_assert(!std::is_same_v<Mat<T, W, H>, self_type>);

			return matrix_cast<Mat<T, W, H>>(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class
	> constexpr bool is_matrix_v{ false };

	template <class T, size_t W, size_t H
	> constexpr bool is_matrix_v<Mat<T, W, H>>{ true };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class M
	> constexpr bool is_matnxn_v{ is_matrix_v<M> && 1 < M::width() && M::width() == M::height() };

	template <class M
	> constexpr bool is_mat2x2_v{ is_matrix_v<M> && M::width() == 2 && M::height() == 2 };

	template <class M
	> constexpr bool is_mat3x3_v{ is_matrix_v<M> && M::width() == 3 && M::height() == 3 };

	template <class M
	> constexpr bool is_mat4x4_v{ is_matrix_v<M> && M::width() == 4 && M::height() == 4 };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class V
	> constexpr bool is_vec_v{ is_matrix_v<V> && 1 < V::width() && 1 == V::height() };

	template <class V
	> constexpr bool is_vec2_v{ is_vec_v<V> && V::width() == 2 && M::height() == 1 };

	template <class V
	> constexpr bool is_vec3_v{ is_vec_v<V> && V::width() == 3 && M::height() == 1 };

	template <class V
	> constexpr bool is_vec4_v{ is_vec_v<V> && V::width() == 4 && M::height() == 1 };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class To, class From, std::enable_if_t<std::is_same_v<To, From>, int> = 0
	> NODISCARD constexpr auto matrix_cast(From && from) noexcept -> decltype(FWD(from))
	{
		static_assert(is_matrix_v<To> && is_matrix_v<From>);

		return FWD(from);
	}

	template <class To, class From, std::enable_if_t<!std::is_same_v<To, From>, int> = 0
	> NODISCARD constexpr To matrix_cast(From const & from) noexcept
	{
		static_assert(is_matrix_v<To> && is_matrix_v<From>);

		To to{};

		for (size_t i{}; i < To::size(); ++i)
		{
			// same dimensions
			if constexpr (From::width() == To::width() && From::height() == To::height())
			{
				to[i] = from.at<typename To::value_type>(i);
			}
			// different dimensions
			else
			{
				size_t const x{ From::map_x(i) }, y{ From::map_y(i) };

				if (x < From::width() && y < From::height())
				{
					to[i] = from.at<typename To::value_type>(x, y);
				}
			}
		}

		return to;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// one
	template <class M, std::enable_if_t<is_matrix_v<M>, int> = 0
	> constexpr M one_v = MAKE(M, m) {
		for (auto & e : m) { e = 1; }
	};

	// identity
	template <class M, std::enable_if_t<is_matnxn_v<M>, int> = 0
	> constexpr M identity_v = MAKE(M, m) {
		for (size_t i{}; i < M::size(); ++i) {
			m[i] = static_cast<typename M::value_type>(i % M::width() == i / M::height());
		}
	};

	// right
	template <class V, std::enable_if_t<is_vec_v<V>, int> = 0
	> constexpr V right_v = MAKE(V, v) { v[0] = 1; };

	// up
	template <class V, std::enable_if_t<is_vec_v<V>, int> = 0
	> constexpr V up_v = MAKE(V, v) { v[1] = 1; };

	// front
	template <class V, std::enable_if_t<is_vec_v<V> && 2 < V::width(), int> = 0
	> constexpr V front_v = MAKE(V, v) { v[2] = 1; };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T1, size_t W1, size_t H1,
		class T2, size_t W2, size_t H2
	> NODISCARD constexpr bool operator==(Mat<T1, W1, H1> const & a, Mat<T2, W2, H2> const & b) noexcept
	{
		return a.m_data == b.m_data;
	}

	template <
		class T1, size_t W1, size_t H1,
		class T2, size_t W2, size_t H2
	> NODISCARD constexpr bool operator!=(Mat<T1, W1, H1> const & a, Mat<T2, W2, H2> const & b) noexcept
	{
		return a.m_data != b.m_data;
	}

	template <
		class T1, size_t W1, size_t H1,
		class T2, size_t W2, size_t H2
	> NODISCARD constexpr bool operator<(Mat<T1, W1, H1> const & a, Mat<T2, W2, H2> const & b) noexcept
	{
		return a.m_data < b.m_data;
	}

	template <
		class T1, size_t W1, size_t H1,
		class T2, size_t W2, size_t H2
	> NODISCARD constexpr bool operator>(Mat<T1, W1, H1> const & a, Mat<T2, W2, H2> const & b) noexcept
	{
		return a.m_data > b.m_data;
	}

	template <
		class T1, size_t W1, size_t H1,
		class T2, size_t W2, size_t H2
	> NODISCARD constexpr bool operator<=(Mat<T1, W1, H1> const & a, Mat<T2, W2, H2> const & b) noexcept
	{
		return a.m_data <= b.m_data;
	}

	template <
		class T1, size_t W1, size_t H1,
		class T2, size_t W2, size_t H2
	> NODISCARD constexpr bool operator>=(Mat<T1, W1, H1> const & a, Mat<T2, W2, H2> const & b) noexcept
	{
		return a.m_data >= b.m_data;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t W, size_t H, class B
	> NODISCARD constexpr Mat<T, W, H> operator+(Mat<T, W, H> const & a, B const & b) noexcept
	{
		return Mat<T, W, H>{ a } += b;
	}

	template <class T, size_t W, size_t H, class B
	> NODISCARD constexpr Mat<T, W, H> operator-(Mat<T, W, H> const & a, B const & b) noexcept
	{
		return Mat<T, W, H>{ a } -= b;
	}

	template <class T, size_t W, size_t H, class B
	> NODISCARD constexpr Mat<T, W, H> operator*(Mat<T, W, H> const & a, B const & b) noexcept
	{
		return Mat<T, W, H>{ a } *= b;
	}

	template <class T, size_t W, size_t H, class B
	> NODISCARD constexpr Mat<T, W, H> operator/(Mat<T, W, H> const & a, B const & b) noexcept
	{
		return Mat<T, W, H>{ a } /= b;
	}

	template <class T, size_t W, size_t H, class B
	> NODISCARD constexpr Mat<T, W, H> operator%(Mat<T, W, H> const & a, B const & b) noexcept
	{
		return Mat<T, W, H>{ a } %= b;
	}

	template <class T, size_t W, size_t H, class B
	> NODISCARD constexpr Mat<T, W, H> operator^(Mat<T, W, H> const & a, B const & b) noexcept
	{
		return Mat<T, W, H>{ a } ^= b;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t W, size_t H
	> NODISCARD constexpr Mat<T, W, H> operator-(Mat<T, W, H> a) noexcept
	{
		for (auto & e : a) { e = -e; }
		return a;
	}

	template <class T, size_t W, size_t H
	> NODISCARD constexpr Mat<T, W, H> operator+(Mat<T, W, H> const & a) noexcept
	{
		return -(-(a));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t W, size_t H
	> NODISCARD Mat<T, W, H> asin(Mat<T, W, H> a) noexcept
	{
		for (auto & e : a) { e = asin(e); }
		return a;
	}

	template <class T, size_t W, size_t H
	> NODISCARD Mat<T, W, H> acos(Mat<T, W, H> a) noexcept
	{
		for (auto & e : a) { e = acos(e); }
		return a;
	}

	template <class T, size_t W, size_t H
	> NODISCARD Mat<T, W, H> atan(Mat<T, W, H> a) noexcept
	{
		for (auto & e : a) { e = atan(e); }
		return a;
	}

	template <class T, size_t W, size_t H
	> NODISCARD Mat<T, W, H> sin(Mat<T, W, H> a) noexcept
	{
		for (auto & e : a) { e = sin(e); }
		return a;
	}

	template <class T, size_t W, size_t H
	> NODISCARD Mat<T, W, H> cos(Mat<T, W, H> a) noexcept
	{
		for (auto & e : a) { e = cos(e); }
		return a;
	}

	template <class T, size_t W, size_t H
	> NODISCARD Mat<T, W, H> tan(Mat<T, W, H> a) noexcept
	{
		for (auto & e : a) { e = tan(e); }
		return a;
	}

	template <class T, size_t W, size_t H
	> NODISCARD Mat<T, W, H> sqrt(Mat<T, W, H> a) noexcept
	{
		for (auto & e : a) { e = sqrt(e); }
		return a;
	}

	template <class T, size_t W, size_t H
	> NODISCARD Mat<T, W, H> inversesqrt(Mat<T, W, H> a) noexcept
	{
		for (auto & e : a) { e = inversesqrt(e); }
		return a;
	}

	template <class T, size_t W, size_t H
	> NODISCARD Mat<T, W, H> abs(Mat<T, W, H> a) noexcept
	{
		for (auto & e : a) { e = abs(e); }
		return a;
	}

	template <class T, size_t W, size_t H
	> NODISCARD Mat<T, W, H> pow(Mat<T, W, H> a, T const b) noexcept
	{
		for (auto & e : a) { e = pow(e, b); }
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t N
	> NODISCARD T length(TVec<T, N> const & a) noexcept
	{
		return sqrt(dot(a, a));
	}

	template <class T, size_t N
	> NODISCARD TVec<T, N> normalize(TVec<T, N> const & a) noexcept
	{
		return a * inversesqrt(dot(a, a));
	}

	template <class T, size_t N
	> NODISCARD TVec<T, N> cross(TVec<T, N> const & a, TVec<T, N> const & b) noexcept
	{
		return bit_cast<TVec<T, N>>(glm::cross((glm::vec<N, T> const &)a, (glm::vec<N, T> const &)b));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#include "vec2.inl"
#include "vec3.inl"
#include "vec4.inl"
#include "mat2x2.inl"
#include "mat3x3.inl"
#include "mat4x4.inl"

#endif // !_PNU_MATRIX_HPP_
