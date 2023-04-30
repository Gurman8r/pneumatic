#ifndef _PNU_RIGIDBODY_COMPONENT_HPP_
#define _PNU_RIGIDBODY_COMPONENT_HPP_

#include <scene/component/component.hpp>

namespace Pnu
{
	class PNU_API RigidbodyComponent : public Component
	{
		DEFINE_CLASS(RigidbodyComponent, Component);

	public:
		RigidbodyComponent();
		virtual ~RigidbodyComponent() override;
	};
}

#endif // !_PNU_RIGIDBODY_COMPONENT_HPP_
