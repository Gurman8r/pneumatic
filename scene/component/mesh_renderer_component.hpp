#ifndef _PN_MESH_RENDERER_COMPONENT_HPP_
#define _PN_MESH_RENDERER_COMPONENT_HPP_

#include <scene/component/renderer_component.hpp>

namespace pn
{
	class PN_API MeshRendererComponent : public RendererComponent
	{
		DEFINE_CLASS(MeshRendererComponent, RendererComponent);

	public:
		MeshRendererComponent();
		virtual ~MeshRendererComponent() override;
	};
}

#endif // !_PN_MESH_RENDERER_COMPONENT_HPP_
