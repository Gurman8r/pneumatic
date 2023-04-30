#ifndef _PNU_ENGINE_HPP_
#define _PNU_ENGINE_HPP_

#include <core/os/main_loop.hpp>

namespace Pnu
{
	class Engine final
	{
		static Engine * __singleton;

	public:
		Engine() noexcept;
		~Engine() noexcept;
		FORCE_INLINE static Engine * get_singleton() noexcept { return __singleton; }

	public:

	};

	SINGLETON_WRAPPER(Engine, get_engine);
}

#endif // !_PNU_ENGINE_HPP_
