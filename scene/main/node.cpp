#include <scene/main/node.hpp>
#include <scene/main/scene_tree.hpp>
#include <core/input/input.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Node, t, TypeFlags_IsAbstract) {}

	i32 Node::__orphan_node_count{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Node::_notification(Notification_ value)
	{
		switch (value)
		{
		case Notification_Process: {
			_process(get_tree()->get_delta_time());
		} break;
		case Notification_PhysicsProcess: {
			_physics_process(get_tree()->get_delta_time());
		} break;
		case Notification_EnterTree: {
			_enter_tree();
		} break;
		case Notification_ExitTree: {
			_exit_tree();
		} break;
		case Notification_Ready: {
			_ready();
		} break;
		case Notification_PathChanged: {
		} break;
		case Notification_PostInitialize: {
		} break;
		case Notification_PreDelete: {
		} break;
		}
	}

	void Node::_process(Duration const delta_time)
	{
	}

	void Node::_physics_process(Duration const delta_time)
	{
	}

	void Node::_enter_tree()
	{
	}

	void Node::_exit_tree()
	{
	}

	void Node::_ready()
	{
	}

	void Node::_input(InputEvent const & input_event)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String Node::get_name() const
	{
		return m_data.name;
	}

	void Node::set_name(String const & name)
	{
	}

	SceneTree * Node::get_tree() const
	{
		return m_data.tree;
	}

	bool Node::set_tree(SceneTree * tree)
	{
		if (!tree || m_data.tree == tree) { return false; }
		m_data.tree = tree;
		for (Node * child : m_data.children) { child->set_tree(tree); }
		return true;
	}

	Node * Node::get_parent() const
	{
		return m_data.parent;
	}

	bool Node::set_parent(Node * parent)
	{
		if (!parent || this == parent || m_data.parent == parent) { return false; }

		// add to new parent
		parent->m_data.children.push_back(this);

		// remove from old parent
		if (m_data.parent) {
			m_data.parent->m_data.children.erase(m_data.parent->m_data.children.begin() + get_sibling_index());
		}

		m_data.parent = parent;
		m_data.tree = parent->m_data.tree;
		return true;
	}

	Node * Node::get_sibling(size_t const index) const
	{
		ASSERT(m_data.parent && index < m_data.parent->m_data.children.size());

		return m_data.parent->m_data.children[index];
	}

	size_t Node::get_sibling_index() const
	{
		size_t i{};

		while (m_data.parent && (i < m_data.parent->m_data.children.size()) && (this != m_data.parent->m_data.children[i])) { ++i; }

		return i;
	}

	void Node::set_sibling_index(size_t new_index)
	{
		ASSERT(m_data.parent);

		ASSERT(new_index < m_data.parent->m_data.children.size());
		
		size_t const old_index{ get_sibling_index() };

		if (new_index == old_index) { return; }
		
		util::swap(m_data.parent->m_data.children[new_index], m_data.parent->m_data.children[old_index]);
	}

	Node * Node::get_child(size_t const index) const
	{
		VERIFY_RANGE(index, -1, get_child_count()); return m_data.children[index];
	}

	size_t Node::get_child_count() const
	{
		return m_data.children.size();
	}

	Node * Node::add_child(Node * child)
	{
		return (child && child->set_parent(this)) ? child : nullptr;
	}

	void Node::destroy_child(size_t const index)
	{
		ASSERT(index < m_data.children.size());

		auto const it{ m_data.children.begin() + index };

		if (*it) { memdelete(*it); }

		m_data.children.erase(it);
	}

	bool Node::is_child_of(Node const * parent, bool recursive) const
	{
		if ((this == parent) || !parent || !m_data.parent) { return false; }
		else if (m_data.parent == parent) { return true; }
		else if (recursive)
		{
			Node * it{ m_data.parent->m_data.parent };
			while (it)
			{
				if (it == parent) { return true; }
				it = it->m_data.parent;
			}
		}
		return false;
	}

	bool Node::is_parent_of(Node const * child, bool recursive) const
	{
		if ((this == child) || !child || !child->m_data.parent) { return false; }
		else if (this == child->m_data.parent) { return true; }
		else if (recursive)
		{
			Node * it{ child->m_data.parent->m_data.parent };
			while (it)
			{
				if (it == this) { return true; }
				it = it->m_data.parent;
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Node::propagate_notification(i32 id, bool reverse)
	{
		notification(id, reverse);

		for (Node * child : m_data.children)
		{
			child->propagate_notification(id, reverse);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Node::Node() { ++__orphan_node_count; }

	Node::~Node()
	{
		while (!m_data.children.empty()) {
			memdelete(m_data.children.back());
			m_data.children.pop_back();
		}

		--__orphan_node_count;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}