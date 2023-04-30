#include <servers/text_server.hpp>
#include <servers/rendering_server.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
#include FT_STROKER_H

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(TextServer, t) {}

	TextServer * TextServer::__singleton{};

	TextServer::TextServer() { SINGLETON_CTOR(); }

	TextServer::~TextServer() { SINGLETON_DTOR(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Glyph, t) {}

	Glyph::Glyph(DynamicBuffer const & buffer, IntRect const & bounds, u32 advance)
		: m_bounds{ bounds }
		, m_advance{ advance }
	{
		m_texture = RENDERING_DEVICE->texture_create(MAKE(RD::TextureCreateInfo, t) {
			t.color_format = RD::DataFormat_R8_UNORM;
			t.width = bounds.width();
			t.height = bounds.height();
		}, buffer);
	}

	Glyph::~Glyph()
	{
		if (m_texture) { RENDERING_DEVICE->texture_destroy(m_texture); m_texture = nullptr; }
	}

	Ref<Image> Glyph::get_data() const
	{
		return m_texture ? get_rendering_server()->texture2d_get_data(m_texture) : nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RID TextServer::font_create(FontCreateInfo const & spec)
	{
		FT_Library library;
		FT_Face face;
		FT_Stroker stroker;

		if (FT_Init_FreeType(&library)) {
			get_os()->printerrf("FAILED LOADING FONT LIBRARY: %s", spec.path.c_str());
			return nullptr;
		}

		if (FT_New_Face(library, spec.path.c_str(), 0, &face)) {
			get_os()->printerrf("FAILED LOADING FONT FACE: %s", spec.path.c_str());
			FT_Done_FreeType(library);
			return nullptr;
		}

		if (FT_Stroker_New(library, &stroker)) {
			get_os()->printerrf("FAILED LOADING FONT STROKER: %s", spec.path.c_str());
			FT_Done_Face(face);
			FT_Done_FreeType(library);
			return nullptr;
		}

		if (FT_Select_Charmap(face, FT_ENCODING_UNICODE)) {
			get_os()->printerrf("FAILED SELECTING FONT CHARMAP: %s", spec.path.c_str());
			FT_Stroker_Done(stroker);
			FT_Done_Face(face);
			FT_Done_FreeType(library);
			return nullptr;
		}

		Font * f{ memnew(Font{}) };
		f->path = spec.path;
		f->font_library = (FontLibrary)library;
		f->font_face = (FontFace)face;
		return (RID)f;
	}

	void TextServer::font_destroy(RID font)
	{
		Font * const f{ VALIDATE((Font *)font) };
		if (f->font_library) { FT_Done_FreeType((FT_Library)f->font_library); }
		if (f->font_face) { FT_Done_Face((FT_Face)f->font_library); }
		memdelete(f);
	}

	TextServer::FontPage * TextServer::font_get_page(RID font, u32 character_size)
	{
		return util::getptr(VALIDATE((Font *)font)->pages, character_size);
	}

	Glyph * TextServer::font_get_glyph(RID font, u32 character, u32 character_size)
	{
		Font * const f{ VALIDATE((Font *)font) };
		FontPage * const page{ (FontPage *)font_get_page(font, character_size) };
		if (Glyph * const glyph{ util::getptr(page->glyphs, character) }) { return glyph; }
		else
		{
			FT_Face const face{ (FT_Face)f->font_face };
			FT_Set_Pixel_Sizes(face, 0, character_size);
			if (FT_Load_Char(face, character, FT_LOAD_RENDER)) {
				get_os()->printerrf("FAILED LOADING GLYPH: %s", f->path.c_str());
				return nullptr;
			}

			IntRect bounds{ face->glyph->bitmap_left, face->glyph->bitmap_top, face->glyph->bitmap.width, face->glyph->bitmap.rows };

			DynamicBuffer buffer{ face->glyph->bitmap.buffer, (size_t)(bounds.width() * bounds.height()) };

			u32 advance{ (u32)face->glyph->advance.x };

			return &page->glyphs.insert({ character, Glyph{ buffer, bounds, advance } }).first->second;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}