#ifndef _PN_REGISTER_LUA_TYPES_HPP_
#define _PN_REGISTER_LUA_TYPES_HPP_

#include <core/extension/extension.hpp>

extern "C"
{
	PN_MOD_API bool open_lua_library(IsmExtensionInterfacePtr iface, IsmExtensionPtr library, IsmExtensionInitializationPtr init);
	PN_MOD_API void initialize_lua_module(void * user, IsmExtensionInitializationLevel level);
	PN_MOD_API void finalize_lua_module(void * user, IsmExtensionInitializationLevel level);
}

#endif // !_PN_REGISTER_LUA_TYPES_HPP_
