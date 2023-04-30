#include <scene/resources/font_library.hpp>
#include <servers/text_server.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Font, t) {}

	Font::~Font()
	{
		if (m_font) { get_text_server()->font_destroy(m_font); m_font = nullptr; }
	}

	Font::Font(String const & path)
	{
		ASSERT(FontLibrary::load_font(*this, path) == Error_OK);
	}

	Glyph * Font::get_glyph(u32 character, u32 character_size)
	{
		if (!m_font || !character || !character_size) { return nullptr; }

		return get_text_server()->font_get_glyph(m_font, character, character_size);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
