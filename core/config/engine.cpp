#include <core/config/engine.hpp>
#include <corecrt_wstdio.h>

namespace Pnu
{
	Engine * Engine::__singleton{};

	Engine::Engine() noexcept
	{
		SINGLETON_CTOR();



		auto _formatter = []() {
			if constexpr (true) {
				return std::vsnprintf;
			}
			else {
				return std::vswprintf;
			}
		};
	}

	Engine::~Engine() noexcept
	{
		SINGLETON_DTOR();
	}
}