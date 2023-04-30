#include <servers/register_server_types.hpp>
#include <core/extension/extension_manager.hpp>

#include <servers/display_server.hpp>
#include <servers/rendering/rendering_server_default.hpp>
#include <servers/text_server.hpp>

namespace pn
{
	void register_server_types()
	{
		REGISTER_CLASS
		(
			DisplayServer,
			RenderingDevice,
			RenderingServerDefault,
			RenderingServer,
			TextServer
		);
	}

	void register_server_singletons()
	{
	}

	void unregister_server_types()
	{
	}
}