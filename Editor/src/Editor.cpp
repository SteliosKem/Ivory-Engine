#include "Editor.h"
#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

EditorLayer::EditorLayer() : Layer("Test2D"), m_camera_controller(1280.0f / 720.f) {
    
}

void EditorLayer::on_attach() {
    m_texture = Ivory::Texture2D::create("C:/Projects/Ivory-Engine/Editor/Assets/Zeus.png");
    m_texture2 = Ivory::Texture2D::create("C:/Projects/Ivory-Engine/Editor/Assets/IVlogo.png");

    Ivory::FrameBufferSpecification frame_buffer_spec;
    frame_buffer_spec.width = 1280;
    frame_buffer_spec.height = 720;
    m_frame_buffer = Ivory::FrameBuffer::create(frame_buffer_spec);
}
void EditorLayer::on_detach() {}

void EditorLayer::on_update(Ivory::Timestep dt) {

    Ivory::Renderer2D::reset_stats();
	if (Ivory::Input::is_mouse_button_pressed(2)) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}
	else {
		ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
	}
    m_frame_buffer->bind();
	Ivory::RenderCommand::set_clear_color({ 0.1f, 0.1f, 0.1f, 1 });
	Ivory::RenderCommand::clear();

    static float rot = 0;
    rot += dt;
    float x_pos = 2 * sinf(rot);
    m_camera_controller.on_update(dt);

    Ivory::Renderer2D::begin_scene(m_camera_controller.get_camera());

    Ivory::Quad textured_quad{ {1.1f, 1.1f, 0.0f }, { 0.8f, 0.8f }, 0, {0.4f, 1.0f, 1.0f, 1.0f}, m_texture };
    Ivory::Quad textured_quad2{ {0.0f, 0.0f, 0.0f }, { 2.0f, 2.0f }, rot, {1.0f, 1.0f, 1.0f, 1.0f}, m_texture2 };
    textured_quad.texture_info.tiling_factor = 4.0f;
    Ivory::Quad quad2{};
    Ivory::Quad quad3{ {1.0f, 0, 0}, {1.0f, 1.0f}, 0, {0.5f, 0.2f, 0.2f, 1.0f} };
    quad2.color = glm::vec4(0.5f, 1.0f, 0.5f, 1.0f);

    Ivory::Renderer2D::draw_quad(quad2);
    Ivory::Renderer2D::draw_quad(quad3);
    Ivory::Renderer2D::draw_quad(textured_quad2);
    Ivory::Renderer2D::draw_quad(textured_quad);
    Ivory::Renderer2D::end_scene();

    m_frame_buffer->unbind();
}

void EditorLayer::on_imgui_render() {
    static bool docking = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &docking, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false))
                docking = false;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");
    ImVec2 vp_size = ImGui::GetContentRegionAvail();
    if (vp_size.x != m_viewport_size.x || vp_size.y != m_viewport_size.y) {
        m_viewport_size = { vp_size.x, vp_size.y };
        m_frame_buffer->resize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
        m_camera_controller.resize_bounds(vp_size.x, vp_size.y);
    }
    uint32_t texture_id = m_frame_buffer->get_color_attachment_rendererID();
    ImGui::Image((void*)texture_id, ImVec2{ m_viewport_size.x, m_viewport_size.y }, ImVec2{0, 1}, ImVec2{1, 0});
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

void EditorLayer::on_event(Ivory::Event& e) {
	m_camera_controller.on_event(e);
}

Ivory::Application* Ivory::create_application() {
	return new Editor();
}