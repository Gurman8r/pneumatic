#ifndef _PNU_ERROR_HPP_
#define _PNU_ERROR_HPP_

// WIP

#include <core/templates/type_traits.hpp>

namespace Pnu
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

#endif // !_PNU_ERROR_HPP_
