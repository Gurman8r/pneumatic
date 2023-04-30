#ifndef _PNU_COLLIDER_COMPONENT_HPP_
#define _PNU_COLLIDER_COMPONENT_HPP_

#include <scene/component/component.hpp>

namespace Pnu
{
	class PNU_API ColliderComponent : public Component
	{
		DEFINE_CLASS(ColliderComponent, Component);

	public:
		ColliderComponent();
		virtual ~ColliderComponent() override;
	};
}

#endif // !_PNU_COLLIDER_COMPONENT_HPP_
