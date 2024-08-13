#include "Test2D.h"
#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

Test2D::Test2D() : Layer("Test2D"), m_camera_controller(1280.0f / 720.f) {

}

void Test2D::on_attach() {
	m_texture = Ivory::Texture2D::create("C:/Projects/Ivory-Engine/Editor/Assets/Zeus.png");
	m_texture2 = Ivory::Texture2D::create("C:/Projects/Ivory-Engine/Editor/Assets/IVlogo.png");
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

	Ivory::Renderer2D::begin_scene(m_camera_controller.get_camera());

	Ivory::Quad textured_quad{ {1.1f, 1.1f, 0.0f }, { 0.8f, 0.8f }, 0, {0.4f, 1.0f, 1.0f, 1.0f}, m_texture };
	Ivory::Quad textured_quad2{ {-1.1f, 1.1f, 0.0f }, { 2.0f, 2.0f }, 0, {0.4f, 1.0f, 1.0f, 1.0f}, m_texture2 };
	textured_quad.texture_info.tiling_factor = 4.0f;
	Ivory::Quad quad2{};
	Ivory::Quad quad3{ {1.0f, 0, 0}, {1.0f, 1.0f}, 0, {0.5f, 0.2f, 0.2f, 1.0f} };
	quad2.color = glm::vec4(m_color, 1.0f);

	Ivory::Renderer2D::draw_quad(quad2);
	Ivory::Renderer2D::draw_quad(quad3);
	Ivory::Renderer2D::draw_quad(textured_quad2);
	Ivory::Renderer2D::draw_quad(textured_quad);
	//Ivory::Renderer2D::draw_quad({ 1.1f, 1.1f, 0.0f }, { 0.8f, 0.8f }, m_texture2);
	//Ivory::Renderer2D::draw_quad({ -1.1f, 1.1f, 0.0f }, { 2.0f, 2.0f }, m_texture);
	
	Ivory::Renderer2D::end_scene();
}

void Test2D::on_imgui_render() {
	ImGui::Begin("Color");
	ImGui::ColorPicker3("Color", glm::value_ptr(m_color));
	ImGui::End();
}

void Test2D::on_event(Ivory::Event& e) {
	m_camera_controller.on_event(e);
}