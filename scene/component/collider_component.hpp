#ifndef _PN_COLLIDER_COMPONENT_HPP_
#define _PN_COLLIDER_COMPONENT_HPP_

#include <scene/component/component.hpp>

namespace pn
{
	class PN_API ColliderComponent : public Component
	{
		DEFINE_CLASS(ColliderComponent, Component);

	public:
		ColliderComponent();
		virtual ~ColliderComponent() override;
	};
}

#endif // !_PN_COLLIDER_COMPONENT_HPP_
