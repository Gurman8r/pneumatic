#ifndef _PN_REGISTER_DRIVER_TYPES_HPP_
#define _PN_REGISTER_DRIVER_TYPES_HPP_

#include <core/typedefs.hpp>

namespace pn
{
	PN_API_FUNC(void) register_core_driver_types();
	PN_API_FUNC(void) unregister_core_driver_types();

	PN_API_FUNC(void) register_driver_types();
	PN_API_FUNC(void) unregister_driver_types();
}

#endif // !_PN_REGISTER_DRIVER_TYPES_HPP_
