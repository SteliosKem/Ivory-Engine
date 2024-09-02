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
#include "Panels/ContentBrowser.h"

#include "Windows/ProjectSetupWindow.h"

#include "AlchemistSandbox.h"


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
		void show_setup_window() {}
	private:
		bool on_key_pressed(KeyPressedEvent& e);
		bool on_mouse_button_pressed(MouseButtonPressedEvent& e);

		void create_project(const std::string& project_name, const std::string& project_path);

		void open_scene();
		void save_scene();
		void save_scene_as();
		void new_scene();

		std::shared_ptr<Ivory::Texture2D> m_texture;
		std::shared_ptr<Ivory::Texture2D> m_texture2;

		std::shared_ptr<Ivory::Scene> m_active_scene;

		
		EditorCamera m_editor_camera; 

		Entity m_camera_entity;
		Ivory::OrthographicCameraController m_camera_controller;
		std::shared_ptr<Ivory::FrameBuffer> m_frame_buffer;

		glm::vec2 m_viewport_size{ 0.0f, 0.0f };
		glm::vec2 m_viewport_bounds[2];

		bool m_viewport_focused = false;
		bool m_viewport_hovered = false;

		bool m_willopen_scene = false;
		bool m_willsave_scene = false;

		std::string current_scene_file = "";
		int m_gizmo = -1;
		bool m_using_gizmo = false;

		Entity m_entity_hovered;

		SceneHierarchy m_hierarchy;
		ContentBrowser m_content_browser;

		ProjectSetupWindow m_setup_window;

		std::string current_path;
	};

	class Editor : public Application {
	public:
		Editor() {
			//Ivory::Application::get_window().set_vsync(false);
			//push_layer(std::make_shared<Test2D>());
			//push_layer(std::make_shared<EditorLayer>());
			push_layer(std::make_shared<AlchemistSandbox>());

			get_window().set_image(icon_path);
		}
		~Editor() {}
	};
}

