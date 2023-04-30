#ifndef _PN_TRANSFORM_COMPONENT_HPP_
#define _PN_TRANSFORM_COMPONENT_HPP_

#include <scene/component/component.hpp>

namespace pn
{
	class PN_API TransformComponent : public Component
	{
		DEFINE_CLASS(TransformComponent, Component);

	public:
		TransformComponent();
		virtual ~TransformComponent() override;
	};
}

#endif // !_PN_TRANSFORM_COMPONENT_HPP_
