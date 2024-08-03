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

		/*m_vertex_array.reset(VertexArray::create_array());

		float vertices[3 * 7] = { 
			-0.25f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			 0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,   0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		};

		m_vertex_buffer.reset(VertexBuffer::create_buffer(vertices, sizeof(vertices)));

		

		m_vertex_array->add_vertex_buffer(m_vertex_buffer);

		unsigned int indeces[3] = { 0, 1, 2 };
		m_index_buffer.reset(IndexBuffer::create_buffer(indeces, 3));
		m_vertex_array->set_index_buffer(m_index_buffer);*/

		m_square_VA.reset(VertexArray::create_array());

		float vertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,   0.0f, 0.0f,
			-0.5f,   0.0f, 0.0f
		};

		std::shared_ptr<VertexBuffer> square_VB = std::shared_ptr<VertexBuffer>(VertexBuffer::create_buffer(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ShaderDataType::Vector3, "a_Position"},
		};
		square_VB->set_layout(layout);
		m_square_VA->add_vertex_buffer(square_VB);

		unsigned int indeces[6] = { 0, 1, 2, 2, 3, 0 };
		//m_index_buffer.reset(IndexBuffer::create_buffer(indeces, 3));
		std::shared_ptr<IndexBuffer> square_IB(IndexBuffer::create_buffer(indeces, 6));

		m_square_VA->set_index_buffer(square_IB);

		std::string vertex_src = R"(
			#version 330

			layout(location = 0) in vec3 a_Position;

			void main() {
				gl_Position = vec4(a_Position, 1.0);
			}

		)";

		std::string fragment_src = R"(
			#version 330

			layout(location = 0) out vec4 color;

			void main() {
				color = vec4(0.8, 0.6, 0.3, 1.0);
			}

		)";

		m_shader = std::make_unique<Shader>(vertex_src, fragment_src);
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

			m_shader->bind();
			m_square_VA->bind();
			glDrawElements(GL_TRIANGLES, m_square_VA->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);

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