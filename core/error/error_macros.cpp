#include <core/error/error_macros.hpp>
#include <core/os/os.hpp>

using namespace Pnu;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Pnu::priv::_crash(cwstring message, cwstring file, u32 line)
{
#if CC_MSVC
	_CSTD _wassert(message, file, line);
#else
#error "crash implementation not found"
#endif
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Pnu::_err_print_error(cstring func, cstring file, u32 line, cstring desc, ErrorHandlerType_ log_type)
{
	get_os()->printerror(func, file, line, desc, "", log_type);
}

void Pnu::_err_print_error(cstring func, cstring file, u32 line, cstring desc, cstring message, ErrorHandlerType_ log_type)
{
	get_os()->printerror(func, file, line, desc, message, log_type);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */