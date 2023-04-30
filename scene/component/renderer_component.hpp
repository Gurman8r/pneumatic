#ifndef _PNU_RENDERER_COMPONENT_HPP_
#define _PNU_RENDERER_COMPONENT_HPP_

#include <scene/component/component.hpp>

namespace Pnu
{
	class PNU_API RendererComponent : public Component
	{
		DEFINE_CLASS(RendererComponent, Component);

	public:
		RendererComponent();
		virtual ~RendererComponent() override;
	};
}

#endif // !_PNU_RENDERER_COMPONENT_HPP_
