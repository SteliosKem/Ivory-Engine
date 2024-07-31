#pragma once
#include "Layers/Layer.h"

namespace Ivory {
	class IVORY_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void on_attach();
		void on_detach();
		void on_update();
		void on_event(Event& e);
	private:
		float m_time = 0.0f;
	};
}