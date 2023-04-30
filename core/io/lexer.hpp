#ifndef _PN_LEXER_HPP_
#define _PN_LEXER_HPP_

// PLACEHOLDER

#include <core/object/class.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PN_API Lexer : public Object
	{
		DEFINE_CLASS(Lexer, Object);

	protected:
		Lexer() noexcept {}

	public:
		virtual ~Lexer() noexcept = default;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_LEXER_HPP_
