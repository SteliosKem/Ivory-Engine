#include "pch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/imgui_impl_opengl3.h"
#include "imgui_spectrum.h"
#include "GLFW/glfw3.h"
#include "Application.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

static ImGuiKey glfw_to_imgui_key(int key)
{
    switch (key)
    {
    case GLFW_KEY_TAB: return ImGuiKey_Tab;
    case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
    case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
    case GLFW_KEY_UP: return ImGuiKey_UpArrow;
    case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
    case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
    case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
    case GLFW_KEY_HOME: return ImGuiKey_Home;
    case GLFW_KEY_END: return ImGuiKey_End;
    case GLFW_KEY_INSERT: return ImGuiKey_Insert;
    case GLFW_KEY_DELETE: return ImGuiKey_Delete;
    case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
    case GLFW_KEY_SPACE: return ImGuiKey_Space;
    case GLFW_KEY_ENTER: return ImGuiKey_Enter;
    case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
    case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
    case GLFW_KEY_COMMA: return ImGuiKey_Comma;
    case GLFW_KEY_MINUS: return ImGuiKey_Minus;
    case GLFW_KEY_PERIOD: return ImGuiKey_Period;
    case GLFW_KEY_SLASH: return ImGuiKey_Slash;
    case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
    case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
    case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
    case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
    case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
    case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
    case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
    case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
    case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
    case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
    case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
    case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
    case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
    case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
    case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
    case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
    case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
    case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
    case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
    case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
    case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
    case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
    case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
    case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
    case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
    case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
    case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
    case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
    case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
    case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
    case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
    case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
    case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
    case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
    case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
    case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
    case GLFW_KEY_MENU: return ImGuiKey_Menu;
    case GLFW_KEY_0: return ImGuiKey_0;
    case GLFW_KEY_1: return ImGuiKey_1;
    case GLFW_KEY_2: return ImGuiKey_2;
    case GLFW_KEY_3: return ImGuiKey_3;
    case GLFW_KEY_4: return ImGuiKey_4;
    case GLFW_KEY_5: return ImGuiKey_5;
    case GLFW_KEY_6: return ImGuiKey_6;
    case GLFW_KEY_7: return ImGuiKey_7;
    case GLFW_KEY_8: return ImGuiKey_8;
    case GLFW_KEY_9: return ImGuiKey_9;
    case GLFW_KEY_A: return ImGuiKey_A;
    case GLFW_KEY_B: return ImGuiKey_B;
    case GLFW_KEY_C: return ImGuiKey_C;
    case GLFW_KEY_D: return ImGuiKey_D;
    case GLFW_KEY_E: return ImGuiKey_E;
    case GLFW_KEY_F: return ImGuiKey_F;
    case GLFW_KEY_G: return ImGuiKey_G;
    case GLFW_KEY_H: return ImGuiKey_H;
    case GLFW_KEY_I: return ImGuiKey_I;
    case GLFW_KEY_J: return ImGuiKey_J;
    case GLFW_KEY_K: return ImGuiKey_K;
    case GLFW_KEY_L: return ImGuiKey_L;
    case GLFW_KEY_M: return ImGuiKey_M;
    case GLFW_KEY_N: return ImGuiKey_N;
    case GLFW_KEY_O: return ImGuiKey_O;
    case GLFW_KEY_P: return ImGuiKey_P;
    case GLFW_KEY_Q: return ImGuiKey_Q;
    case GLFW_KEY_R: return ImGuiKey_R;
    case GLFW_KEY_S: return ImGuiKey_S;
    case GLFW_KEY_T: return ImGuiKey_T;
    case GLFW_KEY_U: return ImGuiKey_U;
    case GLFW_KEY_V: return ImGuiKey_V;
    case GLFW_KEY_W: return ImGuiKey_W;
    case GLFW_KEY_X: return ImGuiKey_X;
    case GLFW_KEY_Y: return ImGuiKey_Y;
    case GLFW_KEY_Z: return ImGuiKey_Z;
    case GLFW_KEY_F1: return ImGuiKey_F1;
    case GLFW_KEY_F2: return ImGuiKey_F2;
    case GLFW_KEY_F3: return ImGuiKey_F3;
    case GLFW_KEY_F4: return ImGuiKey_F4;
    case GLFW_KEY_F5: return ImGuiKey_F5;
    case GLFW_KEY_F6: return ImGuiKey_F6;
    case GLFW_KEY_F7: return ImGuiKey_F7;
    case GLFW_KEY_F8: return ImGuiKey_F8;
    case GLFW_KEY_F9: return ImGuiKey_F9;
    case GLFW_KEY_F10: return ImGuiKey_F10;
    case GLFW_KEY_F11: return ImGuiKey_F11;
    case GLFW_KEY_F12: return ImGuiKey_F12;
    default: return ImGuiKey_None;
    }
}

