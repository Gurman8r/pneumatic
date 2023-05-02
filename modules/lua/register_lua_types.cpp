#include <modules/lua/register_lua_types.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace Pnu;

bool open_lua_library(PneuExtensionInterfacePtr iface, PneuExtensionPtr library, PneuExtensionInitializationPtr initialization)
{
	if (!iface || !library || !initialization) { return false; }
	PRINT_LINE("open lua module");
	auto init{ (ExtensionInitialization *)initialization };
	init->minimum_level = ExtensionInitializationLevel_Scene;
	init->user = nullptr;
	init->initialize = &initialize_lua_module;
	init->finalize = &finalize_lua_module;
	return true;
}

void initialize_lua_module(void * user, PneuExtensionInitializationLevel level)
{
	if (level != ExtensionInitializationLevel_Scene) { return; }
	PRINT_LINE("initialize lua module");
}

void finalize_lua_module(void * user, PneuExtensionInitializationLevel level)
{
	if (level != ExtensionInitializationLevel_Scene) { return; }
	PRINT_LINE("finalize lua module");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */