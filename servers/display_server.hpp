#ifndef _PNU_DISPLAY_SERVER_HPP_
#define _PNU_DISPLAY_SERVER_HPP_

#include <core/os/os.hpp>
#include <core/input/input.hpp>
#include <core/io/image.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// display server
	class PNU_API DisplayServer : public Object
	{
		DEFINE_CLASS(DisplayServer, Object);

		static DisplayServer * __singleton;

	public:
		using WindowID = i32;

		enum { MAIN_WINDOW_ID, INVALID_WINDOW_ID = -1 };

		enum WindowMode_
		{
			WindowMode_Windowed,
			WindowMode_Minimized,
			WindowMode_Maximized,
			WindowMode_Fullscreen,
			WindowMode_MAX
		};

		enum WindowEvent_
		{
			WindowEvent_MouseEnter,
			WindowEvent_MouseExit,
			WindowEvent_FocusIn,
			WindowEvent_FocusOut,
			WindowEvent_CloseRequest,
			WindowEvent_GoBackRequest,
			WindowEvent_SizeChanged,
			WindowEvent_ScaleChanged,
			WindowEvent_MAX
		};

		struct VideoMode
		{
			Vec2i	size{ 1280, 720 };
			Vec4b	bits_per_pixel{ 8, 8, 8, 8 };
			i32		refresh_rate{ -1 };

			NODISCARD constexpr auto compare(VideoMode const & vm) const
			{
				if (this == std::addressof(vm)) { return 0; }
				if (auto const cmp{ CMP(size, vm.size) }; cmp != 0) { return cmp; }
				if (auto const cmp{ CMP(refresh_rate, vm.refresh_rate) }; cmp != 0) { return cmp; }
				if (auto const cmp{ CMP(bits_per_pixel, vm.bits_per_pixel) }; cmp != 0) { return cmp; }
				return 0;
			}
		};

	protected:
		using CreateFunc = DisplayServer * (*)(String const & title, WindowMode_ mode, Vec2i const & position, Vec2i const & size, i32 screen, Error_ & error);
		static CreateFunc __create_func;
		DisplayServer(String const & title, WindowMode_ mode, Vec2i const & position, Vec2i const & size, i32 screen, Error_ & error);

	public:
		virtual ~DisplayServer() override;
		FORCE_INLINE static DisplayServer * get_singleton() noexcept { return __singleton; }
		static DisplayServer * create(String const & title, WindowMode_ mode, Vec2i const & position, Vec2i const & size, i32 screen, Error_ & error);

	public:
		// TODO: global menu goes here

	public:
		virtual void alert(String const & message, String const & title = "ALERT!") = 0;

		virtual String get_clipboard() const = 0;
		virtual void set_clipboard(String const & text) = 0;

		virtual Input::MouseMode_ mouse_get_mode() const = 0;
		virtual void mouse_set_mode(Input::MouseMode_ mode) = 0;

		virtual i32 mouse_get_button(Input::MouseButton_ button) const = 0;
		virtual Vec2 mouse_get_position() const = 0;
		virtual void mouse_set_position(Vec2 const & position) = 0;

		virtual Input::CursorShape_ cursor_get_shape() const = 0;
		virtual void cursor_set_shape(Input::CursorShape_ shape) = 0;
		virtual void cursor_set_custom_image(RES const & cursor, Input::CursorShape_ shape = {}, Vec2 const & hotspot = {}) = 0;

	public:
		virtual i32 get_screen_count() const = 0;
		virtual String screen_get_name(i32 screen = -1) const = 0;
		virtual Vec2i screen_get_physical_size(i32 screen = -1) const = 0;
		virtual IntRect screen_get_workrect(i32 screen = -1) const = 0;
		virtual Vec2i screen_get_workpos(i32 screen = -1) const = 0;
		virtual Vec2i screen_get_worksize(i32 screen = -1) const = 0;
		virtual Vec2 screen_get_scale(i32 screen = -1) const = 0;
		virtual Vec2 screen_get_max_scale() const {
			Vec2 scale{ 1.f, 1.f };
			for (i32 i = 0; i < get_screen_count(); ++i) {
				scale = maximum(scale, screen_get_scale(i));
			}
			return scale;
		}
		
		virtual Vector<WindowID> get_window_list() const = 0;

		virtual String window_get_title(WindowID window = MAIN_WINDOW_ID) const = 0;
		virtual void window_set_title(String const & title, WindowID window = MAIN_WINDOW_ID) = 0;

		virtual i32 window_get_current_screen(WindowID window = MAIN_WINDOW_ID) const = 0;
		virtual void window_set_current_screen(i32 screen, WindowID window = MAIN_WINDOW_ID) = 0;

		virtual Vec2i window_get_position(WindowID window = MAIN_WINDOW_ID) const = 0;
		virtual void window_set_position(Vec2i const & position, WindowID window = MAIN_WINDOW_ID) = 0;

		virtual Vec2i window_get_size(WindowID window = MAIN_WINDOW_ID) const = 0;
		virtual void window_set_size(Vec2i const & size, WindowID window = MAIN_WINDOW_ID) = 0;

		virtual Vec2i window_get_real_size(WindowID window = MAIN_WINDOW_ID) const = 0;

		virtual WindowMode_ window_get_mode(WindowID window = MAIN_WINDOW_ID) const = 0;
		virtual void window_set_mode(WindowMode_ mode, WindowID window = MAIN_WINDOW_ID) = 0;

		virtual bool window_get_flag(i32 flag, WindowID window = MAIN_WINDOW_ID) const = 0;
		virtual void window_set_flag(i32 flag, bool enabled, WindowID window = MAIN_WINDOW_ID) = 0;

		virtual void request_window_attention(WindowID window = MAIN_WINDOW_ID) = 0;
		virtual void move_window_to_foreground(WindowID window = MAIN_WINDOW_ID) = 0;

		virtual void window_set_visible(bool enabled, WindowID window = MAIN_WINDOW_ID) = 0;
		virtual bool window_is_visible(WindowID window = MAIN_WINDOW_ID) const = 0;

		virtual Vec2 window_get_scale(WindowID window = MAIN_WINDOW_ID) const = 0;

		virtual void window_grab_focus(WindowID window = MAIN_WINDOW_ID) = 0;
		virtual bool window_has_focus(WindowID window = MAIN_WINDOW_ID) const = 0;

		//virtual void window_set_window_event_callback(ObjectRef const & callable, WindowID window = MAIN_WINDOW_ID) = 0;
		//virtual void window_set_input_event_callback(ObjectRef const & callable, WindowID window = MAIN_WINDOW_ID) = 0;
		//virtual void window_set_input_text_callback(ObjectRef const & callable, WindowID window = MAIN_WINDOW_ID) = 0;

		virtual void poll_events() = 0;
		virtual void swap_buffers() = 0;

		virtual void set_native_icon(String const & path) = 0;
		virtual void set_icon(u8 const * data, i32 width, i32 height) = 0;

		void vsync_set_enabled(bool enabled);
		bool vsync_is_enabled() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using DS = DisplayServer;

	SINGLETON_WRAPPER(DS, get_display_server);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_DISPLAY_SERVER_HPP_
