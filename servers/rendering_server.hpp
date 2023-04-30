#ifndef _PNU_RENDERING_SERVER_HPP_
#define _PNU_RENDERING_SERVER_HPP_

#include <core/io/image.hpp>
#include <servers/rendering/rendering_device.hpp>
#include <servers/rendering/shader_language.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// high-level graphics api
	class PNU_API RenderingServer : public Object
	{
		DEFINE_CLASS(RenderingServer, Object);

		static RenderingServer * __singleton;

	protected:
		using CreateFunc = RenderingServer * (*)();
		static CreateFunc __create_func;
		RenderingServer();

	public:
		virtual ~RenderingServer() override;
		FORCE_INLINE static RenderingServer * get_singleton() noexcept { return __singleton; }
		static RenderingServer * create();

		virtual void initialize() = 0;
		virtual void finalize() = 0;

	public:
		/* TEXTURE */
		virtual void texture_destroy(RID texture) = 0;
		virtual RID texture_replace(RID old_texture, RID new_texture) = 0;
		virtual RID texture2d_create(Ref<Image> const & image) = 0;
		virtual RID texture2d_placeholder_create() = 0;
		virtual Ref<Image> texture2d_get_data(RID texture) = 0;
		virtual RID texture3d_placeholder_create() = 0;
		virtual RID texturecube_placeholder_create() = 0;

	public:
		/* SHADER */
		virtual RID shader_create() = 0;
		virtual RID shader_placeholder_create() = 0;
		virtual void shader_destroy(RID shader) = 0;

	public:
		/* MATERIAL */
		virtual RID material_create() = 0;
		virtual RID material_placeholder_create() = 0;
		virtual void material_destroy(RID material) = 0;
		virtual RID material_get_shader(RID material) const = 0;
		virtual void material_set_shader(RID material, RID shader) = 0;
		virtual Var material_get_param(RID material, String const & key) const = 0;
		virtual void material_set_param(RID material, String const & key, Var const & value) = 0;
		virtual void material_update(RID material, Map<String, Var> const & params) = 0;

	public:
		/* MESH */
		enum Primitive_
		{
			Primitive_Points,
			Primitive_Lines,
			Primitive_LineStrip,
			Primitive_Triangles,
			Primitive_TriangleStrip,
			Primitive_MAX
		};

		struct SurfaceData
		{
			Primitive_ primitive{};

			DynamicBuffer vertex_data{};
			size_t vertex_count{};

			DynamicBuffer index_data{};
			size_t index_count{};

			RD::VertexLayout layout{};

			RID material{};
		};

		virtual RID mesh_create(Vector<SurfaceData> const & surfaces = {}) = 0;
		virtual RID mesh_placeholder_create() = 0;
		virtual void mesh_destroy(RID mesh) = 0;
		virtual void mesh_clear(RID mesh) = 0;
		virtual void mesh_add_surface(RID mesh, SurfaceData const & surface) = 0;
		virtual size_t mesh_get_surface_count(RID mesh) = 0;
		virtual Primitive_ mesh_surface_get_primitive(RID mesh, size_t index) = 0;
		virtual RID mesh_surface_get_vertex_array(RID mesh, size_t index) = 0;
		virtual RID mesh_surface_get_index_array(RID mesh, size_t index) = 0;
		virtual RID mesh_surface_get_material(RID mesh, size_t index) = 0;

	public:
		/* CAMERA */
		virtual RID camera_create() = 0;
		virtual void camera_destroy(RID camera) = 0;
		virtual void camera_set_perspective(RID camera, f32 fov, f32 znear, f32 zfar) = 0;
		virtual void camera_set_orthographic(RID camera, f32 size, f32 znear, f32 zfar) = 0;
		virtual void camera_set_frustum(RID camera, f32 size, Vec2 offset, f32 znear, f32 zfar) = 0;
		virtual void camera_set_transform(RID camera, Mat4 const & transform) = 0;

	public:
		/* VIEWPORT */
		virtual RID viewport_create() = 0;
		virtual void viewport_destroy(RID viewport) = 0;
		virtual void viewport_set_camera(RID viewport, RID camera) = 0;
		virtual void viewport_set_parent_viewport(RID viewport, RID parent_viewport) = 0;
		virtual void viewport_set_size(RID viewport, i32 width, i32 height) = 0;
		virtual RID viewport_get_texture(RID viewport) const = 0;
		virtual void viewport_attach_to_screen(RID viewport, IntRect const & rect, DS::WindowID screen) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using RS = RenderingServer;

	SINGLETON_WRAPPER(RS, get_rendering_server);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_RENDERING_SERVER_HPP_
