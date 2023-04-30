#ifndef _PNU_FONT_HPP_
#define _PNU_FONT_HPP_

#include <servers/text_server.hpp>

namespace Pnu
{
	class PNU_API Font : public Resource
	{
		DEFINE_CLASS(Font, Resource);

		friend class FontLibrary;

		RID m_font;

	public:
		Font() noexcept {}
		
		explicit Font(String const & path);

		virtual ~Font();

		virtual RID get_rid() const override { return m_font; }

		virtual Glyph * get_glyph(u32 character, u32 character_size);
	};
}

#endif // !_PNU_FONT_HPP_
