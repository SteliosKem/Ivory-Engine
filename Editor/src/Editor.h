#pragma once
#include <Core/IvoryEngine.h>

#include "Core/Entry.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "ImGui/ImGuiNotify.h"

#include "Test2D.h"

#include "Panels/SceneHierarchy.h"

const std::string icon_path = "Assets/IVlogo.png";
const std::string shader_path = "Assets/shaders/shader.glsl";

namespace Ivory {
	class EditorLayer : public Ivory::Layer {
	public:
		EditorLayer();

		void on_attach() override;
		void on_detach() override;

		void on_update(Ivory::Timestep dt) override;
		void on_imgui_render() override;

		void on_event(Ivory::Event& e) override;
	private:

		std::shared_ptr<Ivory::Texture2D> m_texture;
		std::shared_ptr<Ivory::Texture2D> m_texture2;

		std::shared_ptr<Ivory::Scene> m_active_scene;

		Entity m_camera_entity;
		Ivory::OrthographicCameraController m_camera_controller;
		std::shared_ptr<Ivory::FrameBuffer> m_frame_buffer;

		glm::vec2 m_viewport_size{ 0.0f, 0.0f };

		bool m_viewport_focused = false;
		bool m_viewport_hovered = false;

		SceneHierarchy m_hierarchy;
	};

	class Editor : public Application {
	public:
		Editor() {
			//Ivory::Application::get_window().set_vsync(false);
			//push_layer(std::make_shared<Test2D>());
			push_layer(std::make_shared<EditorLayer>());

			get_window().set_image(icon_path);
		}
		~Editor() {}
	};
}

