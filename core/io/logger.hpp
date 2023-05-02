#ifndef _PNU_LOGGER_HPP_
#define _PNU_LOGGER_HPP_

#include <core/error/error_macros.hpp>
#include <core/string/string.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PNU_API Logger
	{
	public:
		virtual ~Logger() noexcept = default;

		virtual void log_error(cstring func, cstring file, u32 line, cstring desc, cstring message, ErrorHandlerType_ type = ErrorHandlerType_Error);

		virtual void logv(cstring fmt, va_list args, bool is_error = false) = 0;

		void logf(cstring fmt, ...);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PNU_API StdLogger : public Logger
	{
	public:
		virtual ~StdLogger() noexcept override = default;

		virtual void logv(cstring fmt, va_list args, bool is_error = false) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PNU_API CompositeLogger : public Logger
	{
		Vector<Logger *> m_loggers;

	public:
		CompositeLogger(Vector<Logger *> loggers);

		virtual ~CompositeLogger() override;

		virtual void logv(cstring fmt, va_list args, bool is_error = false) override;

		void add_logger(Logger * value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_LOGGER_HPP_
