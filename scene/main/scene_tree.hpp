#ifndef _PN_SCENE_TREE_HPP_
#define _PN_SCENE_TREE_HPP_

#include <core/os/main_loop.hpp>

#include <main/performance.hpp>

#include <entt/entt.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Node;
	class Entity;
	class Viewport;
	class Window;
	class InputEvent;

	using EntityID = entt::entity;
	using EntityRegistry = entt::registry;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// scene tree
	class PN_API SceneTree : public MainLoop
	{
		DEFINE_CLASS(SceneTree, MainLoop);

		friend class Main;
		friend class Node;
		friend class Entity;

		static SceneTree * __singleton;
		
		bool			m_initialized : 1, m_should_close : 1;
		Clock			m_main_timer{};
		Duration		m_delta_time{};
		FPS_Tracker		m_fps_tracker{};
		Window *		m_root{};
		EntityRegistry	m_ecs{};

	public:
		SceneTree();
		virtual ~SceneTree() override;
		FORCE_INLINE static SceneTree * get_singleton() noexcept { return __singleton; }

	public:
		virtual void initialize() override;
		virtual bool process(Duration const & dt) override;
		virtual void finalize() override;

		void quit() { m_should_close = true; }

	public:
		auto get_root() const noexcept -> Window * { return m_root; }
		auto get_delta_time() const noexcept -> Duration { return m_delta_time; }
		auto get_elapsed_time() const noexcept -> Duration { return m_main_timer.get_elapsed_time(); }
		auto get_fps() const noexcept -> FPS_Tracker const & { return m_fps_tracker; }
		auto get_ecs() const noexcept -> EntityRegistry & { return (EntityRegistry &)(m_ecs); }

	protected:
		template <class T> void on_component_added(class Entity &, T &) {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_WRAPPER(SceneTree, get_current_scene_tree);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_SCENE_TREE_HPP_
