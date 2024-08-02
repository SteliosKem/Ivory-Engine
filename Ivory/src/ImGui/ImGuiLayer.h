#pragma once
#include "Layers/Layer.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

namespace Ivory {
	class IVORY_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void on_attach();
		virtual void on_detach();
		virtual void on_imgui_render() override;
		void begin();
		void end();
	private:
		float m_time = 0.0f;
	};
}