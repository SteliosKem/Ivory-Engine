#include "AlchemistSandbox.h"
#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

AlchemistSandbox::AlchemistSandbox() : Layer("Test2D"), m_camera_controller(1280.0f / 720.f) {}

void AlchemistSandbox::on_attach() {}

void AlchemistSandbox::on_detach() {}

void AlchemistSandbox::on_update(Ivory::Timestep dt) {
	m_camera_controller.on_update(dt);

	Ivory::RenderCommand::set_clear_color({ 0.1f, 0.1f, 0.1f, 1 });
	Ivory::RenderCommand::clear();

	Ivory::Renderer2D::begin_scene(m_camera_controller.get_camera());

	Ivory::Quad quad2{};
	quad2.color = glm::vec4(m_color, 1.0f);

	Ivory::Renderer2D::draw_quad(quad2);

	Ivory::Renderer2D::end_scene();
}

void AlchemistSandbox::on_imgui_render() {
}

void AlchemistSandbox::on_event(Ivory::Event& e) {
	m_camera_controller.on_event(e);
}