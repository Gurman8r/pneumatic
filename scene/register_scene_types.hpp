#ifndef _PN_REGISTER_SCENE_TYPES_HPP_
#define _PN_REGISTER_SCENE_TYPES_HPP_

#include <core/typedefs.hpp>

namespace pn
{
	PN_API_FUNC(void) register_scene_types();
	PN_API_FUNC(void) register_scene_singletons();
	PN_API_FUNC(void) unregister_scene_types();
}

#endif // !_PN_REGISTER_SCENE_TYPES_HPP_
