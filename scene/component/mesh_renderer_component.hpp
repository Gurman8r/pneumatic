#ifndef _PNU_MESH_RENDERER_COMPONENT_HPP_
#define _PNU_MESH_RENDERER_COMPONENT_HPP_

#include <scene/component/renderer_component.hpp>

namespace Pnu
{
	class PNU_API MeshRendererComponent : public RendererComponent
	{
		DEFINE_CLASS(MeshRendererComponent, RendererComponent);

	public:
		MeshRendererComponent();
		virtual ~MeshRendererComponent() override;
	};
}

#endif // !_PNU_MESH_RENDERER_COMPONENT_HPP_
