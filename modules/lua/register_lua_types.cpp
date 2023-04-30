#include <modules/lua/register_lua_types.hpp>
#include <modules/lua/lua_script.hpp>
#include <modules/lua/lua_behavior.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace pnu;

static LuaLanguage * lua_language{};

bool open_lua_library(IsmExtensionInterfacePtr iface, IsmExtensionPtr library, IsmExtensionInitializationPtr initialization)
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

void initialize_lua_module(void * user, IsmExtensionInitializationLevel level)
{
	if (level != ExtensionInitializationLevel_Scene) { return; }
	PRINT_LINE("initialize lua module");
	REGISTER_CLASS(LuaLanguage, LuaScript, LuaInstance, LuaBehavior);
	lua_language = memnew(LuaLanguage);
	get_script_server()->register_language(lua_language);
}

void finalize_lua_module(void * user, IsmExtensionInitializationLevel level)
{
	if (level != ExtensionInitializationLevel_Scene) { return; }
	PRINT_LINE("finalize lua module");
	get_script_server()->unregister_language(lua_language);
	memdelete(lua_language);
	UNREGISTER_CLASS(LuaLanguage, LuaScript, LuaInstance, LuaBehavior);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */