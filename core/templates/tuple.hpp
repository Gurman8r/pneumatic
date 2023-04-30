#ifndef _PNU_TUPLE_HPP_
#define _PNU_TUPLE_HPP_

#include <core/templates/utility.hpp>

#include <tuple>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// tuple
	template <class Type0, class ... Types
	> using Tuple = std::tuple<Type0, Types...>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_TUPLE_HPP_
