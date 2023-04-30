#ifndef _PN_REGISTER_MONO_TYPES_HPP_
#define _PN_REGISTER_MONO_TYPES_HPP_

#include <core/extension/extension.hpp>

extern "C"
{
	PN_MOD_API bool open_mono_library(IsmExtensionInterfacePtr iface, IsmExtensionPtr library, IsmExtensionInitializationPtr init);
	PN_MOD_API void initialize_mono_module(void * user, IsmExtensionInitializationLevel level);
	PN_MOD_API void finalize_mono_module(void * user, IsmExtensionInitializationLevel level);
}

#endif // !_PN_REGISTER_MONO_TYPES_HPP_
