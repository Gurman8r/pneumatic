#ifndef _PNU_REGISTER_MODULE_TYPES_HPP_
#define _PNU_REGISTER_MODULE_TYPES_HPP_

#include <core/extension/extension_interface.hpp>

namespace Pnu
{
	PNU_API_FUNC(void) initialize_modules(ExtensionInitializationLevel_ level);
	PNU_API_FUNC(void) finalize_modules(ExtensionInitializationLevel_ level);
}

#endif // !_PNU_REGISTER_MODULE_TYPES_HPP_
