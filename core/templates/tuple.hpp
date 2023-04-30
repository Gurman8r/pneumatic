#ifndef _PN_TUPLE_HPP_
#define _PN_TUPLE_HPP_

#include <core/templates/utility.hpp>

#include <tuple>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// tuple
	template <class Type0, class ... Types
	> using Tuple = std::tuple<Type0, Types...>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_TUPLE_HPP_
