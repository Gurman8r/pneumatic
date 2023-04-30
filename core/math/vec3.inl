#ifdef _PN_MATRIX_HPP_

/* VEC3 */

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> inline TVec3<T> operator-(TVec3<T> const & a) noexcept { return { -a[0], -a[1], -a[2] }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD TVec3<T> asin(TVec3<T> const & a) noexcept
	{
		return { asin(a[0]), asin(a[1]), asin(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> acos(TVec3<T> const & a) noexcept
	{
		return { acos(a[0]), acos(a[1]), acos(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> atan(TVec3<T> const & a) noexcept
	{
		return { atan(a[0]), atan(a[1]), atan(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> sin(TVec3<T> const & a) noexcept
	{
		return { sin(a[0]), sin(a[1]), sin(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> cos(TVec3<T> const & a) noexcept
	{
		return { cos(a[0]), cos(a[1]), cos(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> tan(TVec3<T> const & a) noexcept
	{
		return { tan(a[0]), tan(a[1]), tan(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> sqrt(TVec3<T> const & a) noexcept
	{
		return { sqrt(a[0]), sqrt(a[1]), sqrt(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> inversesqrt(TVec3<T> const & a) noexcept
	{
		return { inversesqrt(a[0]), inversesqrt(a[1]), inversesqrt(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> abs(TVec3<T> const & a) noexcept
	{
		return { abs(a[0]), abs(a[1]), abs(a[2]) };
	}

	template <class T
	> NODISCARD TVec3<T> pow(TVec3<T> const & a, T const b) noexcept
	{
		return { pow(a[0], b), pow(a[1], b), pow(a[2], b) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD T dot(TVec3<T> const & a, TVec3<T> const & b) noexcept
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr bool operator==(TVec3<A> const & a, TVec3<B> const & b) noexcept
	{
		return (std::addressof(a) == std::addressof(b)) || (a[0] == (A)b[0] && a[1] == (A)b[1] && a[2] == (A)b[2]);
	}

	template <class A, class B
	> constexpr bool operator!=(TVec3<A> const & a, TVec3<B> const & b) noexcept
	{
		return !(a == b);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr bool operator==(TVec3<A> const & a, B const b) noexcept
	{
		return (a[0] == (A)b && a[1] == (A)b && a[2] == (A)b);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr TVec3<A> & operator+=(TVec3<A> & a, TVec3<B> const & b) noexcept
	{
		a[0] += (A)b[0];
		a[1] += (A)b[1];
		a[2] += (A)b[2];
		return a;
	}

	template <class A, class B
	> constexpr TVec3<A> & operator-=(TVec3<A> & a, TVec3<B> const & b) noexcept
	{
		a[0] -= (A)b[0];
		a[1] -= (A)b[1];
		a[2] -= (A)b[2];
		return a;
	}

	template <class A, class B
	> constexpr TVec3<A> & operator*=(TVec3<A> & a, TVec3<B> const & b) noexcept
	{
		a[0] *= (A)b[0];
		a[1] *= (A)b[1];
		a[2] *= (A)b[2];
		return a;
	}

	template <class A, class B
	> constexpr TVec3<A> & operator/=(TVec3<A> & a, TVec3<B> const & b) noexcept
	{
		a[0] /= (A)b[0];
		a[1] /= (A)b[1];
		a[2] /= (A)b[2];
		return a;
	}

	template <class A, class B
	> constexpr TVec3<A> & operator%=(TVec3<A> & a, TVec3<B> const & b) noexcept
	{
		a[0] %= (A)b[0];
		a[1] %= (A)b[1];
		a[2] %= (A)b[2];
		return a;
	}

	template <class A, class B
	> constexpr TVec3<A> & operator^=(TVec3<A> & a, TVec3<B> const & b) noexcept
	{
		a[0] ^= (A)b[0];
		a[1] ^= (A)b[1];
		a[2] ^= (A)b[2];
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator+=(TVec3<A> & a, B const b) noexcept
	{
		a[0] += (A)b;
		a[1] += (A)b;
		a[2] += (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator-=(TVec3<A> & a, B const b) noexcept
	{
		a[0] -= (A)b;
		a[1] -= (A)b;
		a[2] -= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator*=(TVec3<A> & a, B const b) noexcept
	{
		a[0] *= (A)b;
		a[1] *= (A)b;
		a[2] *= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator/=(TVec3<A> & a, B const b) noexcept
	{
		a[0] /= (A)b;
		a[1] /= (A)b;
		a[1] /= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator%=(TVec3<A> & a, B const b) noexcept
	{
		a[0] %= (A)b;
		a[1] %= (A)b;
		a[2] %= (A)b;
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TVec3<A> & operator^=(TVec3<A> & a, B const b) noexcept
	{
		a[0] ^= (A)b;
		a[1] ^= (A)b;
		a[2] ^= (A)b;
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // _PN_MATRIX_HPP_
