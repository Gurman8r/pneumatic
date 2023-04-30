#ifndef _PN_REGISTER_CORE_TYPES_HPP_
#define _PN_REGISTER_CORE_TYPES_HPP_

#include <core/extension/extension_interface.hpp>

namespace pn
{
	PN_API_FUNC(void) register_core_types();
	PN_API_FUNC(void) register_core_settings();
	PN_API_FUNC(void) register_core_extensions();
	PN_API_FUNC(void) register_core_singletons();
	PN_API_FUNC(void) unregister_core_extensions();
	PN_API_FUNC(void) unregister_core_types();
}

#endif // !_PN_REGISTER_CORE_TYPES_HPP_
