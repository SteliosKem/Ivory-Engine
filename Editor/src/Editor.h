#pragma once
#include <IvoryEngine.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

const std::string icon_path = "C:/Projects/Ivory-Engine/Assets/IVlogo.png";

// For testing layers
class ExampleLayer : public Ivory::Layer {
public:
	ExampleLayer() : Layer("Example"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f) {
		m_solid_square_VA.reset(Ivory::VertexArray::create_array());

		float solid_vertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<Ivory::VertexBuffer> solid_square_VB = std::shared_ptr<Ivory::VertexBuffer>
			(Ivory::VertexBuffer::create_buffer(solid_vertices, sizeof(solid_vertices)));

		Ivory::BufferLayout solid_layout = {
			{Ivory::ShaderDataType::Vector3, "a_position"},
		};
		solid_square_VB->set_layout(solid_layout);
		m_solid_square_VA->add_vertex_buffer(solid_square_VB);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Ivory::IndexBuffer> solid_square_IB(Ivory::IndexBuffer::create_buffer(indices, sizeof(indices) / sizeof(uint32_t)));

		m_solid_square_VA->set_index_buffer(solid_square_IB);

		std::string vertex_src = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;

			uniform mat4 u_view_projection;
			uniform mat4 u_transform;

			void main() {
				gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
			}

		)";

		std::string fragment_src = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			uniform vec3 u_color;

			void main() {
				//color = vec4(0.8, 0.6, 0.3, 1.0);
				color = vec4(u_color, 1.0f);
			}

		)";

		m_square_VA.reset(Ivory::VertexArray::create_array());

		float vertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.1f, 1.1f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Ivory::VertexBuffer> square_VB = std::shared_ptr<Ivory::VertexBuffer>
			(Ivory::VertexBuffer::create_buffer(vertices, sizeof(vertices)));

		Ivory::BufferLayout layout = {
			{Ivory::ShaderDataType::Vector3, "a_position"},
			{Ivory::ShaderDataType::Vector2, "a_texture_coord"},
		};
		square_VB->set_layout(layout);
		m_square_VA->add_vertex_buffer(square_VB);

		//uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Ivory::IndexBuffer> square_IB(Ivory::IndexBuffer::create_buffer(indices, sizeof(indices) / sizeof(uint32_t)));

		m_square_VA->set_index_buffer(square_IB);

		std::string texture_vertex_src = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec2 a_texture_coord;

			uniform mat4 u_view_projection;
			uniform mat4 u_transform;

			out vec2 v_texture_coord;

			void main() {
				v_texture_coord = a_texture_coord;
				gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
			}

		)";

		std::string texture_fragment_src = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			
			in vec2 v_texture_coord;
			uniform sampler2D u_texture;

			void main() {
				
				//color = vec4(v_texture_coord, 0.0f, 1.0f);
				color = texture(u_texture, v_texture_coord);
			}

		)";

		m_shader = std::unique_ptr<Ivory::Shader>(Ivory::Shader::create(vertex_src, fragment_src));
		m_texture_shader = std::unique_ptr<Ivory::Shader>(Ivory::Shader::create(texture_vertex_src, texture_fragment_src));
		m_color = glm::vec3(0.5f, 0.2f, 0.1f);

		m_texture = Ivory::Texture2D::create("C:/Projects/Ivory-Engine/Assets/asd.png");

		std::dynamic_pointer_cast<Ivory::OpenGLShader>(m_texture_shader)->bind();
		std::dynamic_pointer_cast<Ivory::OpenGLShader>(m_texture_shader)->upload_uniform_int("u_texture", 0);
	}

	void on_update(Ivory::Timestep delta_time) override {
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

		m_texture->bind();
		Ivory::Renderer::submit(m_square_VA, m_texture_shader, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

		std::dynamic_pointer_cast<Ivory::OpenGLShader>(m_shader)->bind();
		std::dynamic_pointer_cast<Ivory::OpenGLShader>(m_shader)->upload_uniform_vec3("u_color", m_color);

		Ivory::Renderer::submit(m_solid_square_VA, m_shader, glm::translate(glm::mat4(1.0f), glm::vec3(1.1f, 0.0f, 0.0f)));
		

		Ivory::Renderer::end_scene();

		m_last_mouse_pos = m_mouse_pos;
	}

	void on_imgui_render() override {
		ImGui::Begin("Color");
		ImGui::ColorPicker3("Color", glm::value_ptr(m_color));
		ImGui::End();

	}

	bool on_scroll(Ivory::MouseScrollEvent& e) {
		return false;
	}

	void on_event(Ivory::Event& e) override;
private:
	std::shared_ptr<Ivory::Shader> m_shader;
	std::shared_ptr<Ivory::Shader> m_texture_shader;
	std::shared_ptr<Ivory::VertexArray> m_vertex_array;

	std::shared_ptr<Ivory::VertexArray> m_square_VA;
	std::shared_ptr<Ivory::VertexArray> m_solid_square_VA;

	std::shared_ptr<Ivory::Texture2D> m_texture;

	glm::vec3 m_camera_pos{0.0f};
	glm::vec2 m_mouse_pos{0.0f};

	glm::vec2 m_last_mouse_pos{0.0f};
	glm::vec3 m_color;

	Ivory::OrthographicCamera m_camera;
};

class Editor : public Ivory::Application {
public:
	Editor() {
		//Ivory::Application::get_window().set_vsync(false);
		push_layer(std::make_shared<ExampleLayer>());
		get_window().set_image(icon_path);
	}
	~Editor() {}
};