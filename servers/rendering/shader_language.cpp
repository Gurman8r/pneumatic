#include <servers/rendering/shader_language.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ShaderLanguageGLSL::load_tokens(Vector<String> & v) const
	{
		v.resize(Token_MAX, String{});
		v[Token_Void] = "void"_s;
		v[Token_Bool] = "int"_s;
		v[Token_Int] = "int"_s; v[Token_Vec2i] = "ivec2"_s; v[Token_Vec3i] = "ivec3"_s; v[Token_Vec4i] = "ivec4"_s; v[Token_Mat2i] = "imat2"_s; v[Token_Mat3i] = "imat3"_s; v[Token_Mat4i] = "imat4"_s;
		v[Token_Uint] = "uint"_s; v[Token_Vec2u] = "uvec2"_s; v[Token_Vec3u] = "uvec3"_s; v[Token_Vec4u] = "uvec4"_s; v[Token_Mat2u] = "umat2"_s; v[Token_Mat3u] = "umat3"_s; v[Token_Mat4u] = "umat4"_s;
		v[Token_Float] = "float"_s; v[Token_Vec2f] = "vec2"_s; v[Token_Vec3f] = "vec3"_s; v[Token_Vec4f] = "vec4"_s; v[Token_Mat2f] = "mat2"_s; v[Token_Mat3f] = "mat3"_s; v[Token_Mat4f] = "mat4"_s;
		v[Token_Double] = "double"_s; v[Token_Vec2d] = "dvec2"_s; v[Token_Vec3d] = "dvec3"_s; v[Token_Vec4d] = "dvec4"_s; v[Token_Mat2d] = "dmat2"_s; v[Token_Mat3d] = "dmat3"_s; v[Token_Mat4d] = "dmat4"_s;
		v[Token_Texture2D] = "sampler2D"_s; v[Token_Texture3D] = "sampler3D"_s; v[Token_TextureCube] = "samplerCube"_s;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}