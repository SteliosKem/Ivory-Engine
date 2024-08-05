#pragma once
#include <IvoryEngine.h>

#include "imgui.h"

#include "glm/gtc/matrix_transform.hpp"

// For testing layers
class ExampleLayer : public Ivory::Layer {
public:
	ExampleLayer() : Layer("Example"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f) {
		m_square_VA.reset(Ivory::VertexArray::create_array());

		float vertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,   0.5f, 0.0f,
			-0.5f,   0.5f, 0.0f
		};

		std::shared_ptr<Ivory::VertexBuffer> square_VB = std::shared_ptr<Ivory::VertexBuffer>
			(Ivory::VertexBuffer::create_buffer(vertices, sizeof(vertices)));

		Ivory::BufferLayout layout = {
			{Ivory::ShaderDataType::Vector3, "a_Position"},
		};
		square_VB->set_layout(layout);
		m_square_VA->add_vertex_buffer(square_VB);

		unsigned int indeces[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Ivory::IndexBuffer> square_IB(Ivory::IndexBuffer::create_buffer(indeces, 6));

		m_square_VA->set_index_buffer(square_IB);

		std::string vertex_src = R"(
			#version 330

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_view_projection;
			uniform mat4 u_transform;

			void main() {
				gl_Position = u_view_projection * u_transform * vec4(a_Position, 1.0);
			}

		)";

		std::string fragment_src = R"(
			#version 330

			layout(location = 0) out vec4 color;

			void main() {
				color = vec4(0.8, 0.6, 0.3, 1.0);
			}

		)";

		m_shader = std::make_unique<Ivory::Shader>(vertex_src, fragment_src);
	}

	void on_update(Ivory::Timestep delta_time) override {
		IV_INFO(delta_time.get_ms());
		m_mouse_pos = glm::vec2(Ivory::Input::mouse_pos().x(), Ivory::Input::mouse_pos().y());
		if (Ivory::Input::is_mouse_button_pressed(2)) {
			//IV_INFO(m_mouse_pos.x - m_last_mouse_pos.x);
			//IV_INFO(m_mouse_pos.y - m_last_mouse_pos.y);
			m_camera_pos.x += -(m_mouse_pos.x - m_last_mouse_pos.x) * 0.12f * delta_time;
			m_camera_pos.y += (m_mouse_pos.y - m_last_mouse_pos.y) * 0.12f * delta_time;
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}
		else {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
		}

		Ivory::RenderCommand::set_clear_color({ 0.1f, 0.1f, 0.1f, 1 });
		Ivory::RenderCommand::clear();

		m_camera.set_position(m_camera_pos);
		Ivory::Renderer::begin_scene(m_camera);

		Ivory::Renderer::submit(m_square_VA, m_shader, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

		Ivory::Renderer::end_scene();

		m_last_mouse_pos = m_mouse_pos;
	}

	void on_imgui_render() override {
		

	}

	void on_event(Ivory::Event& e) override {

	}
private:
	std::shared_ptr<Ivory::Shader> m_shader;
	std::shared_ptr<Ivory::VertexArray> m_vertex_array;

	std::shared_ptr<Ivory::VertexArray> m_square_VA;

	glm::vec3 m_camera_pos{0.0f};
	glm::vec2 m_mouse_pos{0.0f};

	glm::vec2 m_last_mouse_pos{0.0f};

	Ivory::OrthographicCamera m_camera;
};

class Editor : public Ivory::Application {
public:
	Editor() {
		Ivory::Application::get_window().set_vsync(false);
		push_layer(std::make_shared<ExampleLayer>());
	}
	~Editor() {}
};