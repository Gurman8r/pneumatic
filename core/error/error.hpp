#ifndef _PN_ERROR_HPP_
#define _PN_ERROR_HPP_

// WIP

#include <core/templates/type_traits.hpp>

namespace pn
{
	enum Error_
	{
		Error_Unknown = -1,
		Error_OK = 0,
		Error_EOF,
		// etc...

		Error_MAX
	};
}

#endif // !_PN_ERROR_HPP_
