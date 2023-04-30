#ifndef _PNU_REGISTER_SERVER_TYPES_HPP_
#define _PNU_REGISTER_SERVER_TYPES_HPP_

#include <core/typedefs.hpp>

namespace Pnu
{
	PNU_API_FUNC(void) register_server_types();
	PNU_API_FUNC(void) register_server_singletons();
	PNU_API_FUNC(void) unregister_server_types();
}

#endif // !_PNU_REGISTER_SERVER_TYPES_HPP_
