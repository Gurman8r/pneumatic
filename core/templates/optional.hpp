#ifndef _PN_OPTIONAL_HPP_
#define _PN_OPTIONAL_HPP_

#include <core/templates/utility.hpp>

#include <optional>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using typename std::nullopt_t;

	using std::nullopt;

	template <class T
	> using Optional = std::optional<T>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> constexpr auto make_optional(T && value)
	{
		return Optional<std::decay_t<T>>{ FWD(value) };
	}

	template <class T, class ... Args
	> constexpr auto make_optional(Args && ... args)
	{
		return Optional<T>{ std::in_place, FWD(args)... };
	}

	template <class T, class Elem, class ... Args
	> constexpr auto make_optional(std::initializer_list<Elem> init, Args&&... args)
	{
		return Optional<T>{ std::in_place, init, FWD(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_OPTIONAL_HPP_
