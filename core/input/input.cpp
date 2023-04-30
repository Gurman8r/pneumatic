#include <core/input/input.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// internal click state
	enum : u8 { Click_Released = 0, Click_Clicked = 1, Click_DoubleClicked = 2, Click_IsNewDoubleClick = 3, };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Input * Input::__singleton{};

	Input::Funcs Input::m_funcs{};

	EMBED_CLASS(Input, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Input::Action_ Input::get_key(i32 key) const
	{
		VERIFY_RANGE(key, -1, Key_MAX);
		return m_key[key];
	}

	Input::Action_ Input::get_mouse_button(i32 button) const
	{
		VERIFY_RANGE(button, -1, MouseButton_MAX);
		return m_mouse_button[button];
	}

	Input::Action_ Input::get_joy_button(i32 device, i32 button) const
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		VERIFY_RANGE(button, -1, JoyButton_MAX);
		return m_joy_button[device][button];
	}

	Input::Action_ Input::get_nav_input(i32 nav_input) const
	{
		VERIFY_RANGE(nav_input, -1, NavInput_MAX);
		return m_nav_input[nav_input];
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Input::set_key(i32 key, Action_ action)
	{
		VERIFY_RANGE(key, -1, Key_MAX);
		m_key[key] = action;

		m_is_ctrl = m_key[Input::Key_LeftCtrl] || m_key[Input::Key_RightCtrl];
		m_is_shift = m_key[Input::Key_LeftShift] || m_key[Input::Key_RightShift];
		m_is_alt = m_key[Input::Key_LeftAlt] || m_key[Input::Key_RightAlt];
		m_is_super = m_key[Input::Key_LeftSuper] || m_key[Input::Key_RightSuper];

		flag_write(m_key_mods, KeyMods_Ctrl, m_is_ctrl);
		flag_write(m_key_mods, KeyMods_Shift, m_is_shift);
		flag_write(m_key_mods, KeyMods_Alt, m_is_alt);
		flag_write(m_key_mods, KeyMods_Super, m_is_super);
	}

	void Input::set_mouse_button(i32 button, Action_ action)
	{
		VERIFY_RANGE(button, -1, MouseButton_MAX);
		m_mouse_button[button] = action;
	}

	void Input::set_joy_button(i32 device, i32 button, Action_ action)
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		VERIFY_RANGE(button, -1, JoyButton_MAX);
		m_joy_button[device][button] = action;
	}

	void Input::set_nav_input(i32 nav_input, Action_ action)
	{
		VERIFY_RANGE(nav_input, -1, NavInput_MAX);
		m_nav_input[nav_input] = action;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Input::is_key(i32 key, i32 action) const
	{
		VERIFY_RANGE(key, -1, Key_MAX);
		return (m_key[key] == action);
	}

	bool Input::is_mouse_button(i32 button, i32 action) const
	{
		VERIFY_RANGE(button, -1, MouseButton_MAX);
		return (m_mouse_button[button] == action);
	}

	bool Input::is_joy_button(i32 device, i32 button, i32 action) const
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		VERIFY_RANGE(button, -1, JoyButton_MAX);
		return (m_joy_button[device][button] == action);
	}

	bool Input::is_nav_input(i32 nav_input, i32 action) const
	{
		VERIFY_RANGE(nav_input, -1, NavInput_MAX);
		return (m_nav_input[nav_input] == action);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Input::is_key_down(i32 key) const
	{
		VERIFY_RANGE(key, -1, Key_MAX);
		return (m_key[key] != Action_Release);
	}

	bool Input::is_mouse_button_down(i32 button) const
	{
		VERIFY_RANGE(button, -1, MouseButton_MAX);
		return (m_mouse_button[button] != Action_Release);
	}

	bool Input::is_joy_button_down(i32 device, i32 button) const
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		VERIFY_RANGE(button, -1, JoyButton_MAX);
		return (m_joy_button[device][button] != Action_Release);
	}

	bool Input::is_nav_input_down(i32 nav_input) const
	{
		VERIFY_RANGE(nav_input, -1, NavInput_MAX);
		return (m_nav_input[nav_input] != Action_Release);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Input::is_any_key_down() const
	{
		for (i32 i = 0; i < Key_MAX; ++i)
		{
			if (m_key[i] != Action_Release)
			{
				return true;
			}
		}
		return false;
	}

	bool Input::is_any_mouse_button_down() const
	{
		for (i32 i = 0; i < MouseButton_MAX; ++i)
		{
			if (m_mouse_button[i] != Action_Release)
			{
				return true;
			}
		}
		return false;
	}

	bool Input::is_any_joy_button_down(i32 device) const
	{
		if (is_joy_known(device))
		{
			for (i32 i = 0; i < JoyButton_MAX; ++i)
			{
				if (m_joy_button[device][i] != Action_Release)
				{
					return true;
				}
			}
		}
		return false;
	}

	bool Input::is_any_nav_input_down() const
	{
		for (i32 i = 0; i < NavInput_MAX; ++i)
		{
			if (m_nav_input[i] != Action_Release)
			{
				return true;
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	f32 Input::get_key_down_time(i32 key) const
	{
		VERIFY_RANGE(key, -1, Key_MAX);
		return m_key_time[key];
	}

	f32 Input::get_mouse_button_down_time(i32 button) const
	{
		VERIFY_RANGE(button, -1, MouseButton_MAX);
		return m_mouse_button_time[button];
	}

	f32 Input::get_joy_button_down_time(i32 device, i32 button) const
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		VERIFY_RANGE(button, -1, JoyButton_MAX);
		return m_joy_button_time[device][button];
	}

	f32 Input::get_nav_input_down_time(i32 nav_input) const
	{
		VERIFY_RANGE(nav_input, -1, NavInput_MAX);
		return m_nav_input_time[nav_input];
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Input::is_key_past_threshold(i32 key, f32 threshold) const
	{
		VERIFY_RANGE(key, -1, Key_MAX);
		if (threshold < 0.f) { threshold = 0.1f; }
		return m_key_time[key] >= threshold  * threshold;
	}

	bool Input::is_mouse_button_past_threshold(i32 button, f32 threshold) const
	{
		VERIFY_RANGE(button, -1, MouseButton_MAX);
		if (threshold < 0.f) { threshold = 0.1f; }
		return m_mouse_button_time[button] >= threshold * threshold;
	}

	bool Input::is_joy_button_past_threshold(i32 device, i32 button, f32 threshold) const
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		VERIFY_RANGE(button, -1, JoyButton_MAX);
		if (threshold < 0.f) { threshold = 0.1f; }
		return m_joy_button_time[device][button] >= threshold * threshold;
	}

	bool Input::is_nav_input_past_threshold(i32 nav_input, f32 threshold) const
	{
		VERIFY_RANGE(nav_input, -1, NavInput_MAX);
		if (threshold < 0.f) { threshold = 0.1f; }
		return m_nav_input_time[nav_input] >= threshold * threshold;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Vector<Input::Joy_> Input::get_connected_joys() const
	{
		Vector<Joy_> temp{};
		for (i32 device = 0; device < Joy_MAX; ++device)
		{
			if (m_joy_known[device])
			{
				temp.push_back((Joy_)device);
			}
		}
		return temp;
	}

	Input::Joy_ Input::get_unused_joy_id()
	{
		return { /* TODO */ };
	}

	bool Input::is_joy_known(i32 device) const
	{
		return ((u32)device < (u32)Joy_MAX) && m_joy_known[device];
	}

	String Input::get_joy_name(i32 device) const
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		return m_joy_name[device];
	}

	String Input::get_joy_guid(i32 device) const
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		return m_joy_guid[device];
	}

	f32 Input::get_joy_axis(i32 device, i32 axis) const
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		VERIFY_RANGE(axis, -1, JoyAxis_MAX);
		return m_joy_axes[device][axis];
	}

	void Input::set_joy_axis(i32 device, i32 axis, f32 value)
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		VERIFY_RANGE(axis, -1, JoyAxis_MAX);
		m_joy_axes[device][axis] = value;
	}

	void Input::joy_connection_changed(i32 device, bool connected, String const & name, String const & guid)
	{
		VERIFY_RANGE(device, -1, Joy_MAX);
		m_joy_known[device] = connected;

		if (connected)
		{
			m_joy_name[device] = name;
			m_joy_guid[device] = guid ? guid : name;
		}
		else
		{
			m_joy_name[device] = ""_s;
			m_joy_guid[device] = ""_s;

			for (i32 i = 0; i < JoyButton_MAX; ++i) {
				m_joy_button[device][i] = Action_Release;
				m_joy_button_time[device][i] = 0.f;
			}

			for (i32 i = 0; i < JoyAxis_MAX; ++i) {
				m_joy_axes[device][i] = 0.f;
			}
		}
	}

	bool Input::parse_mapping(String const & mapping)
	{
		// TODO
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Input::MouseMode_ Input::get_mouse_mode() const
	{
		ASSERT(m_funcs.get_mouse_mode);
		return m_funcs.get_mouse_mode();
	}

	void Input::set_mouse_mode(MouseMode_ mode)
	{
		ASSERT(m_funcs.set_mouse_mode);
		m_funcs.set_mouse_mode(mode);
	}

	Vec2 Input::get_mouse_position() const
	{
		return m_mouse_position;
	}

	void Input::set_mouse_position(Vec2 const & position)
	{
		m_mouse_position = position;

		if (m_funcs.set_mouse_position) {
			m_funcs.set_mouse_position(position);
		}
	}

	Vec2 Input::get_last_mouse_position() const
	{
		return m_mouse_position_prev;
	}

	Vec2 Input::get_mouse_delta() const
	{
		return m_mouse_delta;
	}

	bool Input::is_mouse_dragging(i32 button, f32 threshold) const
	{
		VERIFY_RANGE(button, -1, MouseButton_MAX);
		return m_mouse_button[button]
			&& is_mouse_button_past_threshold(button, threshold);
	}

	bool Input::is_mouse_clicked(i32 button) const
	{
		return bit_read(m_mouse_click[button], Click_Clicked);
	}

	bool Input::is_mouse_double_clicked(i32 button) const
	{
		return bit_read(m_mouse_click[button], Click_DoubleClicked);
	}

	Vec2 Input::get_mouse_wheel() const
	{
		return m_mouse_wheel;
	}

	void Input::set_mouse_wheel(Vec2 const & wheel)
	{
		m_mouse_wheel = wheel;
	}

	Input::CursorShape_ Input::get_cursor_shape() const
	{
		ASSERT(m_funcs.get_cursor_shape);
		return m_funcs.get_cursor_shape();
	}

	void Input::set_cursor_shape(CursorShape_ shape)
	{
		VERIFY_RANGE(shape, -1, CursorShape_MAX);
		ASSERT(m_funcs.set_cursor_shape);
		m_funcs.set_cursor_shape(shape);
	}

	void Input::set_custom_cursor_image(RES const & cursor, CursorShape_ shape, Vec2 const & hotspot)
	{
		VERIFY_RANGE(shape, -1, CursorShape_MAX);
		ASSERT(m_funcs.set_custom_cursor_image);
		m_funcs.set_custom_cursor_image(cursor, shape, hotspot);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Input::iteration(Duration const delta_time)
	{
		/* UPDATE MOUSE */

		// round mouse position
		if (is_valid_mouse_position(&m_mouse_position)) {
			m_mouse_position = m_last_valid_mouse_position = { std::floorf(m_mouse_position[0]), std::floorf(m_mouse_position[1]) };
		}

		// cancel out movement in delta if mouse just appeared or disappeared
		if (is_valid_mouse_position(&m_mouse_position) && is_valid_mouse_position(&m_mouse_position_prev)) {
			m_mouse_delta = m_mouse_position - m_mouse_position_prev;
		}
		else {
			m_mouse_delta = {};
		}

		if (m_mouse_delta[0] != 0.f && m_mouse_delta[1] != 0.f) {
			m_nav_enabled = false;
		}

		m_mouse_position_prev = m_mouse_position;

		for (i32 i = 0; i < MouseButton_MAX; ++i)
		{
			m_mouse_button_time_prev[i] = m_mouse_button_time[i];
			m_mouse_button_time[i] = m_mouse_button[i] ? (m_mouse_button_time[i] < 0.f ? 0.f : m_mouse_button_time[i] + delta_time) : -1.f;

			bit_write(m_mouse_click[i], Click_Released, !m_mouse_button[i] && m_mouse_button_time[i] >= 0.f);
			bit_write(m_mouse_click[i], Click_Clicked, m_mouse_button[i] && m_mouse_button_time[i] < 0.f);
			bit_clear(m_mouse_click[i], Click_DoubleClicked);

			if (bit_read(m_mouse_click[i], Click_Clicked))
			{
				if (delta_time - m_mouse_click_time[i] > m_mouse_double_click_time[i])
				{
					Vec2 const click_delta{ is_valid_mouse_position(&m_mouse_position) ? (m_mouse_position - m_mouse_click_position[i]) : Vec2{} };
					f32 const click_dist{ length(click_delta) };
					if (click_dist < m_double_click_max_distance * m_double_click_max_distance) {
						bit_set(m_mouse_click[i], Click_DoubleClicked);
					}
					m_mouse_click_time[i] = -m_mouse_double_click_time[i] * 2.f; // mark as "old enough" so the third click isn't turned into a double-click
				}
				else
				{
					m_mouse_click_time[i] = delta_time;
				}

				m_mouse_click_position[i] = m_mouse_position;
				bit_write(m_mouse_click[i], Click_IsNewDoubleClick, bit_read(m_mouse_click[i], Click_DoubleClicked));
				m_mouse_drag_sqr[i] = 0.f;
				m_mouse_drag_abs[i] = {};
			}
			else if (m_mouse_button[i])
			{
				Vec2 const click_delta{ is_valid_mouse_position(&m_mouse_position) ? (m_mouse_position - m_mouse_click_position[i]) : Vec2{} };
				f32 const click_dist{ length(click_delta) };
				m_mouse_drag_sqr[i] = maximum(m_mouse_drag_sqr[i], click_dist);
				m_mouse_drag_abs[i][0] = maximum(m_mouse_drag_abs[i][0], click_delta[0] < 0.f ? -click_delta[0] : click_delta[0]);
				m_mouse_drag_abs[i][1] = maximum(m_mouse_drag_abs[i][1], click_delta[1] < 0.f ? -click_delta[1] : click_delta[1]);
			}

			if (m_mouse_button[i] != Action_Release && !bit_read(m_mouse_click[i], Click_Released)) {
				bit_clear(m_mouse_click[i], Click_IsNewDoubleClick);
			}

			if (bit_read(m_mouse_click[i], Click_Clicked)) {
				m_nav_enabled = false;
			}
		}

		/* UPDATE KEYBOARD */

		for (i32 i = 0; i < Key_MAX; ++i)
		{
			m_key_time_prev[i] = m_key_time[i];
			m_key_time[i] = m_key[i] ? (m_key_time[i] < 0.f ? 0.f : m_key_time[i] + delta_time) : -1.f;
		}

		/* UPDATE JOYSTICKS */

		for (i32 j = 0; j < Joy_MAX; ++j)
		{
			if (!m_joy_known[j]) {
				continue;
			}

			for (i32 i = 0; i < JoyButton_MAX; ++i)
			{
				m_joy_button_time_prev[j][i] = m_joy_button_time[j][i];
				m_joy_button_time[j][i] = m_joy_button[j][i] ? (m_joy_button_time[j][i] < 0.f ? 0.f : m_joy_button_time[j][i] + delta_time) : -1.f;
			}
		}

		/* UPDATE NAV INPUTS */

		for (i32 i = 0; i < NavInput_MAX; ++i)
		{
			m_nav_input_time_prev[i] = m_nav_input_time[i];
			m_nav_input_time[i] = m_nav_input[i] ? (m_nav_input_time[i] < 0.f ? 0.f : m_nav_input_time[i] + delta_time) : -1.f;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}