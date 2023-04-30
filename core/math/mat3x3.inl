#ifdef _PN_MATRIX_HPP_

/* MAT3 */

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> constexpr bool operator==(TMat3x3<A> const & a, TMat3x3<B> const & b) noexcept
	{
		return (std::addressof(a) == std::addressof(b)) || (
			a[0] == (A)b[0] && a[1] == (A)b[1] && a[2] == (A)b[2] &&
			a[3] == (A)b[3] && a[4] == (A)b[4] && a[5] == (A)b[5] &&
			a[6] == (A)b[6] && a[7] == (A)b[7] && a[8] == (A)b[8]);
	}

	template <class A, class B
	> constexpr bool operator!=(TMat3x3<A> const & a, TMat3x3<B> const & b) noexcept
	{
		return !(a == b);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B
	> TMat3x3<A> & operator+=(TMat3x3<A> & a, TMat3x3<B> const & b) noexcept
	{
		return (((glm::tmat3x3<A> &)a) += ((glm::tmat3x3<B> const &)b)), a;
	}

	template <class A, class B
	> TMat3x3<A> & operator-=(TMat3x3<A> & a, TMat3x3<B> const & b) noexcept
	{
		return (((glm::tmat3x3<A> &)a) -= ((glm::tmat3x3<B> const &)b)), a;
	}

	template <class A, class B
	> TMat3x3<A> & operator*=(TMat3x3<A> & a, TMat3x3<B> const & b) noexcept
	{
		return (((glm::tmat3x3<A> &)a) *= ((glm::tmat3x3<B> const &)b)), a;
	}

	template <class A, class B
	> TMat3x3<A> & operator/=(TMat3x3<A> & a, TMat3x3<B> const & b) noexcept
	{
		return (((glm::tmat3x3<A> &)a) /= ((glm::tmat3x3<B> const &)b)), a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat3x3<A> & operator+=(TMat3x3<A> & a, B const b) noexcept
	{
		for (auto & e : a) { e += (A)b; }
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat3x3<A> & operator-=(TMat3x3<A> & a, B const b) noexcept
	{
		for (auto & e : a) { e -= (A)b; }
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat3x3<A> & operator*=(TMat3x3<A> & a, B const b) noexcept
	{
		for (auto & e : a) { e *= (A)b; }
		return a;
	}

	template <class A, class B, std::enable_if_t<mpl::is_number_v<B>, int> = 0
	> constexpr TMat3x3<A> & operator/=(TMat3x3<A> & a, B const b) noexcept
	{
		for (auto & e : a) { e /= (A)b; }
		return a;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // _PN_MATRIX_HPP_
