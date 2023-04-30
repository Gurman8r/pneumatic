#ifdef _PN_MATRIX_HPP_

/* VEC4 */

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> inline TVec4<T> operator-(TVec4<T> const & a) noexcept { return { -a[0], -a[1], -a[2], -a[3] }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD TVec4<T> asin(TVec4<T> const & a) noexcept
	{
		return { asin(a[0]), asin(a[1]), asin(a[2]), asin(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> acos(TVec4<T> const & a) noexcept
	{
		return { acos(a[0]), acos(a[1]), acos(a[2]), acos(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> atan(TVec4<T> const & a) noexcept
	{
		return { atan(a[0]), atan(a[1]), atan(a[2]), atan(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> sin(TVec4<T> const & a) noexcept
	{
		return { sin(a[0]), sin(a[1]), sin(a[2]), sin(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> cos(TVec4<T> const & a) noexcept
	{
		return { cos(a[0]), cos(a[1]), cos(a[2]), cos(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> tan(TVec4<T> const & a) noexcept
	{
		return { tan(a[0]), tan(a[1]), tan(a[2]), tan(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> sqrt(TVec4<T> const & a) noexcept
	{
		return { sqrt(a[0]), sqrt(a[1]), sqrt(a[2]), sqrt(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> inversesqrt(TVec4<T> const & a) noexcept
	{
		return { inversesqrt(a[0]), inversesqrt(a[1]), inversesqrt(a[2]), inversesqrt(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> abs(TVec4<T> const & a, TVec4<T> const & b) noexcept
	{
		return { abs(a[0]), abs(a[1]), abs(a[2]), abs(a[3]) };
	}

	template <class T
	> NODISCARD TVec4<T> pow(TVec4<T> const & a, T const b) noexcept
	{
		return { pow(a[0], b), pow(a[1], b), pow(a[2], b), pow(a[3], b) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD T dot(TVec4<T> const & a, TVec4<T> const & b) noexcept
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr bool operator==(TVec4<A> const & a, TVec4<B> const & b) noexcept
	{
		return (std::addressof(a) == std::addressof(b)) || (a[0] == (A)b[0] && a[1] == (A)b[1] && a[2] == (A)b[2] && a[3] == (A)b[3]);
	}

	template <class A, class B
	> constexpr bool operator!=(TVec4<A> const & a, TVec4<B> const & b) noexcept
	{
		return !(a == b);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr TVec4<A> & operator+=(TVec4<A> & a, TVec4<B> const & b) noexcept
	{
		a[0] += (A)b[0];
		a[1] += (A)b[1];
		a[2] += (A)b[2];
		a[3] += (A)b[3];
		return a;
	}

	template <class A, class B
	> constexpr TVec4<A> & operator-=(TVec4<A> & a, TVec4<B> const & b) noexcept
	{
		a[0] -= (A)b[0];
		a[1] -= (A)b[1];
		a[2] -= (A)b[2];
		a[3] -= (A)b[3];
		return a;
	}

	template <class A, class B
	> constexpr TVec4<A> & operator*=(TVec4<A> & a, TVec4<B> const & b) noexcept
	{
		a[0] *= (A)b[0];
		a[1] *= (A)b[1];
		a[2] *= (A)b[2];
		a[3] *= (A)b[3];
		return a;
	}

	template <class A, class B
	> constexpr TVec4<A> & operator/=(TVec4<A> & a, TVec4<B> const & b) noexcept
	{
		a[0] /= (A)b[0];
		a[1] /= (A)b[1];
		a[2] /= (A)b[2];
		a[3] /= (A)b[3];
		return a;
	}

	template <class A, class B
	> constexpr TVec4<A> & operator%=(TVec4<A> & a, TVec4<B> const & b) noexcept
	{
		a[0] %= (A)b[0];
		a[1] %= (A)b[1];
		a[2] %= (A)b[2];
		a[3] %= (A)b[3];
		return a;
	}

	template <class A, class B
	> constexpr TVec4<A> & operator^=(TVec4<A> & a, TVec4<B> const & b) noexcept
	{
		a[0] ^= (A)b[0];
		a[1] ^= (A)b[1];
		a[2] ^= (A)b[2];
		a[3] ^= (A)b[3];
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator+=(TVec4<A> & a, B const b) noexcept
	{
		a[0] += (A)b;
		a[1] += (A)b;
		a[2] += (A)b;
		a[3] += (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator-=(TVec4<A> & a, B const b) noexcept
	{
		a[0] -= (A)b;
		a[1] -= (A)b;
		a[2] -= (A)b;
		a[3] -= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator*=(TVec4<A> & a, B const b) noexcept
	{
		a[0] *= (A)b;
		a[1] *= (A)b;
		a[2] *= (A)b;
		a[3] *= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator/=(TVec4<A> & a, B const b) noexcept
	{
		a[0] /= (A)b;
		a[1] /= (A)b;
		a[1] /= (A)b;
		a[3] /= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator%=(TVec4<A> & a, B const b) noexcept
	{
		a[0] %= (A)b;
		a[1] %= (A)b;
		a[2] %= (A)b;
		a[3] %= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec4<A> & operator^=(TVec4<A> & a, B const b) noexcept
	{
		a[0] ^= (A)b;
		a[1] ^= (A)b;
		a[2] ^= (A)b;
		a[3] ^= (A)b;
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // _PN_MATRIX_HPP_
