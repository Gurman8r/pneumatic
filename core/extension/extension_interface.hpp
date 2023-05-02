#ifndef _PNU_EXTENSION_INTERFACE_HPP_
#define _PNU_EXTENSION_INTERFACE_HPP_

#include <core/typedefs.hpp>

using PneuExtensionInterfacePtr = void *;
using PneuExtensionPtr = void *;
using PneuExtensionInitializationPtr = void *;
using PneuExtensionInitializationLevel = int;

namespace Pnu
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
		void(*initialize)(void * user, PneuExtensionInitializationLevel level) {};
		void(*finalize)(void * user, PneuExtensionInitializationLevel level) {};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using ExtensionInitializationFunc = bool(*)(PneuExtensionInterfacePtr iface, PneuExtensionPtr library, PneuExtensionInitializationPtr init);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_EXTENSION_INTERFACE_HPP_
