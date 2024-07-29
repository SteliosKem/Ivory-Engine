#include "pch.h"
#include "Application.h"
#include "WindowEvent.h"
#include "KeyEvent.h"
#include <GLFW/glfw3.h>

namespace Ivory {
	Application::Application() {
		m_window = std::unique_ptr<Window>(Window::create());
	}

	void Application::run() {
		while (m_running) {
			glClearColor(1, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_window->on_update();
		}
	}
}