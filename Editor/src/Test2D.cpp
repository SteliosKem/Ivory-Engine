#include "Test2D.h"
#include "imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

Test2D::Test2D() : Layer("Test2D"), m_camera_controller(1280.0f / 720.f) {

}

void Test2D::on_attach() {
	m_vertex_array = Ivory::VertexArray::create_array();

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
	m_vertex_array->add_vertex_buffer(solid_square_VB);

	uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
	std::shared_ptr<Ivory::IndexBuffer> solid_square_IB(Ivory::IndexBuffer::create_buffer(indices, sizeof(indices) / sizeof(uint32_t)));

	m_vertex_array->set_index_buffer(solid_square_IB);

	m_shader = Ivory::Shader::create("C:/Projects/Ivory-Engine/Editor/Assets/shaders/flat.glsl");
	m_color = glm::vec3(0.5f, 0.2f, 0.1f);
}
void Test2D::on_detach() {}

void Test2D::on_update(Ivory::Timestep dt) {
	m_camera_controller.on_update(dt);

	if (Ivory::Input::is_mouse_button_pressed(2)) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}
	else {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
	}

	Ivory::RenderCommand::set_clear_color({ 0.1f, 0.1f, 0.1f, 1 });
	Ivory::RenderCommand::clear();

	Ivory::Renderer::begin_scene(m_camera_controller.get_camera());

	std::dynamic_pointer_cast<Ivory::OpenGLShader>(m_shader)->bind();
	std::dynamic_pointer_cast<Ivory::OpenGLShader>(m_shader)->upload_uniform_vec3("u_color", m_color);

	Ivory::Renderer::submit(m_vertex_array, m_shader, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)));


	Ivory::Renderer::end_scene();
}

void Test2D::on_imgui_render() {
	ImGui::Begin("Color");
	ImGui::ColorPicker3("Color", glm::value_ptr(m_color));
	ImGui::End();
}

void Test2D::on_event(Ivory::Event& e) {}