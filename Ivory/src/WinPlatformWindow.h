#pragma once
#include "Window.h"

#include "GLFW/glfw3.h"

namespace Ivory {
	class WinPlatformWindow : public Window {
	public:
		WinPlatformWindow(const WindowProps& props);
		virtual ~WinPlatformWindow();

		void on_update() override;

		inline unsigned int get_width() const override { return m_data.width; }
		inline unsigned int get_height() const override { return m_data.height; }

		inline void set_event_callback(const EventCallback& callback) override { m_data.event_callback = callback; }
		void set_vsync(bool enabled);
		bool get_vsync() const;
	private:
		virtual void init(const WindowProps& props);
		virtual void shutdown();

		GLFWwindow* m_window;

		struct WindowData {
			std::string title;
			unsigned int width, height;
			bool vsync;
			EventCallback event_callback;
		};

		WindowData m_data;
	};
}