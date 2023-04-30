#ifndef _PN_RENDERER_COMPONENT_HPP_
#define _PN_RENDERER_COMPONENT_HPP_

#include <scene/component/component.hpp>

namespace pn
{
	class PN_API RendererComponent : public Component
	{
		DEFINE_CLASS(RendererComponent, Component);

	public:
		RendererComponent();
		virtual ~RendererComponent() override;
	};
}

#endif // !_PN_RENDERER_COMPONENT_HPP_
