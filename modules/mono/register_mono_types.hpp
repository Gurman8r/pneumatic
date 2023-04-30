#ifndef _PNU_REGISTER_MONO_TYPES_HPP_
#define _PNU_REGISTER_MONO_TYPES_HPP_

#include <core/extension/extension.hpp>

extern "C"
{
	PNU_MOD_API bool open_mono_library(PnuExtensionInterfacePtr iface, PnuExtensionPtr library, PnuExtensionInitializationPtr init);
	PNU_MOD_API void initialize_mono_module(void * user, PnuExtensionInitializationLevel level);
	PNU_MOD_API void finalize_mono_module(void * user, PnuExtensionInitializationLevel level);
}

#endif // !_PNU_REGISTER_MONO_TYPES_HPP_
