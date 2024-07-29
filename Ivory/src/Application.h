#pragma once
#include "Core.h"

#include "Event.h"
#include "Window.h"

namespace Ivory {
	class IVORY_API Application
	{
	public:
		Application();
		virtual ~Application() {}
		
		void run();
	private:
		std::unique_ptr<Window> m_window;
		bool m_running = true;
	};

	Application* create_application();
}
