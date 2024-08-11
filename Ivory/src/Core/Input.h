#pragma once
#include "IMath.h"

namespace Ivory {
	class IVORY_API Input {
	public:
		static bool is_key_pressed(int keycode) {
			return s_instance->is_key_pressed_impl(keycode);
		}

		static bool is_mouse_button_pressed(int keycode) {
			return s_instance->is_mouse_button_pressed_impl(keycode);
		}

		static Vector2 mouse_pos() {
			return s_instance->mouse_pos_impl();
		}
	protected:
		virtual bool is_key_pressed_impl(int keycode) = 0;
		virtual bool is_mouse_button_pressed_impl(int keycode) = 0;
		virtual Vector2 mouse_pos_impl() = 0;
	private:
		static Input* s_instance;
	};
}