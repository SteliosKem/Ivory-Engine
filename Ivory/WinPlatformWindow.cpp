#include "pch.h"
#include "WinPlatformWindow.h"

namespace Ivory {
	static bool s_GLFWInitialized = false;

	Window* Window::create(const WindowProps& props) { return new WinPlatformWindow(props); }
	WinPlatformWindow::WinPlatformWindow(const WindowProps& props) { init(props); }
	WinPlatformWindow::~WinPlatformWindow() { shutdown(); }

	void WinPlatformWindow::init(const WindowProps& props) {
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;

		IV_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			IV_CORE_ASSERT(success, "Could not initialize GLFW");

			s_GLFWInitialized = true;
		}

		m_window = glfwCreateWindow((int)props.width, (int)props.height, m_data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_data);
		set_vsync(true);
	}

	void WinPlatformWindow::shutdown() {
		glfwDestroyWindow(m_window);
	}

	void WinPlatformWindow::on_update() {
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void WinPlatformWindow::set_vsync(bool enabled) {
		enabled ? glfwSwapInterval(1) : glfwSwapInterval(0);
		m_data.vsync = enabled;
	}

	bool WinPlatformWindow::get_vsync() const { return m_data.vsync; }
}