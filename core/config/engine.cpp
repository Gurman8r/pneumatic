#include <core/config/engine.hpp>

namespace Pnu
{
	Engine * Engine::__singleton{};

	Engine::Engine() noexcept
	{
		SINGLETON_CTOR();
	}

	Engine::~Engine() noexcept
	{
		SINGLETON_DTOR();
	}
}