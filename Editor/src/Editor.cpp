#include "Editor.h"
#include "imgui.h"
#include "Windows/Dialogs.h"
#include "Scene/SceneSerializer.h"
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

namespace Ivory {
    EditorLayer::EditorLayer() : Layer("Test2D"), m_camera_controller(1280.0f / 720.f) {
    
    }

    void EditorLayer::on_attach() {
        m_texture = Texture2D::create("Assets/Zeus.png");
        m_texture2 = Texture2D::create("Assets/IVlogo.png");

        FrameBufferSpecification frame_buffer_spec;
        frame_buffer_spec.width = 1280;
        frame_buffer_spec.height = 720;
        m_frame_buffer = FrameBuffer::create(frame_buffer_spec);

        m_active_scene = std::make_shared<Scene>();

        /*Entity square_entity = m_active_scene->create_entity("Sprite");
        square_entity.add_component<SpriteRendererComponent>(glm::vec4{ 0.1f, 0.5f, 0.1f, 1.0f });
        m_camera_entity = m_active_scene->create_entity("Camera");
        m_camera_entity.add_component<CameraComponent>();

        class CameraController : public ScriptableEntity {
        public:
            void on_create() {}
            void on_destroy() {}
            void on_update(Timestep dt) {
                
            }
        };

        m_camera_entity.add_component<CScriptComponent>().bind<CameraController>();*/
        m_hierarchy.set_context(m_active_scene);

        SceneSerializer serializer(m_active_scene);

        //serializer.serialize("Assets/scenes/Example.iscene");
        serializer.deserialize("Assets/scenes/Example.iscene");
    }
    void EditorLayer::on_detach() {}

    void EditorLayer::on_update(Timestep dt) {

        if (m_viewport_hovered && m_viewport_focused)
            m_camera_controller.pass_events(true);
        else
            m_camera_controller.pass_events(false);
        m_camera_controller.on_update(dt);
       

        Renderer2D::reset_stats();
        if (Input::is_mouse_button_pressed(2)) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
        else {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
        }
        m_frame_buffer->bind();
        RenderCommand::set_clear_color({ 0.1f, 0.1f, 0.1f, 1 });
        RenderCommand::clear();

        static float rot = 0;
        rot += dt;
        float x_pos = 2 * sinf(rot);


        //Renderer2D::begin_scene(m_camera_controller.get_camera());

        /*Quad textured_quad{{1.1f, 1.1f, 0.0f}, {0.8f, 0.8f}, 0, {0.4f, 1.0f, 1.0f, 1.0f}, m_texture};
        Quad textured_quad2{ {0.0f, 0.0f, 0.0f }, { 2.0f, 2.0f }, rot, {1.0f, 1.0f, 1.0f, 1.0f}, m_texture2 };
        textured_quad.texture_info.tiling_factor = 4.0f;
        Quad quad2{};
        Quad quad3{ {1.0f, 0, 0}, {1.0f, 1.0f}, 0, {0.5f, 0.2f, 0.2f, 1.0f} };
        quad2.color = glm::vec4(0.5f, 1.0f, 0.5f, 1.0f);

        Renderer2D::draw_quad(quad2);
        Renderer2D::draw_quad(quad3);
        Renderer2D::draw_quad(textured_quad2);
        Renderer2D::draw_quad(textured_quad);*/

        m_active_scene->on_update(dt);
        //Renderer2D::end_scene();

        m_frame_buffer->unbind();
    }

    void EditorLayer::on_imgui_render() {
        static bool docking = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_NoWindowMenuButton;

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
        ImGuiStyle& style = ImGui::GetStyle();
        float min_size = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = min_size;


        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                //char* path = "C:\\Projects\\Ivory-Engine";
                if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
                    new_scene();
                }

                

                if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {
                    FileDialog::file_dialog_open = true;
                    m_willopen_scene = true;
                    m_willsave_scene = false;
                   
                }

                
                if (ImGui::MenuItem("Save Scene As", "Ctrl+Shift+S")) {
                    FileDialog::file_dialog_open = true;
                    m_willopen_scene = false;
                    m_willsave_scene = true;
                    
                }
                ImGui::MenuItem("Save Scene", "Ctrl+S");
                
                ImGui::EndMenu();

                
            }
            ImGui::EndMenuBar();
        }

        m_hierarchy.on_imgui_render();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");
        m_viewport_focused = ImGui::IsWindowFocused();
        m_viewport_hovered = ImGui::IsWindowHovered();
        ImVec2 vp_size = ImGui::GetContentRegionAvail();
        if ((vp_size.x != m_viewport_size.x || vp_size.y != m_viewport_size.y) && vp_size.x > 0 && vp_size.y > 0) {
            m_viewport_size = { vp_size.x, vp_size.y };
            m_frame_buffer->resize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
            m_camera_controller.resize_bounds(vp_size.x, vp_size.y);
            m_active_scene->on_viewport_resize((uint32_t)vp_size.x, (uint32_t)vp_size.y);
        }
        uint32_t texture_id = m_frame_buffer->get_color_attachment_rendererID();
        ImGui::Image((void*)texture_id, ImVec2{ m_viewport_size.x, m_viewport_size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
        if (FileDialog::file_dialog_open && m_willopen_scene)
            open_scene();
        else if (FileDialog::file_dialog_open && m_willsave_scene)
            save_scene_as();
        else {
            m_willopen_scene = false;
            m_willsave_scene = false;
        }
    }

    void EditorLayer::on_event(Event& e) {
        m_camera_controller.on_event(e);

        EventDispatcher dispatcher(e);
        dispatcher.dispatch<KeyPressedEvent>(IV_BIND_EVENT_FN(EditorLayer::on_key_pressed));
    }
    bool EditorLayer::on_key_pressed(KeyPressedEvent& e) {
        if (e.get_repeat_count() > 0)
            return false;

        bool control = Input::is_key_pressed(IV_KEY_LEFT_CONTROL) || Input::is_key_pressed(IV_KEY_RIGHT_CONTROL);
        bool shift = Input::is_key_pressed(IV_KEY_LEFT_SHIFT) || Input::is_key_pressed(IV_KEY_RIGHT_SHIFT);
        switch (e.get_keycode()) {
        case IV_KEY_S: {
            if (control && shift) {
                FileDialog::file_dialog_open = true;
                m_willopen_scene = false;
                m_willsave_scene = true;
            }
                
            else if (control)
                save_scene();
            break;
        }
        case IV_KEY_N:
            if (control)
                new_scene();
            break;
        case IV_KEY_O:
            if (control) {
                FileDialog::file_dialog_open = true;
                m_willopen_scene = false;
                m_willsave_scene = true;
            }
            break;
        }
    }

    void EditorLayer::open_scene() {
        std::string file_path; FileDialogs::open_file("", file_path);
        if (file_path != "") {
            m_active_scene = std::make_shared<Scene>();
            m_active_scene->on_viewport_resize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
            m_hierarchy.set_context(m_active_scene);

            SceneSerializer serializer(m_active_scene);
            serializer.deserialize(file_path);
        }
    }
    void EditorLayer::save_scene() {}
    void EditorLayer::save_scene_as() {
        std::string file_path; FileDialogs::save_file("", file_path);
        if (file_path != "") {
            SceneSerializer serializer(m_active_scene);
            serializer.serialize(file_path);
        }
    }
    void EditorLayer::new_scene() {
        m_active_scene = std::make_shared<Scene>();
        m_active_scene->on_viewport_resize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
        m_hierarchy.set_context(m_active_scene);
    }

    Application* create_application() {
        return new Editor();
    }
}
