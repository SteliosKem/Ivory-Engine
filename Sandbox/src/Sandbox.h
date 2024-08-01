#pragma once
#include <IvoryEngine.h>

// For testing layers
class ExampleLayer : public Ivory::Layer {
public:
	ExampleLayer() : Layer("Example") {}

	void on_update() override {
		//IV_INFO("Example Layer Update");
	}

	void on_event(Ivory::Event& e) override {
		//IV_TRACE(e.to_string());
		if (Ivory::Input::is_key_pressed(IV_KEY_TAB))
			IV_TRACE("TAB");
	}
};

class Sandbox : public Ivory::Application {
public:
	Sandbox() { 
		push_layer(new ExampleLayer());
		push_overlay(new Ivory::ImGuiLayer());
	}
	~Sandbox() {}
};