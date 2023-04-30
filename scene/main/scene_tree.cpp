#include <scene/main/scene_tree.hpp>
#include <scene/main/missing_node.hpp>
#include <scene/main/window.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SceneTree * SceneTree::__singleton{};

	EMBED_CLASS(SceneTree, t)
	{
		t.tp_bind = BIND_CLASS(SceneTree, t)
		{
			return t
				.def("initialize", &SceneTree::initialize)
				.def("process", &SceneTree::process)
				.def("finalize", &SceneTree::finalize)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SceneTree::SceneTree()
	{
		if (__singleton == nullptr) { __singleton = this; }

		m_root = memnew(Window);
	}

	SceneTree::~SceneTree()
	{
		memdelete(m_root);

		if (__singleton == this) { __singleton = nullptr; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void SceneTree::initialize()
	{
		m_initialized = true;
		m_root->set_tree(this);
		base_type::initialize();
	}

	bool SceneTree::process(Duration const & dt)
	{
		base_type::process(dt);
		m_fps_tracker.update(dt);
		m_delta_time = dt;

		m_root->propagate_notification(Node::Notification_Internal_Process);
		m_root->propagate_notification(Node::Notification_Process);

		return m_should_close;
	}

	void SceneTree::finalize()
	{
		base_type::finalize();
		m_initialized = false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}