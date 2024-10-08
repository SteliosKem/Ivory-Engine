#pragma once

#include "Core/Core.h"
#include "Events/Event.h"

#include "Core/Timestep.h"

namespace Ivory {
	class IVORY_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void on_attach() {}
		virtual void on_detach() {}
		virtual void on_update(Timestep time_step) {}
		virtual void on_imgui_render() {}
		virtual void on_event(Event& e) {}

		inline const std::string& get_name() const { return m_debug_name; }
	protected:
		std::string m_debug_name;
	};
}