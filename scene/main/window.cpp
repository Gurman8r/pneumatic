#include <scene/main/window.hpp>
#include <scene/main/scene_tree.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Window, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Window::Window() : m_window{ DS::MAIN_WINDOW_ID }
	{
	}

	Window::~Window()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String Window::get_title() const
	{
		return get_display_server()->window_get_title(m_window);
	}

	void Window::set_title(String const & title)
	{
		get_display_server()->window_set_title(title, m_window);
	}

	i32 Window::get_current_screen() const
	{
		return get_display_server()->window_get_current_screen(m_window);
	}

	void Window::set_current_screen(i32 screen)
	{
		get_display_server()->window_set_current_screen(screen, m_window);
	}

	Vec2i Window::get_position() const
	{
		return get_display_server()->window_get_position(m_window);
	}

	void Window::set_position(Vec2i const & position)
	{
		get_display_server()->window_set_position(position, m_window);
	}

	Vec2i Window::get_size() const
	{
		return get_display_server()->window_get_size(m_window);
	}

	void Window::set_size(Vec2i const & size)
	{
		get_display_server()->window_set_size(size, m_window);
	}

	Vec2i Window::get_real_size() const
	{
		return get_display_server()->window_get_real_size(m_window);
	}

	DS::WindowMode_ Window::get_mode() const
	{
		return get_display_server()->window_get_mode(m_window);
	}

	void Window::set_mode(DS::WindowMode_ mode)
	{
		get_display_server()->window_set_mode(mode, m_window);
	}

	bool Window::get_flag(i32 flag) const
	{
		return get_display_server()->window_get_flag(flag, m_window);
	}

	void Window::set_flag(i32 flag, bool enabled)
	{
		get_display_server()->window_set_flag(flag, enabled, m_window);
	}

	void Window::request_attention()
	{
		get_display_server()->request_window_attention(m_window);
	}

	void Window::move_to_foreground()
	{
		get_display_server()->move_window_to_foreground(m_window);
	}

	void Window::set_visible(bool value)
	{
		get_display_server()->window_set_visible(value, m_window);
	}

	bool Window::is_visible() const
	{
		return get_display_server()->window_is_visible(m_window);
	}

	void Window::hide()
	{
		set_visible(false);
	}

	void Window::show()
	{
		set_visible(true);
	}

	Vec2 Window::get_scale() const
	{
		return get_display_server()->window_get_scale(m_window);
	}

	void Window::grab_focus()
	{
		get_display_server()->window_grab_focus(m_window);
	}

	bool Window::has_focus() const
	{
		return get_display_server()->window_has_focus(m_window);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
