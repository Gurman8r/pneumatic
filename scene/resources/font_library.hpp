#ifndef _PN_FONT_LIBRARY_HPP_
#define _PN_FONT_LIBRARY_HPP_

#include <scene/resources/font.hpp>

namespace pn
{
	class PN_API FontLibrary final
	{
	public:
		static Error_ load_font(Font & font, String const & path);
		static Error_ load_font(Ref<Font> font, String const & path);
	};
}

#endif // !_PN_FONT_LIBRARY_HPP_
