#ifndef _PNU_LEXER_HPP_
#define _PNU_LEXER_HPP_

// PLACEHOLDER

#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PNU_API Lexer : public Object
	{
		DEFINE_CLASS(Lexer, Object);

	protected:
		Lexer() noexcept {}

	public:
		virtual ~Lexer() noexcept = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_LEXER_HPP_
