#ifndef _PN_REGISTER_MODULE_TYPES_HPP_
#define _PN_REGISTER_MODULE_TYPES_HPP_

#include <core/extension/extension_interface.hpp>

namespace pn
{
	PN_API_FUNC(void) initialize_modules(ExtensionInitializationLevel_ level);
	PN_API_FUNC(void) finalize_modules(ExtensionInitializationLevel_ level);
}

#endif // !_PN_REGISTER_MODULE_TYPES_HPP_
