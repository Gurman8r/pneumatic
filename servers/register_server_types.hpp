#ifndef _PN_REGISTER_SERVER_TYPES_HPP_
#define _PN_REGISTER_SERVER_TYPES_HPP_

#include <core/typedefs.hpp>

namespace pn
{
	PN_API_FUNC(void) register_server_types();
	PN_API_FUNC(void) register_server_singletons();
	PN_API_FUNC(void) unregister_server_types();
}

#endif // !_PN_REGISTER_SERVER_TYPES_HPP_
