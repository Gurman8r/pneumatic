#include <editor/register_editor_types.hpp>
#include <core/extension/extension_manager.hpp>

#include <editor/editor_node.hpp>
#include <editor/editor_hierarchy.hpp>
#include <editor/editor_log.hpp>
#include <editor/editor_viewport.hpp>

namespace Pnu
{
	void register_editor_types()
	{
		REGISTER_CLASS
		(
			EditorNode,
			EditorPanel,
			EditorHierarchy,
			EditorLog,
			EditorViewport
		);
	}

	void register_editor_singletons()
	{
	}
	
	void unregister_editor_types()
	{
	}
}