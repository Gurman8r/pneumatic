#include <scene/resources/font_library.hpp>
#include <servers/text_server.hpp>

namespace pn
{
	Error_ FontLibrary::load_font(Font & font, String const & path)
	{
		if (path.empty()) { return Error_Unknown; }
		if (font.m_font) { get_text_server()->font_destroy(font.m_font); }
		font.m_font = get_text_server()->font_create(TS::FontCreateInfo{ path });
		if (!font.m_font) { return Error_Unknown; }
		font.set_path(path);
		return Error_OK;
	}

	Error_ FontLibrary::load_font(Ref<Font> font, String const & path)
	{
		return !font ? Error_Unknown : load_font(**font, path);
	}
}