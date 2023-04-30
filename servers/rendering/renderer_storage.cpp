#include <servers/rendering/renderer_storage.hpp>
#include <servers/rendering_server.hpp>
#include <core/math/transform.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(RendererStorage, t) {}

	RendererStorage::RendererStorage(RenderingDevice * device) : m_device{ VALIDATE(device) }
	{
	}

	RendererStorage::~RendererStorage()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RID RendererStorage::texture2d_placeholder_create()
	{
		return nullptr;
	}

	RID RendererStorage::texture3d_placeholder_create()
	{
		return nullptr;
	}

	RID RendererStorage::texturecube_placeholder_create()
	{
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RID RendererStorage::shader_placeholder_create()
	{
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RID RendererStorage::material_create()
	{
		RID const material{ (RID)memnew(_Material{}) };
		_Material & m{ *VALIDATE((_Material *)material) };
		return material;
	}

	RID RendererStorage::material_placeholder_create()
	{
		return nullptr;
	}

	void RendererStorage::material_destroy(RID material)
	{
		_Material & m{ *VALIDATE((_Material *)material) };
		if (m.uniform_buffer) { m_device->buffer_destroy(m.uniform_buffer); }
		if (m.uniform_set) { m_device->uniform_set_destroy(m.uniform_set); }
		memdelete((_Material *)material);
	}

	RID RendererStorage::material_get_shader(RID material)
	{
		_Material & m{ *VALIDATE((_Material *)material) };
		return m.shader;
	}

	void RendererStorage::material_set_shader(RID material, RID shader)
	{
		_Material & m{ *VALIDATE((_Material *)material) };
		if (m.shader == shader) { return; }
		m.shader = shader;
	}

	Var RendererStorage::material_get_param(RID material, String const & key)
	{
		_Material & m{ *VALIDATE((_Material *)material) };

		if (auto const it{ m.params.find(key) }; it != m.params.end())
		{
			return it->second;
		}
		else
		{
			return nullptr;
		}
	}

	void RendererStorage::material_set_param(RID material, String const & key, Var const & value)
	{
		_Material & m{ *VALIDATE((_Material *)material) };

		if (value)
		{
			m.params[key] = value;
		}
		else if (auto const it{ m.params.find(key) }; it != m.params.end())
		{
			m.params.erase(it);
		}
	}

	void RendererStorage::material_update(RID material, Map<String, Var> const & params)
	{
		_Material & m{ *VALIDATE((_Material *)material) };
		m.texture_cache.clear();

		Vector<RD::Uniform> uniforms{};
		size_t buffer_size{};

		for (auto const & [key, value] : params)
		{
			if (!key || !value) { continue; }

			m.params[key] = value;

			if (!value.is_rid())
			{
				buffer_size += SIZE_ROUND_UP(value.size(), 16);
			}
			else
			{
				m.texture_cache.push_back(value.get<RID>());
			}
		}

		//m.uniform_buffer = m_device->uniform_buffer_create(buffer_size);
		//// TODO: write buffer here
		//if (m.uniform_set) { m_device->uniform_set_destroy(m.uniform_set); }
		//m.uniform_set = m_device->uniform_set_create(uniforms, m.shader);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RID RendererStorage::mesh_create(Vector<RS::SurfaceData> const & surfaces)
	{
		RID const mesh{ (RID)memnew(_Mesh{}) };
		_Mesh & m{ *VALIDATE((_Mesh *)mesh) };
		for (RS::SurfaceData const & surface : surfaces) {
			mesh_add_surface(mesh, surface);
		}
		return mesh;
	}

	RID RendererStorage::mesh_placeholder_create()
	{
		return nullptr;
	}

	void RendererStorage::mesh_destroy(RID mesh)
	{
		mesh_clear(mesh);

		memdelete((_Mesh *)mesh);
	}

	void RendererStorage::mesh_clear(RID mesh)
	{
		_Mesh & m{ *VALIDATE((_Mesh *)mesh) };
		for (auto const s : m.surfaces) {
			if (!s) { continue; }
			if (s->vertex_array) { m_device->vertex_array_destroy(s->vertex_array); }
			if (s->index_array) { m_device->index_array_destroy(s->index_array); }
			if (s->uniform_set) { m_device->uniform_set_destroy(s->uniform_set); }
			memdelete(s);
		}
		m.surfaces.clear();
	}

	void RendererStorage::mesh_add_surface(RID mesh, RS::SurfaceData const & surface)
	{
		_Mesh & m{ *VALIDATE((_Mesh *)mesh) };
		_Mesh::Surface * const s{ m.surfaces.emplace_back(memnew(_Mesh::Surface{})) };
		s->primitive = surface.primitive;

		// vertices
		RID vb{ m_device->vertex_buffer_create(surface.vertex_data.size(), surface.vertex_data) };
		RID va{ m_device->vertex_array_create(surface.vertex_count, surface.layout, { vb }) };
		s->vertex_array = va;
		s->vertex_count = surface.vertex_count;
		s->vertex_buffer_size = surface.vertex_data.size();

		// indices
		if (0 < surface.index_count) {
			RID ib{ m_device->index_buffer_create(surface.index_count, DataType_U32, surface.index_data) };
			RID ia{ m_device->index_array_create(ib, 0, surface.index_count) };
			s->index_array = ia;
			s->index_count = surface.index_count;
			s->index_buffer_size = surface.index_data.size();
		}

		// material
		s->material = nullptr;
		s->uniform_set = nullptr;
	}

	size_t RendererStorage::mesh_get_surface_count(RID mesh)
	{
		_Mesh & m{ *VALIDATE((_Mesh *)mesh) };
		return m.surfaces.size();
	}

	RS::Primitive_ RendererStorage::mesh_surface_get_primitive(RID mesh, size_t index)
	{
		_Mesh & m{ *VALIDATE((_Mesh *)mesh) };
		ASSERT(index < m.surfaces.size());
		return m.surfaces[index]->primitive;
	}

	RID RendererStorage::mesh_surface_get_vertex_array(RID mesh, size_t index)
	{
		_Mesh & m{ *VALIDATE((_Mesh *)mesh) };
		ASSERT(index < m.surfaces.size());
		return m.surfaces[index]->vertex_array;
	}

	RID RendererStorage::mesh_surface_get_index_array(RID mesh, size_t index)
	{
		_Mesh & m{ *VALIDATE((_Mesh *)mesh) };
		ASSERT(index < m.surfaces.size());
		return m.surfaces[index]->index_array;
	}

	RID RendererStorage::mesh_surface_get_material(RID mesh, size_t index)
	{
		_Mesh & m{ *VALIDATE((_Mesh *)mesh) };
		ASSERT(index < m.surfaces.size());
		return m.surfaces[index]->material;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RID RendererStorage::render_target_create()
	{
		RID const render_target{ (RID)memnew(_RenderTarget) };
		_RenderTarget & rt{ *VALIDATE((_RenderTarget *)render_target) };

		if (!rt.texture) { rt.texture = texture2d_placeholder_create(); }

		rt.color_format = RD::DataFormat_R8G8B8_UNORM;
		rt.color_format_srgb = RD::DataFormat_R8G8B8_UNORM;

		rt.color = m_device->texture_create(MAKE(RD::TextureCreateInfo, t) {
			t.color_format = rt.color_format;
			t.color_format_srgb = rt.color_format_srgb;
			t.width = (u32)rt.size[0];
			t.width = (u32)rt.size[1];
			t.usage_flags = RD::TextureFlags_Sampling | RD::TextureFlags_ColorAttachment | RD::TextureFlags_CanCopyFrom;
		});

		rt.framebuffer = m_device->framebuffer_create({ rt.color });

		return render_target;
	}

	void RendererStorage::render_target_destroy(RID render_target)
	{
		_RenderTarget & rt{ *VALIDATE((_RenderTarget *)render_target) };
		if (rt.framebuffer) { m_device->framebuffer_destroy(rt.framebuffer); }
		if (rt.backbuffer) { m_device->framebuffer_destroy(rt.backbuffer); }
		memdelete((_RenderTarget *)render_target);
	}

	Vec2i RendererStorage::render_target_get_position(RID render_target)
	{
		return { 0, 0 };
	}

	void RendererStorage::render_target_set_position(RID render_target, i32 x, i32  y)
	{
		_RenderTarget & rt{ *VALIDATE((_RenderTarget *)render_target) };
	}

	Vec2i RendererStorage::render_target_get_size(RID render_target)
	{
		_RenderTarget & rt{ *VALIDATE((_RenderTarget *)render_target) };
		return rt.size;
	}

	void RendererStorage::render_target_set_size(RID render_target, i32 width, i32 height)
	{
		_RenderTarget & rt{ *VALIDATE((_RenderTarget *)render_target) };
		m_device->framebuffer_set_size(rt.framebuffer, width, height);
		m_device->framebuffer_set_size(rt.backbuffer, width, height);
	}

	RID RendererStorage::render_target_get_texture(RID render_target)
	{
		_RenderTarget & rt{ *VALIDATE((_RenderTarget *)render_target) };

		if (!rt.texture) { rt.texture = texture2d_placeholder_create(); }

		return rt.texture;
	}

	void RendererStorage::render_target_request_clear(RID render_target, Color const & value)
	{
		_RenderTarget & rt{ *VALIDATE((_RenderTarget *)render_target) };
		rt.clear_color = value;
		rt.clear_requested = true;
	}

	void RendererStorage::render_target_disable_clear_request(RID render_target)
	{
		_RenderTarget & rt{ *VALIDATE((_RenderTarget *)render_target) };
		rt.clear_requested = false;
	}

	bool RendererStorage::render_target_is_clear_requested(RID render_target)
	{
		return VALIDATE((_RenderTarget *)render_target)->clear_requested;
	}

	Color RendererStorage::render_target_get_clear_request_color(RID render_target)
	{
		return VALIDATE((_RenderTarget *)render_target)->clear_color;
	}

	void RendererStorage::render_target_do_clear_request(RID render_target)
	{
		_RenderTarget & rt{ *VALIDATE((_RenderTarget *)render_target) };
		if (!rt.clear_requested) { return; }
		m_device->draw_list_begin(rt.framebuffer, RD::InitialAction_Clear, RD::FinalAction_Read, RD::InitialAction_Keep, RD::FinalAction_Discard, { rt.clear_color });
		m_device->draw_list_end();
		rt.clear_requested = false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RID RendererStorage::camera_create()
	{
		_Camera * const c{ memnew(_Camera{}) };
		return (RID)c;
	}

	void RendererStorage::camera_destroy(RID camera)
	{
		_Camera & c{ *VALIDATE((_Camera *)camera) };
		memdelete((_Camera *)camera);
	}

	void RendererStorage::camera_set_perspective(RID camera, f32 fov, f32 znear, f32 zfar)
	{
		_Camera & c{ *VALIDATE((_Camera *)camera) };
		c.type = c.Type_Perspective;
		c.fov = fov;
		c.znear = znear;
		c.zfar = zfar;
	}

	void RendererStorage::camera_set_orthographic(RID camera, f32 size, f32 znear, f32 zfar)
	{
		_Camera & c{ *VALIDATE((_Camera *)camera) };
		c.type = c.Type_Orthographic;
		c.size = size;
		c.znear = znear;
		c.zfar = zfar;
	}

	void RendererStorage::camera_set_frustum(RID camera, f32 size, Vec2 offset, f32 znear, f32 zfar)
	{
		_Camera & c{ *VALIDATE((_Camera *)camera) };
		c.type = c.Type_Frustum;
		c.size = size;
		c.offset = offset;
		c.znear = znear;
		c.zfar = zfar;
	}

	void RendererStorage::camera_set_transform(RID camera, Mat4 const & transform)
	{
		_Camera & c{ *VALIDATE((_Camera *)camera) };
		c.transform = transform;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RID RendererStorage::viewport_create()
	{
		RID const viewport{ (RID)memnew(_Viewport) };
		_Viewport & vp{ *VALIDATE((_Viewport *)viewport) };
		vp.self = viewport;
		vp.parent = nullptr;
		vp.position = {};
		vp.size = { 1280, 720 };
		vp.camera = nullptr;
		vp.render_target = render_target_create();
		vp.render_target_texture = nullptr;
		return viewport;
	}

	void RendererStorage::viewport_destroy(RID viewport)
	{
		_Viewport & vp{ *VALIDATE((_Viewport *)viewport) };
		render_target_destroy(vp.render_target);
		memdelete((_Viewport *)viewport);
	}

	void RendererStorage::viewport_set_parent_viewport(RID viewport, RID parent_viewport)
	{
		_Viewport & vp{ *VALIDATE((_Viewport *)viewport) };
		vp.parent = parent_viewport;
	}

	void RendererStorage::viewport_set_size(RID viewport, i32 width, i32 height)
	{
		_Viewport & vp{ *VALIDATE((_Viewport *)viewport) };
		vp.size = { width, height };
	}

	void RendererStorage::viewport_set_camera(RID viewport, RID camera)
	{
	}

	RID RendererStorage::viewport_get_texture(RID viewport) const
	{
		return VALIDATE((_Viewport const *)viewport)->render_target_texture;
	}

	void RendererStorage::viewport_attach_to_screen(RID viewport, IntRect const & rect, DS::WindowID screen)
	{
	}

	void RendererStorage::draw_viewports()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}