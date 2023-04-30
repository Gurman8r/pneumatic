#ifndef _PN_MATERIAL_HPP_
#define _PN_MATERIAL_HPP_

#include <scene/resources/shader.hpp>
#include <scene/resources/texture.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PN_API Material : public Resource
	{
		DEFINE_CLASS(Material, Resource);

		RID m_material;

	protected:
		Material();

	public:
		virtual ~Material();

		virtual RID get_rid() const override { return m_material; }

		virtual RID get_shader_rid() const = 0;

		virtual Shader::Mode_ get_shader_mode() const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PN_API ShaderMaterial : public Material
	{
		DEFINE_CLASS(ShaderMaterial, Material);

		Ref<Shader> m_shader;

	public:
		ShaderMaterial();
		explicit ShaderMaterial(String const & path);
		virtual ~ShaderMaterial() override;
		virtual RID get_shader_rid() const override;
		virtual Shader::Mode_ get_shader_mode() const override;

	public:
		Ref<Shader> get_shader() const;
		void set_shader(Ref<Shader> const & value);

		Var get_shader_param(String const & key) const;
		void set_shader_param(String const & key, Var const & value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PN_API StandardMaterial3D : public Material
	{
		DEFINE_CLASS(StandardMaterial3D, Material);

	public:
		enum Param_
		{
			Param_Albedo,
			Param_Specular,
			Param_Metallic,
			// etc...
			Param_MAX
		};

		static constexpr cstring parameter_names[Param_MAX]
		{
			"Albedo",
			"Specular",
			"Metallic",
			// etc...
		};

	public:
		StandardMaterial3D();
		explicit StandardMaterial3D(String const & path);
		virtual ~StandardMaterial3D() override;
		virtual RID get_shader_rid() const override;
		virtual Shader::Mode_ get_shader_mode() const override;

	private:
		void _update_shader();

		struct Params
		{
			Color albedo{};
			f32 specular{};
			f32 metallic{};
			// etc...
		}
		m_params{};

	public:
		Color get_albedo() const { return m_params.albedo; }
		void set_albedo(Color const & value);

		f32 get_specular() const { return m_params.specular; }
		void set_specular(f32 value);

		f32 get_metallic() const { return m_params.metallic; }
		void set_metallic(f32 value);

		// etc...
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_MATERIAL_HPP_
