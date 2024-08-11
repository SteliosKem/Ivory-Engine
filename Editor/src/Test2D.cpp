#include "Test2D.h"
#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

Test2D::Test2D() : Layer("Test2D"), m_camera_controller(1280.0f / 720.f) {

}

void Test2D::on_attach() {
	
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
	Ivory::Renderer2D::draw_quad({0.0f, 0.0f}, {1.0f, 1.0f}, glm::vec4(m_color, 1.0f));
	Ivory::Renderer2D::draw_quad({ 1.0f, -1.0f }, { 1.0f, 0.5f }, glm::vec4(m_color, 0.5f));
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