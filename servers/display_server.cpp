#include <servers/display_server.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(DisplayServer, t, TypeFlags_IsAbstract) {}

	DisplayServer * DisplayServer::__singleton{};

	DisplayServer::CreateFunc DisplayServer::__create_func{};

	DisplayServer::DisplayServer(String const & title, WindowMode_ mode, Vec2i const & position, Vec2i const & size, i32 screen, Error_ & error)
	{
		SINGLETON_CTOR();

		Input::m_funcs.get_mouse_mode = []() -> Input::MouseMode_ { return get_display_server()->mouse_get_mode(); };
		Input::m_funcs.set_mouse_mode = [](Input::MouseMode_ mode) { get_display_server()->mouse_set_mode(mode); };
		Input::m_funcs.set_mouse_position = [](Vec2 const & position) { get_display_server()->mouse_set_position(position); };
		Input::m_funcs.get_cursor_shape = []() -> Input::CursorShape_ { return get_display_server()->cursor_get_shape(); };
		Input::m_funcs.set_cursor_shape = [](Input::CursorShape_ shape) { get_display_server()->cursor_set_shape(shape); };
		Input::m_funcs.set_custom_cursor_image = [](RES const & cursor, Input::CursorShape_ shape, Vec2 const & hotspot) { get_display_server()->cursor_set_custom_image(cursor, shape, hotspot); };
	}

	DisplayServer::~DisplayServer()
	{
		SINGLETON_DTOR();
	}

	DisplayServer * DisplayServer::create(String const & title, WindowMode_ mode, Vec2i const & position, Vec2i const & size, i32 screen, Error_ & error)
	{
		return VALIDATE(__create_func)(title, mode, position, size, screen, error);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void DisplayServer::vsync_set_enabled(bool value)
	{
	}

	bool DisplayServer::vsync_is_enabled() const
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}