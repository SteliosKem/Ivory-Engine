#include "pch.h"
#include "Application.h"
#include "WindowEvent.h"
#include "KeyEvent.h"
#include <glad/glad.h>

namespace Ivory {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application::Application() {
		m_window = std::unique_ptr<Window>(Window::create());
		m_window->set_event_callback(BIND_EVENT_FN(on_event));
	}

	void Application::on_event(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(on_window_close));

		for (auto it = m_layer_stack.end(); it != m_layer_stack.begin(); ) {
			(*--it)->on_event(e);
			if (e.is_handled())
				break;
		}
	}

	void Application::run() {
		while (m_running) {
			glClearColor(1, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_layer_stack)
				layer->on_update();

			m_window->on_update();
		}
	}

	bool Application::on_window_close(WindowCloseEvent& e) {
		m_running = false;
		return true;
	}

	void Application::push_layer(Layer* layer) {
		m_layer_stack.push_layer(layer);
	}

	void Application::push_overlay(Layer* overlay) {
		m_layer_stack.push_layer(overlay);
	}
}