#ifndef _PNU_SPRITE_RENDERER_COMPONENT_HPP_
#define _PNU_SPRITE_RENDERER_COMPONENT_HPP_

#include <scene/component/renderer_component.hpp>

namespace Pnu
{
	class PNU_API SpriteRendererComponent : public RendererComponent
	{
		DEFINE_CLASS(SpriteRendererComponent, RendererComponent);

	public:
		SpriteRendererComponent();
		virtual ~SpriteRendererComponent() override;
	};
}

#endif // !_PNU_SPRITE_RENDERER_COMPONENT_HPP_
