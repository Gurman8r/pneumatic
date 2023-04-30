#ifndef _PN_SPRITE_RENDERER_COMPONENT_HPP_
#define _PN_SPRITE_RENDERER_COMPONENT_HPP_

#include <scene/component/renderer_component.hpp>

namespace pn
{
	class PN_API SpriteRendererComponent : public RendererComponent
	{
		DEFINE_CLASS(SpriteRendererComponent, RendererComponent);

	public:
		SpriteRendererComponent();
		virtual ~SpriteRendererComponent() override;
	};
}

#endif // !_PN_SPRITE_RENDERER_COMPONENT_HPP_
