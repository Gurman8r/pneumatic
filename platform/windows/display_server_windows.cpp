#include <platform/windows/display_server_windows.hpp>
#include <scene/main/window.hpp>

#include <glfw/glfw3.h>
#if defined(SYSTEM_WINDOWS)
#	undef APIENTRY
#	include <Windows.h>
#	define GLFW_EXPOSE_NATIVE_WIN32
#	include <glfw/glfw3native.h>
#endif

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(DisplayServerWindows, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DisplayServerWindows::DisplayServerWindows(String const & title, DS::WindowMode_ mode, Vec2i const & position, Vec2i const & size, i32 screen, Error_ & error)
		: DisplayServer{ title, mode, position, size, screen, error }
	{
		ASSERT(title);
		ASSERT(0 < size[0]);
		ASSERT(0 < size[1]);
		VERIFY_RANGE(mode, -1, WindowMode_MAX);

		/* GLFW SETUP */

		ASSERT(glfwInit() == GLFW_TRUE);

		glfwSetErrorCallback([](i32 code, cstring message)
		{
			get_os()->printerrf("glfw error %i: %s", code, message);
		});

		glfwSetMonitorCallback([](GLFWmonitor * monitor, i32 connected)
		{
			// monitor connected/disconnected
		});

		glfwSetJoystickCallback([](i32 device, i32 connected)
		{
			get_input()->joy_connection_changed(device, connected == GLFW_CONNECTED, glfwGetJoystickName(device), glfwGetJoystickGUID(device));
		});

		// cursors
		m_cursors[Input::CursorShape_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		m_cursors[Input::CursorShape_IBeam] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		m_cursors[Input::CursorShape_Crosshair] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
		m_cursors[Input::CursorShape_PointingHand] = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);
		m_cursors[Input::CursorShape_EW] = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
		m_cursors[Input::CursorShape_NS] = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
		m_cursors[Input::CursorShape_NESW] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
		m_cursors[Input::CursorShape_NWSE] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
		m_cursors[Input::CursorShape_ResizeAll] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
		m_cursors[Input::CursorShape_NotAllowed] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
		m_cursors[Input::CursorShape_HResize] = nullptr;
		m_cursors[Input::CursorShape_VResize] = nullptr;
		m_cursors[Input::CursorShape_Hand] = nullptr;

		/* WINDOW HINTS */

		// context creation hints
#if OPENGL_ENABLED
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#else
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

		// window hints
		glfwWindowHint(GLFW_RESIZABLE, true);
		glfwWindowHint(GLFW_VISIBLE, mode != WindowMode_Maximized);
		glfwWindowHint(GLFW_DECORATED, true);
		glfwWindowHint(GLFW_FOCUSED, true);
		glfwWindowHint(GLFW_AUTO_ICONIFY, true);
		glfwWindowHint(GLFW_CENTER_CURSOR, true);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW, true);
		glfwWindowHint(GLFW_FLOATING, false);
		glfwWindowHint(GLFW_MAXIMIZED, mode == WindowMode_Maximized);

		// framebuffer hints
		glfwWindowHint(GLFW_RED_BITS, 8);
		glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8);
		glfwWindowHint(GLFW_ALPHA_BITS, 8);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		glfwWindowHint(GLFW_STENCIL_BITS, 8);
		glfwWindowHint(GLFW_DOUBLEBUFFER, false);
		glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

		/* CREATE WINDOW */

		_Window & w{ m_windows[MAIN_WINDOW_ID] = {} };
		w.title = title;
		w.mode = mode;
		w.handle = VALIDATE(glfwCreateWindow(size[0], size[1], w.title.c_str(), nullptr, nullptr));
		glfwSetWindowUserPointer(w.handle, &w);

		switch (mode) {
		case WindowMode_Windowed: {} break;
		case WindowMode_Minimized: { glfwIconifyWindow(w.handle); } break;
		case WindowMode_Maximized: { glfwMaximizeWindow(w.handle); } break;
		case WindowMode_Fullscreen: {} break;
		}

		/* WINDOW CALLBACKS */

		glfwSetCharCallback(w.handle, [](GLFWwindow *, u32 codepoint) {});
		
		glfwSetKeyCallback(w.handle, [](GLFWwindow *, i32 key, i32, i32 action, i32) {
			get_input()->set_key(key, (Input::Action_)action);
		});

		glfwSetMouseButtonCallback(w.handle, [](GLFWwindow *, i32 button, i32 action, i32) {
			get_input()->set_mouse_button(button, (Input::Action_)action);
		});

		glfwSetCursorPosCallback(w.handle, [](GLFWwindow *, f64 x, f64 y) {
			get_input()->set_mouse_position({ (f32)x, (f32)y });
		});

		glfwSetScrollCallback(w.handle, [](GLFWwindow *, f64 x, f64 y) {
			get_input()->set_mouse_wheel({ (f32)x, (f32)y });
		});

		glfwSetCursorEnterCallback(w.handle, [](GLFWwindow *, i32 entered) {
			if (auto const tree{ SceneTree::get_singleton() }) {
				tree->get_root()->propagate_notification(entered ? Node::Notification_WM_MouseEnter : Node::Notification_WM_MouseExit);
			}
		});

		glfwSetWindowCloseCallback(w.handle, [](GLFWwindow *) {
			if (auto const tree{ SceneTree::get_singleton() }) {
				tree->get_root()->propagate_notification(Node::Notification_WM_CloseRequest);
				tree->quit();
			}
		});

		glfwSetWindowFocusCallback(w.handle, [](GLFWwindow *, i32 focused) {
			if (auto const tree{ SceneTree::get_singleton() }) {
				tree->get_root()->propagate_notification(focused ? Node::Notification_WM_FocusIn : Node::Notification_WM_FocusOut);
			}
		});

		glfwSetWindowContentScaleCallback(w.handle, [](GLFWwindow *, f32 x, f32 y) {
			if (auto const tree{ SceneTree::get_singleton() }) {
				tree->get_root()->propagate_notification(Node::Notification_WM_ScaleChanged);
			}
		});

		glfwSetWindowSizeCallback(w.handle, [](GLFWwindow *, i32 w, i32 h) {
			if (auto const tree{ SceneTree::get_singleton() }) {
				tree->get_root()->propagate_notification(Node::Notification_WM_SizeChanged);
			}
		});

		/* DONE */
		glfwMakeContextCurrent(w.handle);
		m_main_window = &w;
	}

	DisplayServerWindows::~DisplayServerWindows()
	{
		// cleanup windows
		for (auto const & [k, v] : m_windows) {
			glfwDestroyWindow(v.handle);
		}

		// cleanup cursors
		for (GLFWcursor * cursor : m_cursors) {
			if (cursor) {
				glfwDestroyCursor(cursor);
			}
		}

		glfwTerminate();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void DisplayServerWindows::get_desktop_video_mode(VideoMode & out) const
	{
		DEVMODE dm; dm.dmSize = sizeof(dm);
		EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm);
		out.size = { (i32)dm.dmPelsWidth, (i32)dm.dmPelsHeight };
		out.bits_per_pixel = bit_cast<Vec4b>(dm.dmBitsPerPel);
		out.refresh_rate = dm.dmDisplayFrequency;
	}

	void DisplayServerWindows::get_fullscreen_video_modes(Vector<VideoMode> & out) const
	{
		DEVMODE dm; dm.dmSize = sizeof(dm);
		for (i32 i = 0; EnumDisplaySettings(nullptr, i, &dm); ++i)
		{
			VideoMode vm;
			vm.size = { (i32)dm.dmPelsWidth, (i32)dm.dmPelsHeight };
			vm.bits_per_pixel = bit_cast<Vec4b>(dm.dmBitsPerPel);
			vm.refresh_rate = dm.dmDisplayFrequency;

			// insert
			if (auto const it{ std::equal_range(out.begin(), out.end(), vm, [
			](VideoMode const & a, VideoMode const & b) { return a.compare(b) < 0; }) }
			; it.first == it.second) {
				out.emplace(it.second, std::move(vm));
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// TODO: global menu stuff goes here

	void DisplayServerWindows::alert(String const & message, String const & title)
	{
	}

	String DisplayServerWindows::get_clipboard() const
	{
		ASSERT(m_main_window);
		return glfwGetClipboardString(m_main_window->handle);
	}

	void DisplayServerWindows::set_clipboard(String const & text)
	{
		ASSERT(m_main_window);
		glfwSetClipboardString(m_main_window->handle, text.c_str());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Input::MouseMode_ DisplayServerWindows::mouse_get_mode() const
	{
		ASSERT(m_main_window);
		switch (glfwGetInputMode(m_main_window->handle, GLFW_CURSOR)) {
		case GLFW_CURSOR_NORMAL: return Input::MouseMode_Normal;
		case GLFW_CURSOR_HIDDEN: return Input::MouseMode_Hidden;
		case GLFW_CURSOR_DISABLED: return Input::MouseMode_Disabled;
		}
		return Input::MouseMode_MAX;
	}

	void DisplayServerWindows::mouse_set_mode(Input::MouseMode_ mode)
	{
		ASSERT(m_main_window);
		glfwSetInputMode(m_main_window->handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	i32 DisplayServerWindows::mouse_get_button(Input::MouseButton_ button) const
	{
		ASSERT(m_main_window);
		return glfwGetMouseButton(m_main_window->handle, (i32)button);
	}

	Vec2 DisplayServerWindows::mouse_get_position() const
	{
		ASSERT(m_main_window);
		Vec2d pos; glfwGetCursorPos(m_main_window->handle, &pos[0], &pos[1]);
		return { (f32)pos[0], (f32)pos[1] };
	}

	void DisplayServerWindows::mouse_set_position(Vec2 const & position)
	{
		ASSERT(m_main_window);
		glfwSetCursorPos(m_main_window->handle, position[0], position[1]);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Input::CursorShape_ DisplayServerWindows::cursor_get_shape() const
	{
		return m_cursor_shape;
	}

	void DisplayServerWindows::cursor_set_shape(Input::CursorShape_ shape)
	{
		ASSERT(m_main_window);
		if (m_cursor_shape == shape) { return; }
		m_cursor_shape = shape;
		glfwSetCursor(m_main_window->handle, m_cursors[shape]);
	}

	void DisplayServerWindows::cursor_set_custom_image(RES const & cursor, Input::CursorShape_ shape, Vec2 const & hotspot)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	i32 DisplayServerWindows::get_screen_count() const
	{
		i32 monitor_count;
		UNUSED(glfwGetMonitors(&monitor_count));
		return monitor_count;
	}

	String DisplayServerWindows::screen_get_name(i32 screen) const
	{
		i32 monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (-1 < screen) ? screen : window_get_current_screen();
		VERIFY_RANGE(screen, -1, monitor_count);
		return glfwGetMonitorName(monitors[screen]);
	}

	Vec2i DisplayServerWindows::screen_get_physical_size(i32 screen) const
	{
		i32 monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (-1 < screen) ? screen : window_get_current_screen();
		VERIFY_RANGE(screen, -1, monitor_count);
		Vec2i size; glfwGetMonitorPhysicalSize(monitors[screen], &size[0], &size[1]);
		return size;
	}

	IntRect DisplayServerWindows::screen_get_workrect(i32 screen) const
	{
		i32 monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (-1 < screen) ? screen : window_get_current_screen();
		VERIFY_RANGE(screen, -1, monitor_count);
		IntRect rect; glfwGetMonitorWorkarea(monitors[screen], &rect[0], &rect[1], &rect[2], &rect[3]);
		return rect;
	}

	Vec2i DisplayServerWindows::screen_get_workpos(i32 screen) const
	{
		i32 monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (-1 < screen) ? screen : window_get_current_screen();
		VERIFY_RANGE(screen, -1, monitor_count);
		Vec2i pos; glfwGetMonitorPos(monitors[screen], &pos[0], &pos[1]);
		return pos;
	}

	Vec2i DisplayServerWindows::screen_get_worksize(i32 screen) const
	{
		i32 monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (-1 < screen) ? screen : window_get_current_screen();
		VERIFY_RANGE(screen, -1, monitor_count);
		GLFWvidmode const * vm{ glfwGetVideoMode(monitors[screen]) };
		return { vm->width, vm->height };
	}

	Vec2 DisplayServerWindows::screen_get_scale(i32 screen) const
	{
		i32 monitor_count;
		GLFWmonitor ** monitors{ glfwGetMonitors(&monitor_count) };
		screen = (-1 < screen) ? screen : window_get_current_screen();
		VERIFY_RANGE(screen, -1, monitor_count);
		Vec2 scale; glfwGetMonitorContentScale(monitors[screen], &scale[0], &scale[1]);
		return scale;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Vector<DS::WindowID> DisplayServerWindows::get_window_list() const
	{
		Vector<WindowID> temp{};
		temp.reserve(m_windows.size());
		for (auto const & [k, v] : m_windows) { temp.push_back(k); }
		return temp;
	}

	String DisplayServerWindows::window_get_title(WindowID window) const
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		return w.title;
	}

	void DisplayServerWindows::window_set_title(String const & title, WindowID window)
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		if (!title) { return; }
		w.title = title;
		glfwSetWindowTitle(w.handle, w.title.c_str());
	}

	i32 DisplayServerWindows::window_get_current_screen(WindowID window) const
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		return w.screen;
	}

	void DisplayServerWindows::window_set_current_screen(i32 screen, WindowID window)
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		if (w.screen == screen) { return; }
		w.screen = screen;
	}

	Vec2i DisplayServerWindows::window_get_position(WindowID window) const
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		Vec2i pos; glfwGetWindowPos(w.handle, &pos[0], &pos[1]);
		return pos;
	}

	void DisplayServerWindows::window_set_position(Vec2i const & position, WindowID window)
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		glfwSetWindowPos(w.handle, (i32)position[0], (i32)position[1]);
	}

	Vec2i DisplayServerWindows::window_get_size(WindowID window) const
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		Vec2i size; glfwGetWindowSize(w.handle, &size[0], &size[1]);
		return size;
	}

	void DisplayServerWindows::window_set_size(Vec2i const & size, WindowID window)
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		glfwSetWindowSize(w.handle, (i32)size[0], (i32)size[1]);
	}

	Vec2i DisplayServerWindows::window_get_real_size(WindowID window) const
	{
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		Vec2i size; glfwGetFramebufferSize(w.handle, &size[0], &size[1]);
		return size;
	}

	DS::WindowMode_ DisplayServerWindows::window_get_mode(WindowID window) const
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		return w.mode;
	}

	void DisplayServerWindows::window_set_mode(WindowMode_ mode, WindowID window)
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		if (w.mode == mode) { return; }
		w.mode = mode;
		switch (mode) {
		case WindowMode_Windowed: {
			//glfwRestoreWindow(w.handle);
		} break;
		case WindowMode_Minimized: {
			//glfwIconifyWindow(w.handle);
		} break;
		case WindowMode_Maximized: {
			//glfwMaximizeWindow(w.handle);
		} break;
		case WindowMode_Fullscreen: {
		} break;
		}
	}

	bool DisplayServerWindows::window_get_flag(i32 flag, WindowID window) const
	{
		return false;
	}

	void DisplayServerWindows::window_set_flag(i32 flag, bool enabled, WindowID window)
	{
	}

	void DisplayServerWindows::request_window_attention(WindowID window)
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		glfwRequestWindowAttention(w.handle);
	}

	void DisplayServerWindows::move_window_to_foreground(WindowID window)
	{
	}

	void DisplayServerWindows::window_set_visible(bool enabled, WindowID window)
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		enabled ? glfwShowWindow(w.handle) : glfwHideWindow(w.handle);
	}

	bool DisplayServerWindows::window_is_visible(WindowID window) const
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		return glfwGetWindowAttrib(w.handle, GLFW_VISIBLE);
	}

	Vec2 DisplayServerWindows::window_get_scale(WindowID window) const
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		Vec2 scale; glfwGetWindowContentScale(w.handle, &scale[0], &scale[1]);
		return scale;
	}

	void DisplayServerWindows::window_grab_focus(WindowID window)
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window & w{ *VALIDATE(util::getptr(m_windows, window)) };
		glfwFocusWindow(w.handle);
	}

	bool DisplayServerWindows::window_has_focus(WindowID window) const
	{
		ASSERT(INVALID_WINDOW_ID < window);
		_Window const & w{ *VALIDATE(util::getptr(m_windows, window)) };
		return glfwGetWindowAttrib(w.handle, GLFW_FOCUSED);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void DisplayServerWindows::poll_events()
	{
		glfwPollEvents();

		for (i32 device = 0; device < Input::Joy_MAX; ++device)
		{
			if (!glfwJoystickPresent(device)) {
				continue;
			}
			
			i32 num_axes;
			f32 const * axes{ glfwGetJoystickAxes(device, &num_axes) };
			num_axes = MIN(num_axes, Input::JoyAxis_MAX);
			for (i32 axis = 0; axis < num_axes; ++axis) {
				get_input()->set_joy_axis(device, axis, axes[axis]);
			}

			i32 num_buttons;
			u8 const * buttons{ glfwGetJoystickButtons(device, &num_buttons) };
			num_buttons = MIN(num_buttons, Input::JoyButton_MAX);
			for (i32 i = 0; i < num_buttons; ++i) {
				get_input()->set_joy_button(device, i, (Input::Action_)buttons[i]);
			}
		}
	}

	void DisplayServerWindows::swap_buffers()
	{
		ASSERT(m_main_window);
		glfwMakeContextCurrent(m_main_window->handle);
		glfwSwapBuffers(m_main_window->handle);
	}

	void DisplayServerWindows::set_native_icon(String const & value)
	{
	}

	void DisplayServerWindows::set_icon(u8 const * data, i32 width, i32 height)
	{
		ASSERT(m_main_window);
		ASSERT(data);
		ASSERT(0 < width);
		ASSERT(0 < height);
		GLFWimage icon;
		icon.pixels = (u8 *)data;
		icon.width = width;
		icon.height = height;
		glfwSetWindowIcon(m_main_window->handle, 1, &icon);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void DisplayServerWindows::initialize()
	{
		DisplayServer::__create_func = [](String const & title, WindowMode_ mode, Vec2i const & position, Vec2i const & size, i32 screen, Error_ & error) -> DisplayServer *
		{
			return memnew(DisplayServerWindows(title, mode, position, size, screen, error));
		};
	}

	void DisplayServerWindows::finalize()
	{
		DisplayServer::__create_func = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

}