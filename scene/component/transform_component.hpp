#ifndef _PNU_TRANSFORM_COMPONENT_HPP_
#define _PNU_TRANSFORM_COMPONENT_HPP_

#include <scene/component/component.hpp>

namespace Pnu
{
	class PNU_API TransformComponent : public Component
	{
		DEFINE_CLASS(TransformComponent, Component);

	public:
		TransformComponent();
		virtual ~TransformComponent() override;
	};
}

#endif // !_PNU_TRANSFORM_COMPONENT_HPP_
