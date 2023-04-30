#include <scene/resources/shader_library.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Shader, t) {}

	Shader::Shader(String const & path)
	{
		ASSERT(ShaderLibrary::load_shader(*this, path) == Error_OK);
	}

	Shader::~Shader()
	{
		if (m_shader) { RENDERING_DEVICE->shader_destroy(m_shader); m_shader = nullptr; }
	}

	String Shader::get_code() const
	{
		return {};
	}

	void Shader::set_code(String const & value)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
