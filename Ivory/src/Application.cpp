#include "pch.h"
#include "Application.h"
#include "Events/WindowEvent.h"
#include "Events/KeyEvent.h"
#include <glad/glad.h>
#include "Input.h"

namespace Ivory {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application* Application::s_instance = nullptr;

	Application::Application() {
		// Only one application can run at a time
		IV_CORE_ASSERT(!s_instance, "Application already exists");
		s_instance = this;
		// Create window on application start
		m_window = std::unique_ptr<Window>(Window::create());
		m_window->set_event_callback(BIND_EVENT_FN(on_event));

		m_imgui_layer = std::make_shared<ImGuiLayer>();
		push_overlay(m_imgui_layer);

		glGenVertexArrays(1, &m_vertex_array);
		glBindVertexArray(m_vertex_array);

		glGenBuffers(1, &m_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

		float vertices[3 * 3] = { 
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &m_index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);

		unsigned int indeces[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);
	}

	void Application::on_event(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(on_window_close));

		// Traverse layer stack in reverse and let a layer handle it.
		// If the event is handled by the layer then stop going deeper, else let the next layer handle the event.
		for (auto it = m_layer_stack.end(); it != m_layer_stack.begin(); ) {
			(*--it)->on_event(e);
			if (e.is_handled())
				break;
		}
	}

	void Application::run() {
		while (m_running) {
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(m_vertex_array);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			// Update every layer
			for (shared_ptr<Layer>& layer : m_layer_stack)
				layer->on_update();

			// Start imgui frame
			m_imgui_layer->begin();

			for (shared_ptr<Layer>& layer : m_layer_stack)
				layer->on_imgui_render();
			m_imgui_layer->end();

			m_window->on_update();
		}
	}

	bool Application::on_window_close(WindowCloseEvent& e) {
		m_running = false;
		return true;
	}

	void Application::push_layer(shared_ptr<Layer> layer) {
		m_layer_stack.push_layer(layer);
		layer->on_attach();
	}

	void Application::push_overlay(shared_ptr<Layer> overlay) {
		m_layer_stack.push_layer(overlay);
		overlay->on_attach();
	}
}