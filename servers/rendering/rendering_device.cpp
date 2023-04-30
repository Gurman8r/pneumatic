#include <servers/rendering/rendering_device.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RD * RD::__singleton{};

	EMBED_CLASS(RenderingDevice, t, TypeFlags_IsAbstract) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}