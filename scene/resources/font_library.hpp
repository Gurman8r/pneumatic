#ifndef _PNU_FONT_LIBRARY_HPP_
#define _PNU_FONT_LIBRARY_HPP_

#include <scene/resources/font.hpp>

namespace Pnu
{
	class PNU_API FontLibrary final
	{
	public:
		static Error_ load_font(Font & font, String const & path);
		static Error_ load_font(Ref<Font> font, String const & path);
	};
}

#endif // !_PNU_FONT_LIBRARY_HPP_
