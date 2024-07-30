#pragma once
#include "Core.h"

#include "Event.h"
#include "Window.h"
#include "WindowEvent.h"
#include "LayerStack.h"

namespace Ivory {
	class IVORY_API Application
	{
	public:
		Application();
		virtual ~Application() {}
		
		void run();

		void on_event(Event& e);

		void push_layer(Layer* layer);
		void push_overlay(Layer* layer);
	private:
		bool on_window_close(WindowCloseEvent& e);

		std::unique_ptr<Window> m_window;
		bool m_running = true;

		LayerStack m_layer_stack;
	};

	Application* create_application();
}
