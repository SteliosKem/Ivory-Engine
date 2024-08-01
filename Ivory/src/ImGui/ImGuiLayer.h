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

		void on_attach();
		void on_detach();
		void on_update();
		void on_event(Event& e);
	private:
		bool on_mouse_button_pressed(MouseButtonPressedEvent& e);
		bool on_mouse_button_released(MouseButtonReleasedEvent& e);
		bool on_mouse_move(MouseMovedEvent& e);
		bool on_mouse_scroll(MouseScrollEvent& e);
		bool on_key_pressed(KeyPressedEvent& e);
		bool on_key_released(KeyReleasedEvent& e);
		bool on_key_typed(KeyTypedEvent& e);
		bool on_window_resize(WindowResizeEvent& e);
		float m_time = 0.0f;
	};
}