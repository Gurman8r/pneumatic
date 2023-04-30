#include <core/config/engine.hpp>

namespace pn
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