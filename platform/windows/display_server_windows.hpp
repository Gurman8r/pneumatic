#ifndef _PN_DISPLAY_SERVER_WINDOWS_HPP_
#define _PN_DISPLAY_SERVER_WINDOWS_HPP_

#include <servers/display_server.hpp>

struct GLFWcursor;
struct GLFWwindow;

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// windows display server
	class PN_API DisplayServerWindows : public DisplayServer
	{
		DEFINE_CLASS(DisplayServerWindows, DisplayServer);

		struct _Window
		{
			GLFWwindow *	handle{};
			String			title{};
			i32				screen{};
			WindowMode_		mode{};
		};

		_Window * m_main_window{};

		Map<WindowID, _Window> m_windows{};

		Input::CursorShape_ m_cursor_shape{};
		GLFWcursor * m_cursors[Input::CursorShape_MAX]{};

	public:
		DisplayServerWindows(String const & title, WindowMode_ mode, Vec2i const & position, Vec2i const & size, i32 screen, Error_ & error);
		~DisplayServerWindows();

	public:
		void get_desktop_video_mode(VideoMode & out) const;
		void get_fullscreen_video_modes(Vector<VideoMode> & out) const;

	public:
		// TODO: global menu stuff goes here

	public:
		virtual void alert(String const & message, String const & title = "ALERT!") override;

		virtual String get_clipboard() const override;
		virtual void set_clipboard(String const & text) override;

		virtual Input::MouseMode_ mouse_get_mode() const override;
		virtual void mouse_set_mode(Input::MouseMode_ mode) override;

		virtual i32 mouse_get_button(Input::MouseButton_ button) const override;
		virtual Vec2 mouse_get_position() const override;
		virtual void mouse_set_position(Vec2 const & position) override;

		virtual Input::CursorShape_ cursor_get_shape() const override;
		virtual void cursor_set_shape(Input::CursorShape_ shape) override;
		virtual void cursor_set_custom_image(RES const & cursor, Input::CursorShape_ shape = {}, Vec2 const & hotspot = {}) override;

	public:
		virtual i32 get_screen_count() const override;
		virtual String screen_get_name(i32 screen = -1) const override;
		virtual Vec2i screen_get_physical_size(i32 screen = -1) const override;
		virtual IntRect screen_get_workrect(i32 screen = -1) const override;
		virtual Vec2i screen_get_workpos(i32 screen = -1) const override;
		virtual Vec2i screen_get_worksize(i32 screen = -1) const override;
		virtual Vec2 screen_get_scale(i32 screen = -1) const override;

	public:
		virtual Vector<WindowID> get_window_list() const override;

		virtual String window_get_title(WindowID window = MAIN_WINDOW_ID) const override;
		virtual void window_set_title(String const & title, WindowID window = MAIN_WINDOW_ID) override;

		virtual i32 window_get_current_screen(WindowID window = MAIN_WINDOW_ID) const override;
		virtual void window_set_current_screen(i32 screen, WindowID window = MAIN_WINDOW_ID) override;

		virtual Vec2i window_get_position(WindowID window = MAIN_WINDOW_ID) const override;
		virtual void window_set_position(Vec2i const & position, WindowID window = MAIN_WINDOW_ID) override;

		virtual Vec2i window_get_size(WindowID window = MAIN_WINDOW_ID) const override;
		virtual void window_set_size(Vec2i const & size, WindowID window = MAIN_WINDOW_ID) override;

		virtual Vec2i window_get_real_size(WindowID window = MAIN_WINDOW_ID) const override;

		virtual WindowMode_ window_get_mode(WindowID window = MAIN_WINDOW_ID) const override;
		virtual void window_set_mode(WindowMode_ mode, WindowID window = MAIN_WINDOW_ID) override;

		virtual bool window_get_flag(i32 flag, WindowID window = MAIN_WINDOW_ID) const override;
		virtual void window_set_flag(i32 flag, bool enabled, WindowID window = MAIN_WINDOW_ID) override;

		virtual void request_window_attention(WindowID window = MAIN_WINDOW_ID) override;
		virtual void move_window_to_foreground(WindowID window = MAIN_WINDOW_ID) override;

		virtual void window_set_visible(bool enabled, WindowID window = MAIN_WINDOW_ID) override;
		virtual bool window_is_visible(WindowID window = MAIN_WINDOW_ID) const override;

		virtual Vec2 window_get_scale(WindowID window = MAIN_WINDOW_ID) const override;

		virtual bool window_has_focus(WindowID window = MAIN_WINDOW_ID) const override;
		virtual void window_grab_focus(WindowID window = MAIN_WINDOW_ID) override;

	public:
		virtual void poll_events() override;
		virtual void swap_buffers() override;

		virtual void set_native_icon(String const & path) override;
		virtual void set_icon(u8 const * data, i32 width, i32 height) override;

	public:
		static void initialize();
		static void finalize();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_DISPLAY_SERVER_WINDOWS_HPP_
