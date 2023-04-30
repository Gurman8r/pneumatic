#ifndef _PN_RENDERING_SERVER_DEFAULT_HPP_
#define _PN_RENDERING_SERVER_DEFAULT_HPP_

#include <servers/rendering/renderer_storage.hpp>
#include <servers/rendering/renderer_2d.hpp>
#include <servers/rendering/renderer_3d.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PN_API RenderingServerDefault : public RenderingServer
	{
		DEFINE_CLASS(RenderingServerDefault, RenderingServer);

	protected:
		RenderingDevice * m_device{};
		RendererStorage * m_storage{};
		Renderer2D * m_canvas{};
		Renderer3D * m_scene{};

	public:
		RenderingServerDefault();
		virtual ~RenderingServerDefault() override;

	public:
		virtual void initialize() override;
		virtual void finalize() override;

	public:
		/* TEXTURE */
		virtual void texture_destroy(RID texture) override;
		virtual RID texture_replace(RID old_texture, RID new_texture) override;
		virtual RID texture2d_create(Ref<Image> const & image) override;
		virtual RID texture2d_placeholder_create() override;
		virtual Ref<Image> texture2d_get_data(RID texture) override;
		virtual RID texture3d_placeholder_create() override;
		virtual RID texturecube_placeholder_create() override;

	public:
		/* SHADER */
		virtual RID shader_create() override;
		virtual RID shader_placeholder_create() override;
		virtual void shader_destroy(RID shader) override;

	public:
		/* MATERIAL */
		virtual RID material_create() override;
		virtual RID material_placeholder_create() override;
		virtual void material_destroy(RID material) override;
		virtual RID material_get_shader(RID material) const override;
		virtual void material_set_shader(RID material, RID shader) override;
		virtual Var material_get_param(RID material, String const & key) const override;
		virtual void material_set_param(RID material, String const & key, Var const & value) override;
		virtual void material_update(RID material, Map<String, Var> const & params) override;

	public:
		/* MESH */
		virtual RID mesh_create(Vector<SurfaceData> const & surfaces = {}) override;
		virtual RID mesh_placeholder_create() override;
		virtual void mesh_destroy(RID mesh) override;
		virtual void mesh_clear(RID mesh) override;
		virtual void mesh_add_surface(RID mesh, SurfaceData const & surface) override;
		virtual size_t mesh_get_surface_count(RID mesh) override;
		virtual RS::Primitive_ mesh_surface_get_primitive(RID mesh, size_t index) override;
		virtual RID mesh_surface_get_vertex_array(RID mesh, size_t index) override;
		virtual RID mesh_surface_get_index_array(RID mesh, size_t index) override;
		virtual RID mesh_surface_get_material(RID mesh, size_t index) override;

	public:
		/* CAMERA */
		virtual RID camera_create() override;
		virtual void camera_destroy(RID camera) override;
		virtual void camera_set_perspective(RID camera, f32 fov, f32 znear, f32 zfar) override;
		virtual void camera_set_orthographic(RID camera, f32 size, f32 znear, f32 zfar) override;
		virtual void camera_set_frustum(RID camera, f32 size, Vec2 offset, f32 znear, f32 zfar) override;
		virtual void camera_set_transform(RID camera, Mat4 const & transform) override;

	public:
		/* VIEWPORT */
		virtual RID viewport_create() override;
		virtual void viewport_destroy(RID viewport) override;
		virtual void viewport_set_parent_viewport(RID viewport, RID parent_viewport) override;
		virtual void viewport_set_size(RID viewport, i32 width, i32 height) override;
		virtual void viewport_set_camera(RID viewport, RID camera) override;
		virtual RID viewport_get_texture(RID viewport) const override;
		virtual void viewport_attach_to_screen(RID viewport, IntRect const & rect, DS::WindowID screen) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_RENDERING_SERVER_DEFAULT_HPP_
