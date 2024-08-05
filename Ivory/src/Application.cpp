#include "pch.h"
#include "Application.h"
#include "Events/WindowEvent.h"
#include "Events/KeyEvent.h"
#include "Input.h"
#include "Rendering/Renderer.h"
#include "Platform/Platform.h"

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
			float time = Platform::get_time();
			Timestep timestep = time - m_last_frame_time;
			m_last_frame_time = time;

			// Update every layer
			for (shared_ptr<Layer>& layer : m_layer_stack)
				layer->on_update(timestep);

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