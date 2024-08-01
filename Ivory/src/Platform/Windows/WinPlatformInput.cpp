#include "pch.h"
#include "WinPlatformInput.h"

#include "Application.h"
#include "GLFW/glfw3.h"

namespace Ivory {
	Input* Input::s_instance = new WindowsInput();

	bool WindowsInput::is_mouse_button_pressed_impl(int keycode) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().get_window().get_native());
		int state = glfwGetMouseButton(window, keycode);
		return state == GLFW_PRESS;
	}

	bool WindowsInput::is_key_pressed_impl(int keycode) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().get_window().get_native());
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	Vector2 WindowsInput::mouse_pos_impl() {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().get_window().get_native());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return Vector2(x, y);
	}
}