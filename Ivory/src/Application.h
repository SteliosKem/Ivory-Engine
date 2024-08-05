#pragma once
#include "Core.h"

#include "Events/Event.h"
#include "Window.h"
#include "Events/WindowEvent.h"
#include "Layers/LayerStack.h"
#include "ImGui/ImGuiLayer.h"

#include "Rendering/Shader.h"
#include <Rendering/Buffer.h>
#include "Rendering/OrthographicCamera.h"
#include "Core/Timestep.h"

namespace Ivory {
	class IVORY_API Application
	{
	public:
		Application();
		virtual ~Application() {}
		
		void run();

		void on_event(Event& e);

		void push_layer(shared_ptr<Layer> layer);
		void push_overlay(shared_ptr<Layer> layer);

		inline Window& get_window() { return *m_window; }
		inline static Application& get() { return *s_instance; }
	private:
		bool on_window_close(WindowCloseEvent& e);

		std::unique_ptr<Window> m_window;
		std::shared_ptr<ImGuiLayer> m_imgui_layer;
		bool m_running = true;

		LayerStack m_layer_stack;

		static Application* s_instance;
		float m_last_frame_time = 0.0f;
	};

	Application* create_application();
}
