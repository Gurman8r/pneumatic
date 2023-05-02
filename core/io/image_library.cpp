#include <core/io/image_library.hpp>

#define STBI_FREE memfree
#define STBI_MALLOC memalloc
#define STBI_REALLOC memrealloc
#define STBI_REALLOC_SIZED memrealloc_sized
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ ImageLibrary::load_image(Image & image, String const & path)
	{
		if (path.empty()) {
			return Error_Unknown;
		}

		static ON_SCOPE_ENTER() { stbi_set_flip_vertically_on_load(true); };
		u8 * data{ (u8 *)stbi_load(path.c_str(), &image.m_width, &image.m_height, &image.m_depth, 0) };
		ON_SCOPE_EXIT(&) { stbi_image_free(data); };
		if (!data) {
			return Error_Unknown;
		}

		image.m_pixels = { data, data + image.m_width * image.m_height * image.m_depth };

		// TODO: properly deduce image format
		switch (image.m_depth) {
		case 1: { image.m_format = ImageFormat_R8; } break;
		case 2: { image.m_format = ImageFormat_RG8; } break;
		case 3: { image.m_format = ImageFormat_RGB8; } break;
		case 4: { image.m_format = ImageFormat_RGBA8; } break;
		}

		return Error_OK;
	}

	Error_ ImageLibrary::load_image(Ref<Image> image, String const & path)
	{
		return !image ? Error_Unknown : load_image(**image, path);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ImageFormatLoader, t) {}

	RES ImageFormatLoader::load(String const & path, Error_ * error)
	{
		auto image{ Ref<Image>::new_() };
		ImageLibrary::load_image(image, path);
		if (error) { *error = Error_OK; }
		return image;
	}

	void ImageFormatLoader::get_recognized_extensions(Vector<String> * out) const
	{
		if (!out) { return; }
		out->push_back(".jpg");
		out->push_back(".jpeg");
		out->push_back(".png");
		out->push_back(".bmp");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
