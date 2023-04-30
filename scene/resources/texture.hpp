#ifndef _PNU_TEXTURE_HPP_
#define _PNU_TEXTURE_HPP_

#include <servers/rendering_server.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// texture
	class PNU_API Texture : public Resource
	{
		DEFINE_CLASS(Texture, Resource);

	protected:
		Texture() noexcept {}

	public:
		virtual ~Texture() noexcept override = default;

		virtual RID get_rid() const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// texture 2D
	class PNU_API Texture2D : public Texture
	{
		DEFINE_CLASS(Texture2D, Texture);

	protected:
		Texture2D() noexcept {}

	public:
		virtual ~Texture2D() noexcept override = default;

		virtual RID get_rid() const = 0;
		virtual i32 get_width() const = 0;
		virtual i32 get_height() const = 0;
		virtual Vec2i get_size() const;
		virtual bool is_pixel_opaque(i32 x, i32 y) const;
		virtual bool has_alpha() const = 0;

		virtual Ref<Image> get_data() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// image texture
	class PNU_API ImageTexture : public Texture2D
	{
		DEFINE_CLASS(ImageTexture, Texture2D);

		RID			m_texture{};
		i32			m_width{};
		i32			m_height{};
		Ref<Image>	m_image_cache{};

	public:
		ImageTexture() noexcept {}
		ImageTexture(String const & path);
		ImageTexture(Ref<Image> const & image);
		virtual ~ImageTexture() override;
		virtual RID get_rid() const override;
		virtual i32 get_width() const override;
		virtual i32 get_height() const override;
		virtual bool has_alpha() const override;
		virtual Ref<Image> get_data() const override;
		void update(Ref<Image> const & image, bool immediate = false);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// texture 3D (NYI)
	class PNU_API Texture3D : public Texture
	{
		DEFINE_CLASS(Texture3D, Texture);

	public:
		Texture3D();

		virtual ~Texture3D() override;

		virtual RID get_rid() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// texture cube (NYI)
	class PNU_API TextureCube : public Texture
	{
		DEFINE_CLASS(TextureCube, Texture);

	public:
		TextureCube();

		virtual ~TextureCube() override;

		virtual RID get_rid() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_TEXTURE_HPP_
