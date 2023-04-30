#ifndef _PNU_REGISTER_LUA_TYPES_HPP_
#define _PNU_REGISTER_LUA_TYPES_HPP_

#include <core/extension/extension.hpp>

extern "C"
{
	PNU_MOD_API bool open_lua_library(PnuExtensionInterfacePtr iface, PnuExtensionPtr library, PnuExtensionInitializationPtr init);
	PNU_MOD_API void initialize_lua_module(void * user, PnuExtensionInitializationLevel level);
	PNU_MOD_API void finalize_lua_module(void * user, PnuExtensionInitializationLevel level);
}

#endif // !_PNU_REGISTER_LUA_TYPES_HPP_
