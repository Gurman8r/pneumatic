#if OPENGL_ENABLED
#ifndef _PN_RENDERING_DEVICE_OPENGL_HPP_
#define _PN_RENDERING_DEVICE_OPENGL_HPP_

#include <servers/rendering_server.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PN_API RenderingDeviceOpenGL : public RenderingDevice
	{
		DEFINE_CLASS(RenderingDeviceOpenGL, RenderingDevice);

	public:
		~RenderingDeviceOpenGL();
		RenderingDeviceOpenGL();

	public:
		virtual void initialize() override;
		virtual void finalize() override;

	public:
		/* BUFFER  */
		struct _BufferBase
		{
			u32 handle{};
			u32 buffer_type{};
			u32 usage{};
			u32 size{};
			u32 draw_mode{};
			DynamicBuffer data{};
		};

		virtual RID buffer_create(BufferType_ buffer_type, size_t size_in_bytes, DynamicBuffer const & data = {}) override;
		virtual void buffer_destroy(RID buffer) override;
		virtual void buffer_update(RID buffer, size_t offset, void const * data, size_t size_in_bytes) override;

	public:
		/* VERTEXARRAY  */
		struct _VertexBuffer : _BufferBase {};

		struct _VertexArray
		{
			u32 handle{};
			u32 vertex_count{};
			VertexLayout layout{};
			Vector<RID> buffers{};
		};

		struct _IndexBuffer : _BufferBase
		{
			u32 index_count{};
			u32 index_type{};
		};

		struct _IndexArray
		{
			RID index_buffer{};
			u32 index_offset{};
			u32 index_count{};
			u32 index_type{};
		};

		virtual RID vertex_buffer_create(size_t size_in_bytes, DynamicBuffer const & data = {}) override;
		virtual RID vertex_array_create(size_t vertex_count, VertexLayout const & layout, Vector<RID> const & buffers) override;
		virtual void vertex_array_destroy(RID rid) override;

		virtual RID index_buffer_create(size_t index_count, DataType_ index_type = DataType_U32, DynamicBuffer const & data = {}) override;
		virtual RID index_array_create(RID index_buffer, size_t index_offset, size_t index_count) override;
		virtual void index_array_destroy(RID index_array) override;

	public:
		/* SAMPLER  */
		struct _Sampler
		{
			u32 handle{};
			u32 mag_filter{}, min_filter{}, mip_filter{};
			u32 repeat_s{}, repeat_t{}, repeat_r{};
			f32 lod_bias{};
			bool use_anisotropy{};
			f32 anisotropy_max{};
			bool enable_compare{};
			u32 compare_op{};
			f32 min_lod{}, max_lod{};
			SamplerBorderColor_ border_color{};
			bool unnormalized_uvw{};
		};

		virtual RID sampler_create(SamplerCreateInfo const & sampler_state = {}) override;
		virtual void sampler_destroy(RID sampler) override;

	public:
		/* TEXTURE  */
		struct _Texture
		{
			u32 handle{};
			u32 texture_type{};
			DataFormat_ color_format{}, color_format_srgb{};
			ImageFormat_ image_format{};
			i32 width{}, height{}, depth{}, layers{}, mipmaps{};
			u32 min_filter{}, mag_filter{};
			u32 repeat_s{}, repeat_t{};
			TextureSamples_ samples{};
			Vec4i swizzle_mask{};
			i32 usage_flags{};
			i32 width_2d{ width }, height_2d{ height }; // size override
		};

		virtual RID texture_create(TextureCreateInfo const & spec, DynamicBuffer const & data = {}) override;
		virtual void texture_destroy(RID texture) override;
		virtual void texture_update(RID texture, DynamicBuffer const & data = {}) override;
		void _texture_update(_Texture & t, void const * data);
		virtual void * texture_get_handle(RID texture) override;
		virtual DynamicBuffer texture_get_data(RID texture) override;

	public:
		/* FRAMEBUFFER  */
		struct _Framebuffer
		{
			u32 handle{};
			i32 width{}, height{};
			Vector<RID> texture_attachments{};
		};

		virtual RID framebuffer_create(Vector<RID> const & texture_attachments) override;
		virtual void framebuffer_destroy(RID framebuffer) override;
		virtual void framebuffer_set_size(RID framebuffer, i32 width, i32 height) override;

	public:
		/* SHADER  */
		struct _Shader
		{
			u32 handle{};
			FlatMap<size_t, i32> bindings{};
			Vector<ShaderStageData> stage_data{};
		};

		virtual RID shader_create(ShaderStageData const (&spec)[ShaderStage_MAX] = {}) override;
		virtual void shader_destroy(RID shader) override;
		virtual String shader_get_code(RID shader) override;
		virtual void shader_set_code(RID shader, String const & value) override;
		virtual i32 shader_get_uniform_location(RID shader, String const & name) override;

	public:
		/* UNIFORM  */
		struct _UniformBuffer : _BufferBase {};

		struct _UniformDescriptor
		{
			UniformType_ uniform_type{};
			u32 binding{};
			u32 length{};
			Vector<RID> buffers{};
			Vector<RID> images{};
		};

		struct _UniformSet
		{
			RID shader{};
			Vector<_UniformDescriptor> uniforms{};
		};

		virtual RID uniform_buffer_create(size_t size_in_bytes, DynamicBuffer const & data = {}) override;
		virtual RID uniform_set_create(Vector<Uniform> const & uniforms, RID shader) override;
		virtual void uniform_set_destroy(RID uniform_set) override;
		void _uniform_set_bind(_UniformSet const & us);

	public:
		/* PIPELINE  */
		struct _RenderPipeline
		{
			RID shader{};
			u32 primitive{};
			RasterizationState rasterization_state{};
			MultisampleState multisample_state{};
			DepthStencilState depth_stencil_state{};
			ColorBlendState color_blend_state{};
		};

		virtual RID render_pipeline_create(RID shader, RenderPrimitive_ primitive, RasterizationState const & rasterization_state, MultisampleState const & multisample_state, DepthStencilState const & depth_stencil_state, ColorBlendState const & color_blend_state) override;
		virtual void render_pipeline_destroy(RID pipeline) override;
		void _render_pipeline_bind(_RenderPipeline const & rp);

	public:
		/* DRAWLIST  */
		struct _DrawList
		{
			using Cmd = std::function<void()>;
			Vector<Cmd> command_buffer{};

			IntRect viewport{};

			struct SetState
			{
				RID uniform_set{};
				bool bound{ true };
			};

			struct State
			{
				SetState sets[32]{};
				u32 set_count{};
				RID pipeline{};
				RID pipeline_shader{};
				RID vertex_array{};
				RID index_array{};
			}
			state{};
		};

		Vector<_DrawList> m_draw_list{};

		virtual DrawListID draw_list_begin_for_screen(DS::WindowID window = DS::MAIN_WINDOW_ID, Color const & clear_color = {}) override;
		virtual DrawListID draw_list_begin(RID framebuffer, InitialAction_ initial_color_action, FinalAction_ final_color_action, InitialAction_ initial_depth_action, FinalAction_ final_depth_action, Vector<Color> const & clear_colors = {}, f32 clear_depth = 1.f, i32 clear_stencil = 0) override;
		virtual void draw_list_bind_pipeline(DrawListID draw_list, RID pipeline) override;
		virtual void draw_list_bind_uniform_set(DrawListID draw_list, RID uniform_set, size_t index) override;
		virtual void draw_list_bind_vertex_array(DrawListID draw_list, RID vertex_array) override;
		virtual void draw_list_bind_index_array(DrawListID draw_list, RID index_array) override;
		virtual void draw_list_set_line_width(DrawListID draw_list, f32 width) override;
		virtual void draw_list_set_push_constant(DrawListID draw_list, void const * data, size_t data_size) override;
		virtual void draw_list_enable_scissor(DrawListID draw_list, IntRect const & rect) override;
		virtual void draw_list_disable_scissor(DrawListID draw_list) override;
		virtual void draw_list_draw(DrawListID draw_list, bool use_indices, size_t instances, size_t procedural_vertices) override;
		virtual void draw_list_end() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_RENDERING_DEVICE_OPENGL_HPP_
#endif // OPENGL_ENABLED
