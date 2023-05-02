#include <modules/mono/register_mono_types.hpp>
#include <modules/mono/csharp.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace Pnu;

static CSharpLanguage * cs_language{};
static Ref<CSharpScriptFormatLoader> cs_loader{};

bool open_mono_library(PneuExtensionInterfacePtr iface, PneuExtensionPtr library, PneuExtensionInitializationPtr initialization)
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

void initialize_mono_module(void * user, PneuExtensionInitializationLevel level)
{
	if (level != ExtensionInitializationLevel_Scene) { return; }
	PRINT_LINE("initialize mono module");
	cs_loader.instance(); get_resource_loader()->add(cs_loader);
	REGISTER_CLASS(CSharpLanguage, CSharpScript, CSharpInstance);
	cs_language = memnew(CSharpLanguage); get_script_server()->register_language(cs_language);
}

void finalize_mono_module(void * user, PneuExtensionInitializationLevel level)
{
	if (level != ExtensionInitializationLevel_Scene) { return; }
	PRINT_LINE("finalize mono module");
	get_script_server()->unregister_language(cs_language); memdelete(cs_language);
	UNREGISTER_CLASS(CSharpLanguage, CSharpScript, CSharpInstance);
	get_resource_loader()->remove(cs_loader); cs_loader = nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */