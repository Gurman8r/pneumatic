#ifndef _PNU_ERROR_MACROS_HPP_
#define _PNU_ERROR_MACROS_HPP_

#include <core/error/error.hpp>
#include <core/string/print_string.hpp>
#include <cassert>
#include <stdexcept>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// crash implementation
	namespace priv
	{
		PNU_API_FUNC(void) _crash(cwstring message, cwstring file, u32 line);
	}

	// crash
#define CRASH(m_message) \
		(Pnu::priv::_crash)(WIDE(m_message), WIDE(__FILE__), __LINE__)

	// debug crash
#if DEBUG_ENABLED
#define DEBUG_CRASH(m_message) CRASH(m_message)
#else
#define DEBUG_CRASH(m_message) UNUSED(0)
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// assert
#define ASSERT(m_expr) \
		BRANCHLESS_IF(!(m_expr), CRASH(TOSTR(m_expr)))

	// debug assert
#if DEBUG_ENABLED
#define DEBUG_ASSERT(m_expr) ASSERT(m_expr)
#else
#define DEBUG_ASSERT(m_expr) UNUSED(0)
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// validate implementation
	namespace priv
	{
		template <class T
		> auto _validate(T && expr, cwstring message, cwstring file, u32 line) noexcept -> decltype(FWD(expr))
		{
			return BRANCHLESS_IF(!(expr), _crash(message, file, line)), FWD(expr);
		}
	}

	// validate
#define VALIDATE(m_expr) \
		(Pnu::priv::_validate)((m_expr), WIDE(TOSTR(m_expr)), WIDE(__FILE__), __LINE__)

	// debug validate
#if DEBUG_ENABLED
#define DEBUG_VALIDATE(m_expr) VALIDATE(m_expr)
#else
#define DEBUG_VALIDATE(m_expr) (m_expr)
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// verify
#define VERIFY(m_expr, m_message)												\
		do {																	\
			if (m_expr) { /* contextually convertible to bool paranoia */ }		\
			else {																\
				Pnu::priv::_crash(WIDE(m_message), WIDE(__FILE__), __LINE__);	\
			}																	\
		} while (0)

	// debug verify
#if DEBUG_ENABLED
#define DEBUG_VERIFY(m_expr, m_message) VERIFY(m_expr, m_message)
#else
#define DEBUG_VERIFY(m_expr, m_message) UNUSED(0)
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// verify range
#define VERIFY_RANGE(m_expr, m_min, m_max) \
		(UNUSED((!!(( \
		static_cast<decltype(m_expr)>(m_min) < (m_expr)) && \
		((m_expr) < static_cast<decltype(m_expr)>(m_max)))) || \
		(CRASH("range error: \'" TOSTR(m_min) "\' < \'" TOSTR(m_expr) "\' < \'" TOSTR(m_max) "\'"), 0) \
		))

	// debug verify range
#if DEBUG_ENABLED
#define DEBUG_VERIFY_RANGE(m_expr, m_min, m_max) VERIFY_RANGE(m_expr, m_min, m_max)
#else
#define DEBUG_VERIFY_RANGE(m_expr, m_min, m_max) UNUSED(0)
#endif

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum ErrorHandlerType_
	{
		ErrorHandlerType_Error,
		ErrorHandlerType_Warning,
		ErrorHandlerType_MAX
	};

	PNU_API_FUNC(void) _err_print_error(cstring func, cstring file, u32 line, cstring desc, ErrorHandlerType_ log_type = ErrorHandlerType_Error);
	PNU_API_FUNC(void) _err_print_error(cstring func, cstring file, u32 line, cstring desc, cstring m_message, ErrorHandlerType_ log_type = ErrorHandlerType_Error);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ERR_PRINT_ERROR(m_desc) \
		(Pnu::_err_print_error(__PRETTY_FUNCTION__, __FILE__, __LINE__, (m_desc), Pnu::ErrorHandlerType_Error))

#define ERR_PRINT_ERROR_MSG(m_desc, m_message) \
		(Pnu::_err_print_error(__PRETTY_FUNCTION__, __FILE__, __LINE__, (m_desc), (m_message), Pnu::ErrorHandlerType_Error))

#define ERR_PRINT_WARNING(m_desc) \
		(Pnu::_err_print_error(__PRETTY_FUNCTION__, __FILE__, __LINE__, (m_desc), Pnu::ErrorHandlerType_Warning))

#define ERR_PRINT_WARNING_MSG(m_desc, m_message) \
		(Pnu::_err_print_error(__PRETTY_FUNCTION__, __FILE__, __LINE__, (m_desc), (m_message), Pnu::ErrorHandlerType_Error))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ERROR_MACROS_HPP_
