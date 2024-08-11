#pragma once
#include "Core/Input.h"

namespace Ivory {
	class WindowsInput : public Input {
	protected:
		virtual bool is_key_pressed_impl(int keycode) override;
		virtual bool is_mouse_button_pressed_impl(int keycode) override;
		virtual Vector2 mouse_pos_impl() override;
	};
}