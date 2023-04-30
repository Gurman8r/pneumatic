#ifndef _PNU_IMAGE_HPP_
#define _PNU_IMAGE_HPP_

#include <core/io/resource.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum ImageFormat_
	{
		ImageFormat_L8, //luminance
		ImageFormat_LA8, //luminance-alpha
		ImageFormat_R8,
		ImageFormat_RG8,
		ImageFormat_RGB8,
		ImageFormat_RGBA8,
		ImageFormat_RGBA4444,
		ImageFormat_RGB565,
		ImageFormat_RF, //float
		ImageFormat_RGF,
		ImageFormat_RGBF,
		ImageFormat_RGBAF,
		ImageFormat_RH, //half float
		ImageFormat_RGH,
		ImageFormat_RGBH,
		ImageFormat_RGBAH,
		ImageFormat_RGBE9995,
		ImageFormat_DXT1, //s3tc bc1
		ImageFormat_DXT3, //bc2
		ImageFormat_DXT5, //bc3
		ImageFormat_RGTC_R,
		ImageFormat_RGTC_RG,
		ImageFormat_BPTC_RGBA, //btpc bc7
		ImageFormat_BPTC_RGBF, //float bc6h
		ImageFormat_BPTC_RGBFU, //unsigned float bc6hu
		ImageFormat_PVRTC1_2, //pvrtc1
		ImageFormat_PVRTC1_2A,
		ImageFormat_PVRTC1_4,
		ImageFormat_PVRTC1_4A,
		ImageFormat_ETC, //etc1
		ImageFormat_ETC2_R11, //etc2
		ImageFormat_ETC2_R11S, //signed, NOT srgb.
		ImageFormat_ETC2_RG11,
		ImageFormat_ETC2_RG11S,
		ImageFormat_ETC2_RGB8,
		ImageFormat_ETC2_RGBA8,
		ImageFormat_ETC2_RGB8A1,
		ImageFormat_ETC2_RA_AS_RG, //used to make basis universal happy
		ImageFormat_DXT5_RA_AS_RG, //used to make basis universal happy
		ImageFormat_MAX
	};

	enum ImageInterpolation_
	{
		ImageInterpolation_Nearest,
		ImageInterpolation_Bilinear,
		ImageInterpolation_Cubic,
		ImageInterpolation_Trilinear,
		ImageInterpolation_Lanczos,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PNU_API Image : public Resource
	{
		DEFINE_CLASS(Image, Resource);

		friend class ImageLibrary;

	private:
		i32 m_width{}, m_height{}, m_depth{};
		ImageFormat_ m_format{};
		DynamicBuffer m_pixels{};

	public:
		using iterator = typename decltype(m_pixels)::iterator;
		using const_iterator = typename decltype(m_pixels)::const_iterator;

	public:
		Image() noexcept {}
		explicit Image(String const & path);
		Image(i32 width, i32 height, ImageFormat_ format);
		Image(i32 width, i32 height, ImageFormat_ format, DynamicBuffer const & data);
		virtual RID get_rid() const override { return RID{}; }

		void clear();
		void flip_vertically();
		void flip_horizontally();

		i32 get_width() const noexcept { return m_width; }
		i32 get_height() const noexcept { return m_height; }
		i32 get_depth() const noexcept { return m_depth; }
		i32 get_capacity() const noexcept { return m_width * m_height * m_depth; }
		ImageFormat_ get_format() const noexcept { return m_format; }
		DynamicBuffer const & get_data() const noexcept { return m_pixels; }

		Color32 get_pixel(size_t index) const;
		Color32 get_pixel(size_t x, size_t y) const noexcept { return get_pixel((x + y * m_width) * m_depth); }

		void set_pixel(size_t index, Color32 value);
		void set_pixel(size_t x, size_t y, Color32 value) noexcept { set_pixel(((x + y * m_width) * m_depth), value); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_IMAGE_HPP_
