#pragma once
#include <Core/IvoryEngine.h>

#include "Core/Entry.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "ImGui/ImGuiNotify.h"

#include "Test2D.h"

const std::string icon_path = "C:/Projects/Ivory-Engine/Editor/Assets/IVlogo.png";
const std::string shader_path = "C:/Projects/Ivory-Engine/Editor/Assets/shaders/shader.glsl";

// For testing layers
class EditorLayer : public Ivory::Layer {
public:
	EditorLayer();

	void on_attach() override;
	void on_detach() override;

	void on_update(Ivory::Timestep dt) override;
	void on_imgui_render() override;

	void on_event(Ivory::Event& e) override;
private:
	Ivory::OrthographicCameraController m_camera_controller;
};

class Editor : public Ivory::Application {
public:
	Editor() {
		//Ivory::Application::get_window().set_vsync(false);
		//push_layer(std::make_shared<Test2D>());
		push_layer(std::make_shared<EditorLayer>());
		
		get_window().set_image(icon_path);
	}
	~Editor() {}
};