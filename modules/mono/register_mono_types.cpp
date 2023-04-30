#include <modules/mono/register_mono_types.hpp>
#include <modules/mono/mono_script.hpp>
#include <modules/mono/mono_behavior.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace pn;

static MonoLanguage * mono_language{};

static Ref<MonoScriptFormatLoader> cs_loader{};

bool open_mono_library(IsmExtensionInterfacePtr iface, IsmExtensionPtr library, IsmExtensionInitializationPtr initialization)
{
	if (!iface || !library || !initialization) { return false; }
	PRINT_LINE("open mono module");
	auto init{ (ExtensionInitialization *)initialization };
	init->minimum_level = ExtensionInitializationLevel_Scene;
	init->user = nullptr;
	init->initialize = &initialize_mono_module;
	init->finalize = &finalize_mono_module;
	return true;
}

void initialize_mono_module(void * user, IsmExtensionInitializationLevel level)
{
	if (level != ExtensionInitializationLevel_Scene) { return; }
	PRINT_LINE("initialize mono module");
	cs_loader.instance(); get_resource_loader()->add(cs_loader);
	REGISTER_CLASS(MonoLanguage, MonoScript, MonoInstance, MonoBehavior);
	mono_language = memnew(MonoLanguage); get_script_server()->register_language(mono_language);
}

void finalize_mono_module(void * user, IsmExtensionInitializationLevel level)
{
	if (level != ExtensionInitializationLevel_Scene) { return; }
	PRINT_LINE("finalize mono module");
	get_script_server()->unregister_language(mono_language); memdelete(mono_language);
	UNREGISTER_CLASS(MonoLanguage, MonoScript, MonoInstance, MonoBehavior);
	get_resource_loader()->remove(cs_loader); cs_loader = nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */