#ifndef _PNU_SHADER_LANGUAGE_HPP_
#define _PNU_SHADER_LANGUAGE_HPP_

#include <core/object/var.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PNU_API ShaderLanguage
	{
		mutable Vector<String> m_tokens;

	public:
		virtual ~ShaderLanguage() noexcept = default;

	public:
		enum Token_ : size_t
		{
			Token_Void,
			Token_Bool,
			Token_Int, Token_Vec2i, Token_Vec3i, Token_Vec4i, Token_Mat2i, Token_Mat3i, Token_Mat4i,
			Token_Uint, Token_Vec2u, Token_Vec3u, Token_Vec4u, Token_Mat2u, Token_Mat3u, Token_Mat4u,
			Token_Float, Token_Vec2f, Token_Vec3f, Token_Vec4f, Token_Mat2f, Token_Mat3f, Token_Mat4f,
			Token_Double, Token_Vec2d, Token_Vec3d, Token_Vec4d, Token_Mat2d, Token_Mat3d, Token_Mat4d,
			Token_Texture2D, Token_Texture3D, Token_TextureCube,
			Token_MAX
		};

		virtual void load_tokens(Vector<String> & v) const = 0;

		String const & get_token(Token_ i) const noexcept {
			ASSERT(i < Token_MAX);
			if (m_tokens.empty()) { load_tokens(m_tokens); }
			ASSERT(i < m_tokens.size());
			return m_tokens[i];
		}

	public:
		struct BaseNode
		{
		};

		struct ShaderNode : BaseNode
		{
			struct Uniform
			{
				i32 order{ -1 };
				i32 texture_order{ -1 };
				VarType_ type{ VarType_MAX };
			};
		};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PNU_API ShaderLanguageGLSL : public ShaderLanguage
	{
	public:
		virtual ~ShaderLanguageGLSL() noexcept override = default;

		virtual void load_tokens(Vector<String> & v) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_SHADER_LANGUAGE_HPP_
