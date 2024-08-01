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
		IV_TRACE(e.to_string());
		//IV_CORE_TRACE(e.to_string());
	}
};

class Sandbox : public Ivory::Application {
public:
	Sandbox() { 
		//push_layer(new ExampleLayer());
		push_overlay(new Ivory::ImGuiLayer());
	}
	~Sandbox() {}
};