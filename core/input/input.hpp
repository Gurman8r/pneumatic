#ifndef _PNU_INPUT_HPP_
#define _PNU_INPUT_HPP_

#include <core/input/input_map.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// input
	class PNU_API Input : public Object
	{
		DEFINE_CLASS(Input, Object);

		static Input * __singleton;

	public:
		Input() noexcept { SINGLETON_CTOR(); }
		virtual ~Input() noexcept override { SINGLETON_DTOR(); }
		FORCE_INLINE static Input * get_singleton() noexcept { return __singleton; }

	public:
		enum Action_ : u8
		{
			Action_Release	= 0,
			Action_Press	= 1,
			Action_Repeat	= 2,
		};

		enum MouseButton_
		{
			MouseButton_0 = 0,
			MouseButton_1 = 1,
			MouseButton_2 = 2,
			MouseButton_3 = 3,
			MouseButton_4 = 4,
			MouseButton_5 = 5,
			MouseButton_6 = 6,
			MouseButton_7 = 7,
			MouseButton_MAX
		};

		enum MouseMode_
		{
			MouseMode_Normal	= 0,
			MouseMode_Hidden	= 1,
			MouseMode_Disabled	= 2,
			MouseMode_MAX
		};

		enum CursorShape_
		{
			CursorShape_Arrow			= 0,
			CursorShape_IBeam			= 1,
			CursorShape_Crosshair		= 2,
			CursorShape_PointingHand	= 3,
			CursorShape_EW				= 4,
			CursorShape_NS				= 5,
			CursorShape_NESW			= 6,
			CursorShape_NWSE			= 7,
			CursorShape_ResizeAll		= 8,
			CursorShape_NotAllowed		= 9,
			CursorShape_HResize			= 10,
			CursorShape_VResize			= 11,
			CursorShape_Hand			= 12,
			CursorShape_MAX
		};

		enum Key_
		{
			Key_Any				= -1,
			Key_Space			= 32,
			Key_Apostrophe		= 39,
			Key_Comma			= 44,
			Key_Minus			= 45,
			Key_Period			= 46,
			Key_Slash			= 47,
			Key_Num0			= 48,
			Key_Num1			= 49,
			Key_Num2			= 50,
			Key_Num3			= 51,
			Key_Num4			= 52,
			Key_Num5			= 53,
			Key_Num6			= 54,
			Key_Num7			= 55,
			Key_Num8			= 56,
			Key_Num9			= 57,
			Key_Semicolon		= 59,
			Key_Equal			= 61,
			Key_A				= 65,
			Key_B				= 66,
			Key_C				= 67,
			Key_D				= 68,
			Key_E				= 69,
			Key_F				= 70,
			Key_G				= 71,
			Key_H				= 72,
			Key_I				= 73,
			Key_J				= 74,
			Key_K				= 75,
			Key_L				= 76,
			Key_M				= 77,
			Key_N				= 78,
			Key_O				= 79,
			Key_P				= 80,
			Key_Q				= 81,
			Key_R				= 82,
			Key_S				= 83,
			Key_T				= 84,
			Key_U				= 85,
			Key_V				= 86,
			Key_W				= 87,
			Key_X				= 88,
			Key_Y				= 89,
			Key_Z				= 90,
			Key_LeftBracket		= 91,
			Key_Backslash		= 92,
			Key_RightBracket	= 93,
			Key_GraveAccent		= 96,
			Key_World1			= 161,
			Key_World2			= 162,
			Key_Escape			= 256,
			Key_Enter			= 257,
			Key_Tab				= 258,
			Key_Backspace		= 259,
			Key_Insert			= 260,
			Key_Del				= 261,
			Key_Right			= 262,
			Key_Left			= 263,
			Key_Down			= 264,
			Key_Up				= 265,
			Key_PageUp			= 266,
			Key_PageDown		= 267,
			Key_Home			= 268,
			Key_End				= 269,
			Key_CapsLock		= 280,
			Key_ScrollLock		= 281,
			Key_NumLock			= 282,
			Key_PrintScreen		= 283,
			Key_Pause			= 284,
			Key_F1				= 290,
			Key_F2				= 291,
			Key_F3				= 292,
			Key_F4				= 293,
			Key_F5				= 294,
			Key_F6				= 295,
			Key_F7				= 296,
			Key_F8				= 297,
			Key_F9				= 298,
			Key_F10				= 299,
			Key_F11				= 300,
			Key_F12				= 301,
			Key_F13				= 302,
			Key_F14				= 303,
			Key_F15				= 304,
			Key_F16				= 305,
			Key_F17				= 306,
			Key_F18				= 307,
			Key_F19				= 308,
			Key_F20				= 309,
			Key_F21				= 310,
			Key_F22				= 311,
			Key_F23				= 312,
			Key_F24				= 313,
			Key_F25				= 314,
			Key_Pad0			= 320,
			Key_Pad1			= 321,
			Key_Pad2			= 322,
			Key_Pad3			= 323,
			Key_Pad4			= 324,
			Key_Pad5			= 325,
			Key_Pad6			= 326,
			Key_Pad7			= 327,
			Key_Pad8			= 328,
			Key_Pad9			= 329,
			Key_PadDecimal		= 330,
			Key_PadDivide		= 331,
			Key_PadMultiply		= 332,
			Key_PadSubtract		= 333,
			Key_PadAdd			= 334,
			Key_PadEnter		= 335,
			Key_PadEqual		= 336,
			Key_LeftShift		= 340,
			Key_LeftCtrl		= 341,
			Key_LeftAlt			= 342,
			Key_LeftSuper		= 343,
			Key_RightShift		= 344,
			Key_RightCtrl		= 345,
			Key_RightAlt		= 346,
			Key_RightSuper		= 347,
			Key_Menu			= 348,
			Key_MAX				= 512
		};

		enum KeyMods_
		{
			KeyMods_None	= 0,
			KeyMods_Ctrl	= 1 << 0,
			KeyMods_Shift	= 1 << 1,
			KeyMods_Alt		= 1 << 2,
			KeyMods_Super	= 1 << 3
		};

		enum Joy_
		{
			Joy_0	= 0,
			Joy_1	= 1,
			Joy_2	= 2,
			Joy_3	= 3,
			Joy_4	= 4,
			Joy_5	= 5,
			Joy_6	= 6,
			Joy_7	= 7,
			Joy_8	= 8,
			Joy_9	= 9,
			Joy_10	= 10,
			Joy_11	= 11,
			Joy_12	= 12,
			Joy_13	= 13,
			Joy_14	= 14,
			Joy_15	= 15,
			Joy_MAX
		};

		enum JoyButton_
		{
			JoyButton_A				= 0,
			JoyButton_B				= 1,
			JoyButton_X				= 2,
			JoyButton_Y				= 3,
			JoyButton_Left_Bumper	= 4,
			JoyButton_Right_Bumper	= 5,
			JoyButton_Back			= 6,
			JoyButton_Start			= 7,
			JoyButton_Guide			= 8,
			JoyButton_Left_Thumb	= 9,
			JoyButton_Right_Thumb	= 10,
			JoyButton_DPAD_Up		= 11,
			JoyButton_DPAD_Right	= 12,
			JoyButton_DPAD_Down		= 13,
			JoyButton_DPAD_Left		= 14,
			JoyButton_MAX,

			JoyButton_Cross = JoyButton_A,
			JoyButton_Circle = JoyButton_B,
			JoyButton_Square = JoyButton_X,
			JoyButton_Triangle = JoyButton_Y,
		};

		enum JoyAxis_
		{
			JoyAxis_Left_X			= 0,
			JoyAxis_Left_Y			= 1,
			JoyAxis_Right_X			= 2,
			JoyAxis_Right_Y			= 3,
			JoyAxis_Left_Trigger	= 4,
			JoyAxis_Right_Trigger	= 5,
			JoyAxis_MAX
		};

		enum NavInput_
		{
			NavInput_Activate,
			NavInput_Cancel,
			NavInput_Input,
			NavInput_Menu,
			NavInput_DpadLeft,
			NavInput_DpadRight,
			NavInput_DpadUp,
			NavInput_DpadDown,
			NavInput_LStickLeft,
			NavInput_LStickRight,
			NavInput_LStickUp,
			NavInput_LStickDown,
			NavInput_FocusPrev,
			NavInput_FocusNext,
			NavInput_MAX
		};

	public:
		static constexpr bool is_valid_mouse_position(Vec2 const * value = nullptr) noexcept
		{
			constexpr f32 mouse_invalid{ -256000.f };
			Vec2 const p{ value ? *value : __singleton->m_mouse_position };
			return p[0] >= mouse_invalid && p[1] >= mouse_invalid;
		}

	protected:
		friend class DisplayServer;

		/* DISPLAY SERVER FUNCTIONS */
		static struct Funcs
		{
			MouseMode_(*get_mouse_mode)() = 0;
			void (*set_mouse_mode)(MouseMode_) = 0;
			void (*set_mouse_position)(Vec2 const &) = 0;

			CursorShape_(*get_cursor_shape)() = 0;
			void (*set_cursor_shape)(CursorShape_) = 0;
			void (*set_custom_cursor_image)(RES const &, CursorShape_, Vec2 const &) = 0;
		}
		m_funcs;

		/* MOUSE */
		Vec2	m_mouse_position{};
		Vec2	m_mouse_position_prev{};
		Vec2	m_last_valid_mouse_position{};
		Vec2	m_mouse_delta{};
		Vec2	m_mouse_wheel{};
		f32		m_double_click_max_distance{ 6.f };
		Action_	m_mouse_button[MouseButton_MAX]{};
		f32		m_mouse_button_time[MouseButton_MAX]{};
		f32		m_mouse_button_time_prev[MouseButton_MAX]{};
		u8		m_mouse_click[MouseButton_MAX]{}; // 0:released, 1:clicked, 2:double_clicked, 3:new_double_click
		f32		m_mouse_click_time[MouseButton_MAX]{};
		Vec2	m_mouse_click_position[MouseButton_MAX]{};
		Vec2	m_mouse_released_position[MouseButton_MAX]{};
		f32		m_mouse_double_click_time[MouseButton_MAX]{};
		Vec2	m_mouse_drag_abs[MouseButton_MAX]{};
		f32		m_mouse_drag_sqr[MouseButton_MAX]{};

		/* KEYBOARD */
		bool	m_is_shift : 1, m_is_ctrl : 1, m_is_alt : 1, m_is_super : 1;
		i32		m_key_mods{ KeyMods_None };
		Action_	m_key[Key_MAX]{};
		f32		m_key_time[Key_MAX]{};
		f32		m_key_time_prev[Key_MAX]{};

		/* JOYSTICKS */
		bool	m_joy_known[Joy_MAX]{};
		String	m_joy_name[Joy_MAX]{};
		String	m_joy_guid[Joy_MAX]{};
		f32		m_joy_axes[Joy_MAX][JoyAxis_MAX]{};
		Action_	m_joy_button[Joy_MAX][JoyButton_MAX]{};
		f32		m_joy_button_time[Joy_MAX][JoyButton_MAX]{};
		f32		m_joy_button_time_prev[Joy_MAX][JoyButton_MAX]{};

		/* NAVIGATION */
		bool	m_nav_enabled{};
		Action_	m_nav_input[NavInput_MAX]{};
		f32		m_nav_input_time[NavInput_MAX]{};
		f32		m_nav_input_time_prev[NavInput_MAX]{};

	public:
		Action_ get_key(i32 key) const;
		Action_ get_mouse_button(i32 button) const;
		Action_ get_joy_button(i32 device, i32 button) const;
		Action_ get_nav_input(i32 nav_input) const;

		void set_key(i32 key, Action_ action);
		void set_mouse_button(i32 button, Action_ action);
		void set_joy_button(i32 device, i32 button, Action_ action);
		void set_nav_input(i32 nav_input, Action_ action);

		bool is_key(i32 key, i32 action) const;
		bool is_mouse_button(i32 button, i32 action) const;
		bool is_joy_button(i32 device, i32 button, i32 action) const;
		bool is_nav_input(i32 nav_input, i32 action) const;

		bool is_key_released(i32 key) const noexcept { return is_key(key, Action_Release); }
		bool is_mouse_button_released(i32 button) const noexcept { return is_mouse_button(button, Action_Release); }
		bool is_joy_button_released(i32 device, i32 button) const noexcept { return is_joy_button(device, button, Action_Release); }
		bool is_nav_input_released(i32 nav_input) const noexcept { return is_nav_input(nav_input, Action_Release); }

		bool is_key_pressed(i32 key) const noexcept { return is_key(key, Action_Press); }
		bool is_mouse_button_pressed(i32 button) const noexcept { return is_mouse_button(button, Action_Press); }
		bool is_joy_button_pressed(i32 device, i32 button) const noexcept { return is_joy_button(device, button, Action_Press); }
		bool is_nav_input_pressed(i32 nav_input) const noexcept { return is_nav_input(nav_input, Action_Press); }

		bool is_key_repeat(i32 key) const noexcept { return is_key(key, Action_Repeat); }
		bool is_mouse_button_repeat(i32 button) const noexcept { return is_mouse_button(button, Action_Repeat); }
		bool is_joy_button_repeat(i32 device, i32 button) const noexcept { return is_joy_button(device, button, Action_Repeat); }
		bool is_nav_input_repeat(i32 nav_input) const noexcept { return is_nav_input(nav_input, Action_Repeat); }
		
		bool is_key_down(i32 key) const;
		bool is_mouse_button_down(i32 button) const;
		bool is_joy_button_down(i32 device, i32 button) const;
		bool is_nav_input_down(i32 nav_input) const;

		bool is_any_key_down() const;
		bool is_any_mouse_button_down() const;
		bool is_any_joy_button_down(i32 device) const;
		bool is_any_nav_input_down() const;

		f32 get_key_down_time(i32 key) const;
		f32 get_mouse_button_down_time(i32 button) const;
		f32 get_joy_button_down_time(i32 device, i32 button) const;
		f32 get_nav_input_down_time(i32 nav_input) const;

		bool is_key_past_threshold(i32 key, f32 threshold) const;
		bool is_mouse_button_past_threshold(i32 button, f32 threshold) const;
		bool is_joy_button_past_threshold(i32 device, i32 button, f32 threshold) const;
		bool is_nav_input_past_threshold(i32 nav_input, f32 threshold) const;

	public:
		Vector<Joy_> get_connected_joys() const;
		Joy_ get_unused_joy_id();
		bool is_joy_known(i32 device) const;
		String get_joy_name(i32 device) const;
		String get_joy_guid(i32 device) const;
		f32 get_joy_axis(i32 device, i32 axis) const;
		void set_joy_axis(i32 device, i32 axis, f32 value);
		void joy_connection_changed(i32 device, bool connected, String const & name = {}, String const & guid = {});
		bool parse_mapping(String const & mapping);

	public:
		MouseMode_ get_mouse_mode() const;
		void set_mouse_mode(MouseMode_ mode);

		Vec2 get_mouse_position() const;
		void set_mouse_position(Vec2 const & position);

		Vec2 get_last_mouse_position() const;
		Vec2 get_mouse_delta() const;

		bool is_mouse_dragging(i32 button, f32 threshold = -1.f) const;
		bool is_mouse_clicked(i32 button) const;
		bool is_mouse_double_clicked(i32 button) const;

		Vec2 get_mouse_wheel() const;
		void set_mouse_wheel(Vec2 const & wheel);

		CursorShape_ get_cursor_shape() const;
		void set_cursor_shape(CursorShape_ shape);
		void set_custom_cursor_image(RES const & cursor, CursorShape_ shape = CursorShape_Arrow, Vec2 const & hotspot = {});

	public:
		void iteration(Duration const delta_time);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_WRAPPER(Input, get_input);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_INPUT_HPP_
