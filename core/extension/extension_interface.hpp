#ifndef _PN_EXTENSION_INTERFACE_HPP_
#define _PN_EXTENSION_INTERFACE_HPP_

#include <core/typedefs.hpp>

using PnExtensionInterfacePtr = void *;
using PnExtensionPtr = void *;
using PnExtensionInitializationPtr = void *;
using PnExtensionInitializationLevel = int;

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum ExtensionInitializationLevel_
	{
		ExtensionInitializationLevel_Core,
		ExtensionInitializationLevel_Servers,
		ExtensionInitializationLevel_Scene,
		ExtensionInitializationLevel_Editor,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ExtensionInterface
	{
		u32		version_major{};
		u32		version_minor{};
		u32		version_patch{};
		u32		version_build{};
		cstring	version_string{};
		
		// etc...
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ExtensionInitialization
	{
		ExtensionInitializationLevel_ minimum_level{};
		void * user{};
		void(*initialize)(void * user, PnExtensionInitializationLevel level) {};
		void(*finalize)(void * user, PnExtensionInitializationLevel level) {};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using ExtensionInitializationFunc = bool(*)(PnExtensionInterfacePtr iface, PnExtensionPtr library, PnExtensionInitializationPtr init);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_EXTENSION_INTERFACE_HPP_
