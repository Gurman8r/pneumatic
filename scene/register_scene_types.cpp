#include <scene/register_scene_types.hpp>
#include <core/extension/extension_manager.hpp>

#include <scene/main/scene_tree.hpp>
#include <scene/main/window.hpp>
#include <scene/main/missing_node.hpp>
#include <scene/main/entity.hpp>

#include <scene/resources/font.hpp>
#include <scene/resources/material.hpp>
#include <scene/resources/mesh.hpp>
#include <scene/resources/shader.hpp>
#include <scene/resources/text_file.hpp>
#include <scene/resources/texture.hpp>

namespace pn
{
	void register_scene_types()
	{
		REGISTER_CLASS
		(
			SceneTree,
			Node,
			MissingNode,
			Viewport,
			Window,
			Entity,
	
			Font,
			Material,
			Mesh,
			Shader,
			TextFile,
			Texture,
			Texture2D,
			Texture3D,
			TextureCube
		);
	}

	void register_scene_singletons()
	{
	}
	
	void unregister_scene_types()
	{
	}
}