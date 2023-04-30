#ifndef _PN_RIGIDBODY_COMPONENT_HPP_
#define _PN_RIGIDBODY_COMPONENT_HPP_

#include <scene/component/component.hpp>

namespace pn
{
	class PN_API RigidbodyComponent : public Component
	{
		DEFINE_CLASS(RigidbodyComponent, Component);

	public:
		RigidbodyComponent();
		virtual ~RigidbodyComponent() override;
	};
}

#endif // !_PN_RIGIDBODY_COMPONENT_HPP_
