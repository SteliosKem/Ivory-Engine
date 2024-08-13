#pragma once
#include <Core/IvoryEngine.h>

class Test2D : public Ivory::Layer {
public:
	Test2D();

	void on_attach() override;
	void on_detach() override;

	void on_update(Ivory::Timestep dt) override;
	void on_imgui_render() override;

	void on_event(Ivory::Event& e) override;
private:
	std::shared_ptr<Ivory::Shader> m_shader;
	std::shared_ptr<Ivory::VertexArray> m_vertex_array;

	std::shared_ptr<Ivory::Texture2D> m_texture;
	std::shared_ptr<Ivory::Texture2D> m_texture2;

	Ivory::OrthographicCameraController m_camera_controller;
	glm::vec3 m_color{ 1.0f };
};