#include <core/string/print_string.hpp>
#include <core/os/os.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void pn::print_line(String const & s)
	{
		get_os()->printf("%.*s\n", s.size(), s.data());
	}

	void pn::print_error(String const & s)
	{
		get_os()->printerrf("%.*s\n", s.size(), s.data());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}