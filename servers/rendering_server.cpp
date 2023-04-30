#include <servers/rendering_server.hpp>
#include <servers/rendering/rendering_server_default.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(RenderingServer, t, TypeFlags_IsAbstract) {}

	RenderingServer * RenderingServer::__singleton{};

	RenderingServer::CreateFunc RenderingServer::__create_func{ []() { return (RenderingServer *)memnew(RenderingServerDefault); } };

	RenderingServer::RenderingServer() { SINGLETON_CTOR(); }

	RenderingServer::~RenderingServer() { SINGLETON_DTOR(); }

	RenderingServer * RenderingServer::create()
	{
		return VALIDATE(__create_func)();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}