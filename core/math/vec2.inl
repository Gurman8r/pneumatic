#ifdef _PNU_MATRIX_HPP_

/* VEC2 */

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> inline TVec2<T> operator-(TVec2<T> const & a) noexcept { return { -a[0], -a[1] }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD TVec2<T> asin(TVec2<T> const & a) noexcept
	{
		return { asin(a[0]), asin(a[1]) };
	}

	template <class T
	> NODISCARD TVec2<T> acos(TVec2<T> const & a) noexcept
	{
		return { acos(a[0]), acos(a[1]) };
	}

	template <class T
	> NODISCARD TVec2<T> atan(TVec2<T> const & a) noexcept
	{
		return { atan(a[0]), atan(a[1]) };
	}

	template <class T
	> NODISCARD TVec2<T> sin(TVec2<T> const & a) noexcept
	{
		return { sin(a[0]), sin(a[1]) };
	}

	template <class T
	> NODISCARD TVec2<T> cos(TVec2<T> const & a) noexcept
	{
		return { cos(a[0]), cos(a[1]) };
	}

	template <class T
	> NODISCARD TVec2<T> tan(TVec2<T> const & a) noexcept
	{
		return { tan(a[0]), tan(a[1]) };
	}

	template <class T
	> NODISCARD TVec2<T> sqrt(TVec2<T> const & a) noexcept
	{
		return { sqrt(a[0]), sqrt(a[1]) };
	}

	template <class T
	> NODISCARD TVec2<T> inversesqrt(TVec2<T> const & a) noexcept
	{
		return { inversesqrt(a[0]), inversesqrt(a[1]) };
	}

	template <class T
	> NODISCARD TVec2<T> abs(TVec2<T> const & a) noexcept
	{
		return { abs(a[0]), abs(a[1]) };
	}

	template <class T
	> NODISCARD TVec2<T> pow(TVec2<T> const & a, T const b) noexcept
	{
		return { pow(a[0], b), pow(a[1], b) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD T dot(TVec2<T> const & a, TVec2<T> const & b) noexcept
	{
		return a[0] * b[0] + a[1] * b[1];
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr bool operator==(TVec2<A> const & a, TVec2<B> const & b) noexcept
	{
		return (std::addressof(a) == std::addressof(b)) || (a[0] == (A)b[0] && a[1] == (A)b[1]);
	}

	template <class A, class B
	> constexpr bool operator!=(TVec2<A> const & a, TVec2<B> const & b) noexcept
	{
		return !(a == b);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr TVec2<A> & operator+=(TVec2<A> & a, TVec2<B> const & b) noexcept
	{
		a[0] += (A)b[0];
		a[1] += (A)b[1];
		return a;
	}

	template <class A, class B
	> constexpr TVec2<A> & operator-=(TVec2<A> & a, TVec2<B> const & b) noexcept
	{
		a[0] -= (A)b[0];
		a[1] -= (A)b[1];
		return a;
	}

	template <class A, class B
	> constexpr TVec2<A> & operator*=(TVec2<A> & a, TVec2<B> const & b) noexcept
	{
		a[0] *= (A)b[0];
		a[1] *= (A)b[1];
		return a;
	}

	template <class A, class B
	> constexpr TVec2<A> & operator/=(TVec2<A> & a, TVec2<B> const & b) noexcept
	{
		a[0] /= (A)b[0];
		a[1] /= (A)b[1];
		return a;
	}

	template <class A, class B
	> constexpr TVec2<A> & operator%=(TVec2<A> & a, TVec2<B> const & b) noexcept
	{
		a[0] %= (A)b[0];
		a[1] %= (A)b[1];
		return a;
	}

	template <class A, class B
	> constexpr TVec2<A> & operator^=(TVec2<A> & a, TVec2<B> const & b) noexcept
	{
		a[0] ^= (A)b[0];
		a[1] ^= (A)b[1];
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec2<A> & operator+=(TVec2<A> & a, B const b) noexcept
	{
		a[0] += (A)b;
		a[1] += (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec2<A> & operator-=(TVec2<A> & a, B const b) noexcept
	{
		a[0] -= (A)b;
		a[1] -= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec2<A> & operator*=(TVec2<A> & a, B const b) noexcept
	{
		a[0] *= (A)b;
		a[1] *= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec2<A> & operator/=(TVec2<A> & a, B const b) noexcept
	{
		a[0] /= (A)b;
		a[1] /= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec2<A> & operator%=(TVec2<A> & a, B const b) noexcept
	{
		a[0] %= (A)b;
		a[1] %= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec2<A> & operator^=(TVec2<A> & a, B const b) noexcept
	{
		a[0] ^= (A)b;
		a[1] ^= (A)b;
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // _PNU_MATRIX_HPP_
