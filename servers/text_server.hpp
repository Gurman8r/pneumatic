#ifndef _PNU_TEXT_SERVER_HPP_
#define _PNU_TEXT_SERVER_HPP_

#include <scene/resources/texture.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PNU_API Glyph : public Texture2D
	{
		DEFINE_CLASS(Glyph, Texture2D);

		RID m_texture{};
		IntRect m_bounds{};
		u32 m_advance{};

	public:
		Glyph() noexcept {}

		Glyph(DynamicBuffer const & buffer, IntRect const & bounds, u32 advance);

		Glyph(Glyph && other) noexcept { swap(other); }

		Glyph & operator=(Glyph && other) noexcept { return swap(other); }

		virtual ~Glyph();

		Glyph & swap(Glyph & other) noexcept
		{
			if (this != std::addressof(other))
			{
				util::swap(m_texture, other.m_texture);
				util::swap(m_bounds, other.m_bounds);
				util::swap(m_advance, other.m_advance);
			}
			return (*this);
		}

		u32 get_advance() const noexcept { return m_advance; }

		virtual RID get_rid() const override { return m_texture; }

		virtual i32 get_width() const override { return m_bounds.width(); }

		virtual i32 get_height() const override { return m_bounds.height(); }

		virtual bool has_alpha() const override { return false; }

		virtual Ref<Image> get_data() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PNU_API TextServer : public Object
	{
		DEFINE_CLASS(TextServer, Object);

		static TextServer * __singleton;

	public:
		TextServer();
		virtual ~TextServer() override;
		FORCE_INLINE static TextServer * get_singleton() noexcept { return __singleton; }

	public:
		/* FONT API */
		using FontLibrary = void *;

		using FontFace = void *;

		struct FontPage
		{
			u32 character_size{};

			Map<u32, Glyph> glyphs{};
		};

		struct Font
		{
			String path{};

			FontLibrary font_library{};

			FontFace font_face{};

			Map<u32, FontPage> pages{};
		};

		struct FontCreateInfo
		{
			String path{};
		};

		RID font_create(FontCreateInfo const & spec);
		void font_destroy(RID font);
		FontPage * font_get_page(RID font, u32 character_size);
		Glyph * font_get_glyph(RID font, u32 character, u32 character_size);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using TS = TextServer;

	SINGLETON_WRAPPER(TS, get_text_server);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_TEXT_SERVER_HPP_
