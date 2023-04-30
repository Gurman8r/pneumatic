#ifdef _PNU_MATRIX_HPP_

/* MAT2 */

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr bool operator==(TMat2x2<A> const & a, TMat2x2<B> const & b) noexcept
	{
		return (std::addressof(a) == std::addressof(b)) || (
			a[0] == (A)b[0] && a[1] == (A)b[1] &&
			a[2] == (A)b[2] && a[3] == (A)b[3]);
	}

	template <class A, class B
	> constexpr bool operator!=(TMat2x2<A> const & a, TMat2x2<B> const & b) noexcept
	{
		return !(a == b);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> TMat2x2<A> & operator+=(TMat2x2<A> & a, TMat2x2<B> const & b) noexcept
	{
		return (((glm::tmat2x2<A> &)a) += ((glm::tmat2x2<B> const &)b)), a;
	}

	template <class A, class B
	> TMat2x2<A> & operator-=(TMat2x2<A> & a, TMat2x2<B> const & b) noexcept
	{
		return (((glm::tmat2x2<A> &)a) -= ((glm::tmat2x2<B> const &)b)), a;
	}

	template <class A, class B
	> TMat2x2<A> & operator*=(TMat2x2<A> & a, TMat2x2<B> const & b) noexcept
	{
		return (((glm::tmat2x2<A> &)a) *= ((glm::tmat2x2<B> const &)b)), a;
	}

	template <class A, class B
	> TMat2x2<A> & operator/=(TMat2x2<A> & a, TMat2x2<B> const & b) noexcept
	{
		return (((glm::tmat2x2<A> &)a) /= ((glm::tmat2x2<B> const &)b)), a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat2x2<A> & operator+=(TMat2x2<A> & a, B const b) noexcept
	{
		for (auto & e : a) { e += (A)b; }
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat2x2<A> & operator-=(TMat2x2<A> & a, B const b) noexcept
	{
		for (auto & e : a) { e -= (A)b; }
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat2x2<A> & operator*=(TMat2x2<A> & a, B const b) noexcept
	{
		for (auto & e : a) { e *= (A)b; }
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat2x2<A> & operator/=(TMat2x2<A> & a, B const b) noexcept
	{
		for (auto & e : a) { e /= (A)b; }
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // _PNU_MATRIX_HPP_
