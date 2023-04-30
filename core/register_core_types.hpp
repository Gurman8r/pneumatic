#ifndef _PNU_REGISTER_CORE_TYPES_HPP_
#define _PNU_REGISTER_CORE_TYPES_HPP_

#include <core/extension/extension_interface.hpp>

namespace Pnu
{
	PNU_API_FUNC(void) register_core_types();
	PNU_API_FUNC(void) register_core_settings();
	PNU_API_FUNC(void) register_core_extensions();
	PNU_API_FUNC(void) register_core_singletons();
	PNU_API_FUNC(void) unregister_core_extensions();
	PNU_API_FUNC(void) unregister_core_types();
}

#endif // !_PNU_REGISTER_CORE_TYPES_HPP_
