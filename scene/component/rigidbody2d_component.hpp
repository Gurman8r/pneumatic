#ifndef _PN_RIGIDBODY2D_COMPONENT_HPP_
#define _PN_RIGIDBODY2D_COMPONENT_HPP_

#include <scene/component/component.hpp>

namespace pn
{
	class PN_API Rigidbody2DComponent : public Component
	{
		DEFINE_CLASS(Rigidbody2DComponent, Component);

	public:
		Rigidbody2DComponent();
		virtual ~Rigidbody2DComponent() override;
	};
}

#endif // !_PN_RIGIDBODY2D_COMPONENT_HPP_
