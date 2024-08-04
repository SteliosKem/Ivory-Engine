#pragma once
#include <IvoryEngine.h>

#include "imgui.h"

// For testing layers
class ExampleLayer : public Ivory::Layer {
public:
	ExampleLayer() : Layer("Example") {}

	void on_update() override {
		//IV_INFO("Example Layer Update");
	}

	void on_imgui_render() override {
		//ImGui::Begin("Example");
		//ImGui::Text("Example");
		//ImGui::End();
	}

	void on_event(Ivory::Event& e) override {
		//IV_TRACE(e.to_string());
		if (Ivory::Input::is_key_pressed(IV_KEY_TAB))
			IV_TRACE("TAB");
	}
};

class Editor : public Ivory::Application {
public:
	Editor() {
		push_layer(std::make_shared<ExampleLayer>());
	}
	~Editor() {}
};