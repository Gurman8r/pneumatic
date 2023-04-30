#ifndef _PN_SHADER_LIBRARY_HPP_
#define _PN_SHADER_LIBRARY_HPP_

#include <scene/resources/shader.hpp>

namespace pn
{
	class PN_API ShaderLibrary final
	{
	public:
		static Error_ load_shader(Shader & shader, String const & path);
		static Error_ load_shader(Ref<Shader> shader, String const & path);
	};
}

#endif // !_PN_SHADER_LIBRARY_HPP_
