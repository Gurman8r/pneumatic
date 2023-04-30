#ifndef _PNU_PRINT_STRING_HPP_
#define _PNU_PRINT_STRING_HPP_

#include <core/string/string_utility.hpp>

#define PRINT_LINE (Pnu::print_line)

#define PRINT_ERROR (Pnu::print_error)

namespace Pnu
{
	PNU_API_FUNC(void) print_line(String const & s);

	PNU_API_FUNC(void) print_error(String const & s);
}

#endif // !_PNU_PRINT_STRING_HPP_
