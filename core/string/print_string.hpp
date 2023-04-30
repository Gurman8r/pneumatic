#ifndef _PN_PRINT_STRING_HPP_
#define _PN_PRINT_STRING_HPP_

#include <core/string/string_utility.hpp>

#define PRINT_LINE (pn::print_line)

#define PRINT_ERROR (pn::print_error)

namespace pn
{
	PN_API_FUNC(void) print_line(String const & s);

	PN_API_FUNC(void) print_error(String const & s);
}

#endif // !_PN_PRINT_STRING_HPP_
