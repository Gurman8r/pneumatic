#ifndef _PNU_REGISTER_DRIVER_TYPES_HPP_
#define _PNU_REGISTER_DRIVER_TYPES_HPP_

#include <core/typedefs.hpp>

namespace Pnu
{
	PNU_API_FUNC(void) register_core_driver_types();
	PNU_API_FUNC(void) unregister_core_driver_types();

	PNU_API_FUNC(void) register_driver_types();
	PNU_API_FUNC(void) unregister_driver_types();
}

#endif // !_PNU_REGISTER_DRIVER_TYPES_HPP_
