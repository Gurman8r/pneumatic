#ifndef _PN_RENDERER_CANVAS_RENDERER_HPP_
#define _PN_RENDERER_CANVAS_RENDERER_HPP_

#include <servers/rendering/renderer_storage.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// canvas renderer
	class PN_API Renderer2D : public Object
	{
		DEFINE_CLASS(Renderer2D, Object);
		
		friend class RenderingServerDefault;

		RenderingDevice * const m_device;
		RendererStorage * const m_storage;

	public:
		Renderer2D(RenderingDevice * device, RendererStorage * storage);

		virtual ~Renderer2D() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_RENDERER_CANVAS_RENDERER_HPP_
