#include <main/main.hpp>

#include <core/register_core_types.hpp>
#include <drivers/register_driver_types.hpp>
#include <platform/register_platform_apis.hpp>
#include <modules/register_module_types.hpp>
#include <scene/register_scene_types.hpp>
#include <servers/register_server_types.hpp>

#include <core/io/resource_loader.hpp>
#include <core/io/resource_saver.hpp>
#include <core/io/file_access_zip.hpp>

#include <core/config/engine.hpp>
#include <core/config/project_settings.hpp>
#include <core/extension/extension_manager.hpp>
#include <core/object/script.hpp>

#include <servers/audio_server.hpp>
#include <servers/physics_server.hpp>
#include <servers/rendering_server.hpp>
#include <servers/text_server.hpp>
#include <scene/main/scene_tree.hpp>

#if TOOLS_ENABLED
#include <editor/editor_node.hpp>
#include <editor/register_editor_types.hpp>
#endif

#include <scene/gui/imgui.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	i32 Main::m_iterating{};
	u64 Main::m_frame{};

	static Engine *				engine{};
	static Performance *		performance{};
	static Input *				input{};
	static ProjectSettings *	settings{};

	static PackedData *			packages{};
	static ZipArchive *			zip_archive{};

	static AudioServer *		audio{};
	static DisplayServer *		display{};
	static RenderingServer *	graphics{};
	static PhysicsServer *		physics{};
	static TextServer *			text{};

	static bool editor{ true };

	static ImGuiContext * imgui_context{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ Main::setup(cstring exepath, i32 argc, char * argv[])
	{
		Error_ error{ Error_OK };
		Vector<String> args{ argv, argv + argc };

		get_os()->initialize();
		engine = memnew(Engine);
		register_core_types();
		register_core_driver_types();

		performance = memnew(Performance);
		input = memnew(Input);
		settings = memnew(ProjectSettings);
		register_core_settings();

		if (!(packages = get_packed_data())) { packages = memnew(PackedData); }
		if (!(zip_archive = get_zip_archive())) { zip_archive = memnew(ZipArchive); }
		packages->add_pack_source(zip_archive);

		settings->setup(exepath);

		initialize_modules(ExtensionInitializationLevel_Core);
		register_core_extensions();

		get_os()->set_cmdline(exepath, args);

		display = DS::create("Pneumatic", DS::WindowMode_Maximized, { 0, 0 }, { 1280, 720 }, 0, error);
		graphics = RS::create();
		text = memnew(TextServer);
		physics = memnew(PhysicsServer);
		audio = memnew(AudioServer);

		register_core_singletons();

		register_server_types();
		initialize_modules(ExtensionInitializationLevel_Servers);
		get_extension_manager()->initialize_extensions(ExtensionInitializationLevel_Servers);
		register_server_singletons();

		register_scene_types();
		register_driver_types();
		initialize_modules(ExtensionInitializationLevel_Scene);
		get_extension_manager()->initialize_extensions(ExtensionInitializationLevel_Scene);
		register_scene_singletons();

#if TOOLS_ENABLED
		register_editor_types();
		initialize_modules(ExtensionInitializationLevel_Editor);
		get_extension_manager()->initialize_extensions(ExtensionInitializationLevel_Editor);
		register_editor_singletons();
#endif

		register_platform_apis();

		//initialize_theme();

		//initialize_physics();
	
		get_script_server()->initialize_languages();
	
		imgui_context = VALIDATE(ImGui_Initialize());

		return error;
	}

	bool Main::start()
	{
		String script{};

		Ref<MainLoop> main_loop{};

		if (editor) { main_loop = memnew(SceneTree); }

		TYPE main_loop_type{};

		if (script) {
			/* TODO: load main loop from script */
		}

		if (!main_loop && !main_loop_type) {
			main_loop_type = typeof<SceneTree>();
		}

		if (!main_loop) {
			ASSERT(TYPE::check_(main_loop_type));
			main_loop = main_loop_type();
			ASSERT(main_loop);
		}

		if (isinstance<SceneTree>(main_loop))
		{
			SceneTree * tree{ (SceneTree *)*main_loop };

			Window * root{ tree->get_root() };

#if TOOLS_ENABLED
			if (editor) { root->add_child<EditorNode>(); }
#endif

			// etc...
		}
	
		get_os()->set_main_loop(main_loop);

		return true;
	}

	bool Main::iteration()
	{
		++m_iterating; ON_SCOPE_EXIT(&) { --m_iterating; };

		Clock const loop_timer{};
		static Duration delta_time{ 16_ms };
		ON_SCOPE_EXIT(&) { delta_time = loop_timer.get_elapsed_time(); };

		++m_frame;

		bool should_close{ false };

		// TODO: physics stuff goes here

		get_input()->iteration(delta_time);

		ImGui_BeginFrame(imgui_context);

		if (get_os()->get_main_loop()->process(delta_time)) { should_close = true; }

		ImGui::Render();
		RENDERING_DEVICE->draw_list_begin_for_screen();
		ImGui_RenderDrawData(&imgui_context->Viewports[0]->DrawDataP);
		RENDERING_DEVICE->draw_list_end();

		ImGui_EndFrame(imgui_context);
	
		return should_close;
	}

	void Main::cleanup()
	{
		//remove_custom_loaders();
		//remove_custom_savers();

		ImGui_Finalize(imgui_context);

		get_os()->delete_main_loop();

		get_script_server()->finalize_languages();

#if TOOLS_ENABLED
		finalize_modules(ExtensionInitializationLevel_Editor);
		get_extension_manager()->finalize_extensions(ExtensionInitializationLevel_Editor);
		unregister_editor_types();
#endif

		finalize_modules(ExtensionInitializationLevel_Scene);
		get_extension_manager()->finalize_extensions(ExtensionInitializationLevel_Scene);
		unregister_platform_apis();
		unregister_driver_types();
		unregister_scene_types();

		//finalize_theme();

		finalize_modules(ExtensionInitializationLevel_Servers);
		get_extension_manager()->finalize_extensions(ExtensionInitializationLevel_Servers);
		unregister_server_types();

		memdelete(audio);
		get_os()->finalize();
		graphics->finalize();
		memdelete(graphics);
		memdelete(display);
		memdelete(physics);
		memdelete(text);

		memdelete(packages);
		memdelete(input);
		memdelete(settings);
		memdelete(performance);

		unregister_core_driver_types();
		unregister_core_extensions();
		unregister_core_types();
		memdelete(engine);
		get_os()->finalize_core();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}