#include <servers/rendering/renderer_3d.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Renderer3D, t) {}

	Renderer3D::Renderer3D(RenderingDevice * device, RendererStorage * storage) : m_device{ VALIDATE(device) }, m_storage{ VALIDATE(storage) }
	{
	}

	Renderer3D::~Renderer3D()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
