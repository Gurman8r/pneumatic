#ifndef _PNU_REGISTER_SCENE_TYPES_HPP_
#define _PNU_REGISTER_SCENE_TYPES_HPP_

#include <core/typedefs.hpp>

namespace Pnu
{
	PNU_API_FUNC(void) register_scene_types();
	PNU_API_FUNC(void) register_scene_singletons();
	PNU_API_FUNC(void) unregister_scene_types();
}

#endif // !_PNU_REGISTER_SCENE_TYPES_HPP_
