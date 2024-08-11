#include "pch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "imgui_spectrum.h"
#include "GLFW/glfw3.h"
#include "Core/Application.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "ImGuiNotify.h"
#include "IconsFontAwesome6.h"
#include <fstream>

namespace Ivory {
#define BIND_EVENT_FN(x) std::bind(&ImGuiLayer::x, this, std::placeholders::_1)
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

	ImGuiLayer::~ImGuiLayer() {

	}

	void ImGuiLayer::on_attach() {
        // ImGui initialization on attach
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        // Enable these config flags
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::Spectrum::StyleColorsSpectrum();
        ImGui::Spectrum::LoadFont(18);

        Application& app = Application::get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.get_window().get_native());
        
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");

        io.Fonts->AddFontDefault();

        float baseFontSize = 16.0f; // Default font size
        float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

        // Check if FONT_ICON_FILE_NAME_FAS is a valid path
        //std::ifstream fontAwesomeFile(FONT_ICON_FILE_NAME_FAS);

        //if (!fontAwesomeFile.good())
        //{
            // If it's not good, then we can't find the font and should abort
        //    std::cerr << "Could not find the FontAwesome font file." << std::endl;
        //    abort();
        //}

        static const ImWchar iconsRanges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
        ImFontConfig iconsConfig;
        iconsConfig.MergeMode = true;
        iconsConfig.PixelSnapH = true;
        iconsConfig.GlyphMinAdvanceX = iconFontSize;
        // WILL CHANGE
        io.Fonts->AddFontFromFileTTF("C:/Projects/Ivory-Engine/Ivory/src/ImGui/fa-solid-900.ttf", iconFontSize, &iconsConfig, iconsRanges);
	}

	void ImGuiLayer::on_detach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

    // On new frame
    void ImGuiLayer::begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    // On end of frame
    void ImGuiLayer::end() {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::get();
        // Set current display size
        io.DisplaySize = ImVec2(app.get_window().get_width(), app.get_window().get_height());

        // Render the frame
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update viewports
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::on_imgui_render() {
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }
}