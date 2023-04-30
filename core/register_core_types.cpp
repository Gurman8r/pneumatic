#include <core/register_core_types.hpp>
#include <core/io/resource_loader.hpp>
#include <core/io/resource_saver.hpp>
#include <core/io/image_library.hpp>

#include <core/config/engine.hpp>
#include <core/config/project_settings.hpp>
#include <core/extension/extension_manager.hpp>
#include <core/input/input.hpp>
#include <core/object/script.hpp>
#include <core/os/main_loop.hpp>
#include <core/io/config_file.hpp>
#include <core/io/dir_access.hpp>
#include <core/io/event_system.hpp>
#include <core/io/file_access_zip.hpp>
#include <core/io/lexer.hpp>

namespace pn
{
	static Internals *			internals{};
	static ResourceLoader *		loader{};
	static ResourceSaver *		saver{};

	static ExtensionManager *	extensions{};
	static ScriptServer *		scripts{};

	static Ref<ImageFormatLoader> image_format_loader{};

	void register_core_types()
	{
		internals = memnew(Internals);
		loader = memnew(ResourceLoader);
		saver = memnew(ResourceSaver);
		extensions = memnew(ExtensionManager);
		scripts = memnew(ScriptServer);

		image_format_loader.instance(); get_resource_loader()->add(image_format_loader);

		REGISTER_CLASS
		(
			CppFunctionObject, // <- cppfunction must go first
			Object,
			TypeObject,
			IntObject,
			FloatObject,
			IteratorObject,
			StringObject,
			TupleObject,
			ListObject,
			DictObject,
			CapsuleObject,
			FunctionObject,
			MethodObject,
			PropertyObject,
			ModuleObject,
			GenericObject,

			Resource,

			ScriptServer, Script, ScriptInstance, ScriptLanguage, PlaceholderScriptInstance,

			Extension, ExtensionManager,

			DirAccess, FileAccess, FileAccessPack, FileAccessZip,

			Event, EventListener, DummyListener, EventDelegate<Event>, EventBus,

			Input, InputEvent, InputMap,
			
			ConfigFile,
			
			Image, ImageFormatLoader,

			Lexer,

			ProjectSettings,
			
			MainLoop
		);

	}

	void register_core_settings()
	{
	}
	
	void register_core_extensions()
	{
		Extension::initialize_interface();
		extensions->load_extensions();
		extensions->initialize_extensions(ExtensionInitializationLevel_Core);
	}
	
	void register_core_singletons()
	{
	}

	void unregister_core_extensions()
	{
		extensions->finalize_extensions(ExtensionInitializationLevel_Core);
		memdelete(scripts);
		memdelete(extensions);
	}

	void unregister_core_types()
	{
		get_resource_loader()->remove(image_format_loader); image_format_loader = nullptr;

		memdelete(loader);
		memdelete(saver);
		memdelete(internals);
	}
}