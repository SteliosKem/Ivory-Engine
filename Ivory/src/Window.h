#pragma once
#include "pch.h"

#include "Core.h"
#include "Events/Event.h"

namespace Ivory {
	// Window data
	struct WindowProps {
		std::string title = "Ivory Engine";
		unsigned int width = 1280;
		unsigned int height = 720;
	};

	class IVORY_API Window {
	public:
		// Callback function type
		using EventCallback = std::function<void(Event&)>;

		virtual ~Window() {}
		virtual void on_update() = 0;

		virtual unsigned int get_width() const = 0;
		virtual unsigned int get_height() const = 0;

		virtual void set_event_callback(const EventCallback& func) = 0;
		virtual void set_vsync(bool enable) = 0;
		virtual bool get_vsync() const = 0;

		static Window* create(const WindowProps& props = WindowProps{});
	};
}