#ifndef _PNU_REGISTER_MONO_TYPES_HPP_
#define _PNU_REGISTER_MONO_TYPES_HPP_

#include <core/extension/extension.hpp>

extern "C"
{
	PNU_MOD_API bool open_mono_library(PneuExtensionInterfacePtr iface, PneuExtensionPtr library, PneuExtensionInitializationPtr init);
	PNU_MOD_API void initialize_mono_module(void * user, PneuExtensionInitializationLevel level);
	PNU_MOD_API void finalize_mono_module(void * user, PneuExtensionInitializationLevel level);
}

#endif // !_PNU_REGISTER_MONO_TYPES_HPP_
