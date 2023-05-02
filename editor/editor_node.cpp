#include <editor/editor_node.hpp>
#include <editor/editor_filesystem.hpp>
#include <editor/editor_hierarchy.hpp>
#include <editor/editor_log.hpp>
#include <editor/editor_viewport.hpp>

#include <scene/gui/imgui.hpp>
#include <core/math/face3.hpp>
#include <core/math/transform.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum {
		SCENE_STATE_UNIFORMS,
		RENDER_PASS_UNIFORMS,
		TRANSFORMS_UNIFORMS,
		MATERIAL_UNIFORMS,
		MAX_UNIFORMS
	};

	static RID uniform_buffers[MAX_UNIFORMS]{};
	static RID uniform_sets[MAX_UNIFORMS]{};
	static RID material{};
	static RID pipeline{};
	static RID framebuffer{}, backbuffer{};
	static RID render_target{};
	static RID viewport{};

	static Vec2 screen_resolution{};

	static struct Camera
	{
		f32 fov{ radians(45) }, znear{ 0.001f }, zfar{ 1000.f };

		f32 pitch{}, yaw{}, roll{};

		Mat4 projection{ identity_v<Mat4> };

		Transform transform{ Vec3{ 0, 0, -5 } };
	}
	camera;

	static Transform transform[] =
	{
		{ Vec3{ 0, 0, 0 } },
	};

	static void _setup_pipeline(RID const shader)
	{
		pipeline = RENDERING_DEVICE->render_pipeline_create
		(
			shader,
			RD::RenderPrimitive_Triangles,
			MAKE(RD::RasterizationState, r)
			{
				r.enable_depth_clamp = false;
				r.discard_primitives = false;
				r.enable_wireframe = false;
				r.cull_mode = RD::PolygonCullMode_Front;
				r.front_face = RD::PolygonFrontFace_Clockwise;
				r.enable_depth_bias = false;
				r.depth_bias_constant_factor = 0.f;
				r.depth_bias_clamp = 0.f;
				r.depth_bias_slope_factor = 0.f;
				r.line_width = 1.f;
				r.patch_control_points = 1;
			},
			MAKE(RD::MultisampleState, m)
			{
				m.sample_count = RD::TextureSamples_1;
				m.enable_sample_shading = false;
				m.sample_mask = {};
				m.enable_alpha_to_coverage = false;
				m.enable_alpha_to_one = false;
			},
			MAKE(RD::DepthStencilState, d)
			{
				d.enable_depth_test = true;
				d.enable_depth_write = true;
				d.depth_compare_operator = CompareOperator_Less;
				d.enable_depth_range = false;
				d.depth_range_min = 0.f;
				d.depth_range_max = 0.f;
				d.enable_stencil = false;
				d.front_op.fail = d.back_op.fail = RD::StencilOperation_Zero;
				d.front_op.pass = d.back_op.pass = RD::StencilOperation_Zero;
				d.front_op.depth_fail = d.back_op.depth_fail = RD::StencilOperation_Zero;
				d.front_op.compare = d.back_op.compare = CompareOperator_Always;
				d.front_op.compare_mask = d.back_op.compare_mask = 0;
				d.front_op.write_mask = d.back_op.write_mask = 0;
				d.front_op.reference = d.back_op.reference = 0;
			},
			MAKE(RD::ColorBlendState, c)
			{
				c.enable_logic_op = true;
				c.logic_op = LogicOperation_Clear;
				c.blend_constant = Colors::clear;
				c.attachments.push_back({ true,
					RD::BlendFactor_SrcAlpha, RD::BlendFactor_OneMinusSrcAlpha, RD::BlendOperation_Add,
					RD::BlendFactor_SrcAlpha, RD::BlendFactor_OneMinusSrcAlpha, RD::BlendOperation_Add,
					true, true, true, true });
			}
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorNode * EditorNode::__singleton{};

	EMBED_CLASS(EditorNode, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorNode::EditorNode()
	{
		ASSERT(!__singleton); __singleton = this;

		m_filesystem = memnew(EditorFileSystem);
		m_hierarchy = memnew(EditorHierarchy);
		m_log = memnew(EditorLog);
		m_viewport = memnew(EditorViewport);

		RID c{ get_rendering_server()->camera_create() };
		get_rendering_server()->camera_set_perspective(c, radians(45), 0.001f, 1000.f);
		get_rendering_server()->camera_set_transform(c, camera.transform);
		get_rendering_server()->camera_destroy(c);

		m_shaders["2d"].instance("../assets/shaders/2d.shader");
		m_shaders["3d"].instance("../assets/shaders/3d.shader");
		m_textures["earth_dm_2k"].instance<ImageTexture>("../assets/textures/earth/earth_dm_2k.png");
		m_textures["earth_sm_2k"].instance<ImageTexture>("../assets/textures/earth/earth_sm_2k.png");
		m_meshes["sphere32x24"].instance("../assets/meshes/sphere32x24.obj");

		//RS::SurfaceData quad_spec{};
		//quad_spec.primitive = RS::Primitive_Triangles;
		//quad_spec.vertex_data
		//	<< Vec4{ -1, -1 } << Vec4{} << Vec4{ 0, 0 } << Vec4{} << Vec4{}
		//	<< Vec4{ 1, -1 } << Vec4{} << Vec4{ 1, 0 } << Vec4{} << Vec4{}
		//	<< Vec4{ 1, 1 } << Vec4{} << Vec4{ 1, 1 } << Vec4{} << Vec4{};
		//quad_spec.vertex_data
		//	<< Vec4{ 1, 1 } << Vec4{} << Vec4{ 1, 1 } << Vec4{} << Vec4{}
		//	<< Vec4{ 1, -1 } << Vec4{} << Vec4{ 1, 0 } << Vec4{} << Vec4{}
		//	<< Vec4{ -1, -1 } << Vec4{} << Vec4{ 0, 0 } << Vec4{} << Vec4{}
		//	<< Vec4{ -1, 1 } << Vec4{} << Vec4{ 0, 1 } << Vec4{} << Vec4{};
		//quad_spec.vertex_data
		//	<< Vec4{ -1, 1 } << Vec4{} << Vec4{ 0, 1 } << Vec4{} << Vec4{}
		//	<< Vec4{ -1, -1 } << Vec4{} << Vec4{ 0, 0 } << Vec4{} << Vec4{}
		//	<< Vec4{ 1, -1 } << Vec4{} << Vec4{ 1, 0 } << Vec4{} << Vec4{}
		//	<< Vec4{ -1, 1 } << Vec4{} << Vec4{ 0, 1 } << Vec4{} << Vec4{}
		//	<< Vec4{ 1, -1 } << Vec4{} << Vec4{ 1, 0 } << Vec4{} << Vec4{}
		//	<< Vec4{ 1, 1 } << Vec4{} << Vec4{ 1, 1 } << Vec4{} << Vec4{};
		//quad_spec.vertex_count = quad_spec.vertex_data.size() / sizeof(f32);
		//quad_spec.index_data << 0u << 1u << 2u;
		//quad_spec.index_count = quad_spec.index_data.size() / sizeof(u32);
		//m_meshes["quad"].instance(quad_spec);

		RID const shader{ m_shaders["3d"]->get_rid() };
		_setup_pipeline(shader);

		uniform_buffers[SCENE_STATE_UNIFORMS] = RENDERING_DEVICE->uniform_buffer_create(sizeof(RD::Std140<Mat4, Mat4>));
		uniform_sets[SCENE_STATE_UNIFORMS] = RENDERING_DEVICE->uniform_set_create({
			{ RD::UniformType_UniformBuffer, SCENE_STATE_UNIFORMS, { uniform_buffers[SCENE_STATE_UNIFORMS] } },
		}, shader);

		uniform_buffers[RENDER_PASS_UNIFORMS] = RENDERING_DEVICE->uniform_buffer_create(sizeof(RD::Std140<Mat4>));
		uniform_sets[RENDER_PASS_UNIFORMS] = RENDERING_DEVICE->uniform_set_create({
			{ RD::UniformType_UniformBuffer, RENDER_PASS_UNIFORMS, { uniform_buffers[RENDER_PASS_UNIFORMS] } },
		}, shader);

		uniform_buffers[TRANSFORMS_UNIFORMS] = RENDERING_DEVICE->uniform_buffer_create(sizeof(RD::Std140<Mat4>));
		uniform_sets[TRANSFORMS_UNIFORMS] = RENDERING_DEVICE->uniform_set_create({
			{ RD::UniformType_UniformBuffer, TRANSFORMS_UNIFORMS, { uniform_buffers[TRANSFORMS_UNIFORMS] } },
		}, shader);

		uniform_buffers[MATERIAL_UNIFORMS] = RENDERING_DEVICE->uniform_buffer_create(sizeof(RD::Std140<Vec4, Vec4, Vec4, f32>));
		uniform_sets[MATERIAL_UNIFORMS] = RENDERING_DEVICE->uniform_set_create({
			{ RD::UniformType_UniformBuffer, MATERIAL_UNIFORMS, { uniform_buffers[MATERIAL_UNIFORMS] } },
			{ RD::UniformType_Texture, 0, { m_textures["earth_dm_2k"]->get_rid() } },
			{ RD::UniformType_Texture, 1, { m_textures["earth_sm_2k"]->get_rid() } },
		}, shader);
	
		// material (WIP)
		material = get_rendering_server()->material_create();
		get_rendering_server()->material_set_shader(material, shader);
		get_rendering_server()->material_update(material, {
			{ "Ambient", Vec4{ 0.8f, 0.4f, 0.2f, 1.0f } },
			{ "Diffuse", Vec4{ 0.5f, 0.5f, 0.5f, 1.0f } },
			{ "Specular", Vec4{ 1.0f, 1.0f, 1.0f, 1.0f } },
			{ "Shininess", 32.f },
			{ "Diffuse_Map", m_textures["earth_dm_2k"]->get_rid() },
			{ "Specular_Map", m_textures["earth_sm_2k"]->get_rid() },
		});
	
		// framebuffers
		Vector<RID> fb_textures{
			RENDERING_DEVICE->texture_create(MAKE(RD::TextureCreateInfo, t) {
				t.color_format = RD::DataFormat_R8G8B8_UNORM;
				t.usage_flags = RD::TextureFlags_Sampling | RD::TextureFlags_CanCopyFrom | RD::TextureFlags_ColorAttachment;
			}),
			RENDERING_DEVICE->texture_create(MAKE(RD::TextureCreateInfo, t) {
				t.color_format = RD::DataFormat_D24_UNORM_S8_UINT;
				t.usage_flags = RD::TextureFlags_Sampling | RD::TextureFlags_CanCopyFrom | RD::TextureFlags_DepthStencilAttachment;
			}),
		};
		framebuffer = RENDERING_DEVICE->framebuffer_create(fb_textures);
		m_viewport->m_main_texture = fb_textures[0];
	}

	EditorNode::~EditorNode()
	{
		ASSERT(this == __singleton); ON_SCOPE_EXIT(&) { __singleton = nullptr; };

		if (material) { get_rendering_server()->material_destroy(material); }

		for (size_t i{}; i < ARRAY_SIZE(uniform_buffers); ++i) {
			if (uniform_buffers[i]) { RENDERING_DEVICE->buffer_destroy(uniform_buffers[i]); }
		}

		for (size_t i{}; i < ARRAY_SIZE(uniform_sets); ++i) {
			if (uniform_sets[i]) { RENDERING_DEVICE->uniform_set_destroy(uniform_sets[i]); }
		}

		if (framebuffer) { RENDERING_DEVICE->framebuffer_destroy(framebuffer); }
		if (backbuffer) { RENDERING_DEVICE->framebuffer_destroy(backbuffer); }
		if (pipeline) { RENDERING_DEVICE->render_pipeline_destroy(pipeline); }

		memdelete(m_filesystem);
		memdelete(m_hierarchy);
		memdelete(m_log);
		memdelete(m_viewport);
	}

	void EditorNode::_notification(Notification_ id)
	{
		switch (id)
		{
		case Notification_Process: {

			get_tree()->get_root()->set_title(String::format<48>("Pnu @ %.3f fps", get_tree()->get_fps().value));

			Duration const delta_time{ get_tree()->get_delta_time() };

			f32 const camera_move_speed{ 10.f * delta_time };
			if (get_input()->is_key_down(Input::Key_Q)) { camera.transform.translate(-up_v<Vec3> * camera_move_speed); }
			else if (get_input()->is_key_down(Input::Key_Z)) { camera.transform.translate(up_v<Vec3> * camera_move_speed); }
			if (get_input()->is_key_down(Input::Key_W)) { camera.transform.translate(camera.transform.front() * camera_move_speed); }
			else if (get_input()->is_key_down(Input::Key_S)) { camera.transform.translate(-camera.transform.front() * camera_move_speed); }
			if (get_input()->is_key_down(Input::Key_D)) { camera.transform.translate(camera.transform.right() * camera_move_speed); }
			else if (get_input()->is_key_down(Input::Key_A)) { camera.transform.translate(-camera.transform.right() * camera_move_speed); }

			if (m_viewport->get_window()) {
				if (Vec2 const res{ m_viewport->get_window()->InnerRect.GetWidth(), m_viewport->get_window()->InnerRect.GetHeight() }
				; screen_resolution != res) {
					screen_resolution = res;
					camera.projection = perspective(camera.fov, screen_resolution[0] / screen_resolution[1], camera.znear, camera.zfar);
					RENDERING_DEVICE->framebuffer_set_size(framebuffer, (i32)screen_resolution[0], (i32)screen_resolution[1]);
				}
			}

			if (m_viewport->m_is_dragging_view) {
				Vec2 const drag{ get_input()->get_mouse_delta() * (f32)delta_time };
				camera.yaw += drag[0];
				camera.pitch = constrain(camera.pitch + drag[1], -89.f, 89.f);
			}
			camera.transform.set_rotation(camera.pitch, camera.yaw, camera.roll);

			m_viewport->m_camera_proj = camera.projection;
			m_viewport->m_camera_view = camera.transform;

			{
				RD::Std140<Mat4, Mat4> scene_state_ubo;
				scene_state_ubo.set<0>(camera.projection); // camera projection
				scene_state_ubo.set<1>(camera.transform); // camera view
				RENDERING_DEVICE->buffer_update(uniform_buffers[SCENE_STATE_UNIFORMS], 0, scene_state_ubo, sizeof(scene_state_ubo));

				RD::Std140<Mat4> render_pass_ubo;
				render_pass_ubo.set<0>(identity_v<Mat4>); // placeholder
				RENDERING_DEVICE->buffer_update(uniform_buffers[RENDER_PASS_UNIFORMS], 0, render_pass_ubo, sizeof(render_pass_ubo));

				RD::Std140<Mat4> transforms_ubo;
				transforms_ubo.set<0>(transform[0]); // model transform
				RENDERING_DEVICE->buffer_update(uniform_buffers[TRANSFORMS_UNIFORMS], 0, transforms_ubo, sizeof(transforms_ubo));

				RD::Std140<Vec4, Vec4, Vec4, f32> material_ubo;
				material_ubo.set<0>({ 0.8f, 0.4f, 0.2f, 1.0f }); // ambient
				material_ubo.set<1>({ 0.5f, 0.5f, 0.5f, 1.0f }); // diffuse
				material_ubo.set<2>({ 1.0f, 1.0f, 1.0f, 1.0f }); // specular
				material_ubo.set<3>(32.f); // shininess
				RENDERING_DEVICE->buffer_update(uniform_buffers[MATERIAL_UNIFORMS], 0, material_ubo, sizeof(material_ubo));

				static Vector<Color> clear_colors{ Colors::magenta };
				clear_colors[0] = rotate_hue(clear_colors[0], 10.f * delta_time);

				RD::DrawListID const draw_list{ RENDERING_DEVICE->draw_list_begin(framebuffer, RD::InitialAction_Clear, RD::FinalAction_Read, RD::InitialAction_Keep, RD::FinalAction_Discard, clear_colors) };
				RENDERING_DEVICE->draw_list_bind_pipeline(draw_list, pipeline);
				RENDERING_DEVICE->draw_list_bind_uniform_set(draw_list, uniform_sets[SCENE_STATE_UNIFORMS], SCENE_STATE_UNIFORMS);
				RENDERING_DEVICE->draw_list_bind_uniform_set(draw_list, uniform_sets[RENDER_PASS_UNIFORMS], RENDER_PASS_UNIFORMS);
				RENDERING_DEVICE->draw_list_bind_uniform_set(draw_list, uniform_sets[TRANSFORMS_UNIFORMS], TRANSFORMS_UNIFORMS);
				RENDERING_DEVICE->draw_list_bind_uniform_set(draw_list, uniform_sets[MATERIAL_UNIFORMS], MATERIAL_UNIFORMS);
				static Mesh * mesh{ *m_meshes["sphere32x24"] };
				for (size_t i{}; i < mesh->get_surface_count(); ++i) {
					RENDERING_DEVICE->draw_list_bind_vertex_array(draw_list, mesh->surface_get_vertex_array(i));
					RID const ia{ mesh->surface_get_index_array(i) };
					RENDERING_DEVICE->draw_list_bind_index_array(draw_list, ia);
					RENDERING_DEVICE->draw_list_draw(draw_list, ia != nullptr, 1, 0);
				}
				RENDERING_DEVICE->draw_list_end();
			}

			// imgui
			_draw_dockspace();
			//m_filesystem->process(delta_time);
			m_hierarchy->process(delta_time);
			m_log->process(delta_time);
			m_viewport->process(delta_time);
			if (m_show_imgui_demo) { ImGui::ShowDemoWindow(&m_show_imgui_demo); }

		} break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void EditorNode::_draw_dockspace()
	{
		static bool opt_fullscreen{ true };
		static bool opt_padding{ false };
		static ImGuiDockNodeFlags dockspace_flags{ ImGuiDockNodeFlags_None };
	
		ImGuiWindowFlags window_flags{ ImGuiWindowFlags_NoDocking };
		if (m_show_main_menu_bar) { window_flags |= ImGuiWindowFlags_MenuBar; }
	
		if (opt_fullscreen) {
			ImGuiViewport * viewport{ ImGui::GetMainViewport() };
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVarType_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVarType_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else {
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) { window_flags |= ImGuiWindowFlags_NoBackground; }
	
		if (!opt_padding) { ImGui::PushStyleVar(ImGuiStyleVarType_WindowPadding, { 0.f, 0.f }); }
	
		bool const dockspace_open{ ImGui::Begin("Editor", nullptr, window_flags) };
	
		if (!opt_padding) { ImGui::PopStyleVar(); }
	
		if (opt_fullscreen) { ImGui::PopStyleVar(2); }
	
		ImGuiID const dockspace_id{ ImGui::GetID("##EditorDockSpace") };

		if (dockspace_open && !ImGui::DockBuilderGetNode(dockspace_id)) {
			ImGui::DockBuilderRemoveNode(dockspace_id);
			ImGui::DockBuilderAddNode(dockspace_id);
			_build_dockspace();
			ImGui::DockBuilderFinish(dockspace_id);
		}
	
		ImGui::DockSpace(dockspace_id, { 0.f, 0.f }, dockspace_flags);

		if (m_show_main_menu_bar && ImGui::BeginMenuBar()) {
			_draw_menu_bar();
			ImGui::EndMenuBar();
		}

		ImGui::End();
	}

	void EditorNode::_build_dockspace()
	{
		ImGuiID const dockspace_id{ ImGui::GetID("##EditorDockSpace") };
		ImGuiID right{ dockspace_id };
		ImGuiID left_up{ ImGui::DockBuilderSplitNode(right, ImGuiDir_Left, 0.2f, nullptr, &right) };
		ImGuiID left_down{ ImGui::DockBuilderSplitNode(left_up, ImGuiDir_Down, 0.5f, nullptr, &left_up) };
		ImGui::DockBuilderDockWindow(m_viewport->get_name(), right);
		ImGui::DockBuilderDockWindow(m_hierarchy->get_name(), left_up);
		ImGui::DockBuilderDockWindow(m_log->get_name(), left_down);
	}

	void EditorNode::_draw_menu_bar()
	{
		if (ImGui::BeginMenu("File")) {
			//ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Alt+F4")) { get_tree()->quit(); }
			ImGui::EndMenu();
		}
	
		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Dear ImGui Demo")) { m_show_imgui_demo = !m_show_imgui_demo; }
			if (ImGui::MenuItem(m_filesystem->get_name(), "", m_filesystem->is_open())) { m_filesystem->toggle_open(); }
			if (ImGui::MenuItem(m_hierarchy->get_name(), "", m_hierarchy->is_open())) { m_hierarchy->toggle_open(); }
			if (ImGui::MenuItem(m_log->get_name(), "", m_log->is_open())) { m_log->toggle_open(); }
			if (ImGui::MenuItem(m_viewport->get_name(), "", m_viewport->is_open())) { m_viewport->toggle_open(); }
			ImGui::EndMenu();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}