namespace Ivory {
#define BIND_EVENT_FN(x) std::bind(&ImGuiLayer::x, this, std::placeholders::_1)
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

	ImGuiLayer::~ImGuiLayer() {

	}

	void ImGuiLayer::on_attach() {
        ImGui::CreateContext();
        ImGui::Spectrum::StyleColorsSpectrum();
        ImGui::Spectrum::LoadFont(18);

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        

        ImGui_ImplOpenGL3_Init();
	}

	void ImGuiLayer::on_detach() {
		
	}

	void ImGuiLayer::on_update() {
        ImGuiIO& io = ImGui::GetIO();
        Application& application = Application::get();
        io.DisplaySize = ImVec2(application.get_window().get_width(), application.get_window().get_height());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_time > 0.0 ? (time - m_time) : (1.0f / 60.0f);
        m_time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::on_event(Event& e) {
        EventDispatcher dispatcher(e);
        IV_ERROR(e.to_string());
        dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(on_mouse_button_pressed));
        dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(on_mouse_button_released));
        dispatcher.dispatch<MouseMovedEvent>(BIND_EVENT_FN(on_mouse_move));
        dispatcher.dispatch<MouseScrollEvent>(BIND_EVENT_FN(on_mouse_scroll));
        dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(on_key_pressed));
        dispatcher.dispatch<KeyReleasedEvent>(BIND_EVENT_FN(on_key_released));
        dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(on_window_resize));
	}

    bool ImGuiLayer::on_mouse_button_pressed(MouseButtonPressedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        //int mb = e.get_mouse_button();
        //if (mb >= 0 && mb < ImGuiMouseButton_COUNT)
        //    io.AddMouseButtonEvent(mb, true);
        io.MouseDown[e.get_mouse_button()] = true;
        return false;
    }
    bool ImGuiLayer::on_mouse_button_released(MouseButtonReleasedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        //int mb = e.get_mouse_button();
        //if (mb >= 0 && mb < ImGuiMouseButton_COUNT)
        //    io.AddMouseButtonEvent(mb, false);
        io.MouseDown[e.get_mouse_button()] = false;

        return false;
    }
    bool ImGuiLayer::on_mouse_move(MouseMovedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.get_x(), e.get_y());
        //io.AddMousePosEvent(e.get_x(), e.get_y());

        return false;
    }
    bool ImGuiLayer::on_mouse_scroll(MouseScrollEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += e.get_offset_y();
        io.MouseWheelH += e.get_offset_x();
        //io.AddMouseWheelEvent(e.get_offset_x(), e.get_offset_y());

        return false;
    }
    bool ImGuiLayer::on_key_pressed(KeyPressedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        //io.KeysDown[(ImGuiKey)e.get_keycode()] = true;
        io.AddKeyEvent(e.get_keycode(), true);

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL || GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT || GLFW_KEY_RIGHT_SHIFT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER || GLFW_KEY_RIGHT_SUPER];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT || GLFW_KEY_RIGHT_ALT];

        return false;
    }
    bool ImGuiLayer::on_key_released(KeyReleasedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        //io.KeysDown[(ImGuiKey)e.get_keycode()] = false;
        io.AddKeyEvent(e.get_keycode(), false);

        return false;
    }
    bool ImGuiLayer::on_key_typed(KeyTypedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        int c = e.get_keycode();
        if (c > 0 && c < 0x10000)
            io.AddInputCharacter((unsigned short)c);

        return false;
    }
    bool ImGuiLayer::on_window_resize(WindowResizeEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.get_width(), e.get_height());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, e.get_width(), e.get_height());

        return false;
    }
}