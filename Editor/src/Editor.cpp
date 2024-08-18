#include "Editor.h"
#include "imgui.h"
#include "Windows/Dialogs.h"
#include "Scene/SceneSerializer.h"
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
#include "ImGuizmo.h"
#include "Core/Math.h"
#include "Rendering/EditorCamera.h"

namespace Ivory {
    EditorLayer::EditorLayer() : Layer("Test2D"), m_camera_controller(1280.0f / 720.f) {
    
    }

    void EditorLayer::on_attach() {
        m_texture = Texture2D::create("Assets/Zeus.png");
        m_texture2 = Texture2D::create("Assets/IVlogo.png");

        FrameBufferSpecification frame_buffer_spec;
        frame_buffer_spec.width = 1280;
        frame_buffer_spec.height = 720;
        frame_buffer_spec.attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
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

        m_editor_camera = EditorCamera(30.0f, 16.0f / 9.0f, 0.01f, 1000.0f);
    }
    void EditorLayer::on_detach() {}

    void EditorLayer::on_update(Timestep dt) {
        /*FrameBufferSpecification spec = m_frame_buffer->get_spec();
        if (m_viewport_size.x > 0.0f && m_viewport_size.y > 0.0f && (spec.width != m_viewport_size.x || spec.height != m_viewport_size.y)) {
            m_frame_buffer->resize((uint32_t)m_viewport_size.x, );

        }*/

        if (m_viewport_hovered && m_viewport_focused)
            m_camera_controller.pass_events(true);
        else
            m_camera_controller.pass_events(false);
        m_camera_controller.on_update(dt);
        m_editor_camera.on_update(dt);

        m_frame_buffer->bind();
        RenderCommand::set_clear_color({ 0.1f, 0.1f, 0.1f, 1 });
        RenderCommand::clear();

        m_frame_buffer->clear_attachment(1, -1);

        static float rot = 0;
        rot += dt;
        float x_pos = 2 * sinf(rot);

        m_active_scene->on_update_editor(dt, m_editor_camera);

        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_viewport_bounds[0].x;
        my -= m_viewport_bounds[0].y;
        glm::vec2 viewport_size = m_viewport_bounds[1] - m_viewport_bounds[0];
        my = viewport_size.y - my;

        int mousex = (int)mx;
        int mousey = (int)my;

        if (mousex >= 0 && mousey >= 0 && mousex < (int)m_viewport_size.x && mousey < (int)m_viewport_size.y) {
            int pixel_data = m_frame_buffer->read_pixel(1, mousex, mousey);
            //m_hierarchy.set_selected(pixel_data == -1 ? Entity() : Entity((entt::entity)pixel_data, m_active_scene.get()));

            m_entity_hovered = pixel_data == -1 ? Entity() : Entity((entt::entity)pixel_data, m_active_scene.get());
        }
       

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

        // VIEWPORT

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");

        float tab_height = ImGui::GetWindowSize().y - ImGui::GetContentRegionAvail().y;
        auto viewport_offset = ImGui::GetCursorPos();
        viewport_offset.y -= tab_height;

        m_viewport_focused = ImGui::IsWindowFocused();
        m_viewport_hovered = ImGui::IsWindowHovered();
        ImVec2 vp_size = ImGui::GetContentRegionAvail();
        if ((vp_size.x != m_viewport_size.x || vp_size.y != m_viewport_size.y) && vp_size.x > 0 && vp_size.y > 0) {
            m_viewport_size = { vp_size.x, vp_size.y };
            m_frame_buffer->resize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
            m_camera_controller.resize_bounds(vp_size.x, vp_size.y);
            m_editor_camera.set_viewport_size(vp_size.x, vp_size.y);
            m_active_scene->on_viewport_resize((uint32_t)vp_size.x, (uint32_t)vp_size.y);
        }
        uint32_t texture_id = m_frame_buffer->get_color_attachment_rendererID();
        ImGui::Image((void*)texture_id, ImVec2{ m_viewport_size.x, m_viewport_size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        auto window_size = ImGui::GetWindowSize();
        ImVec2 min_bound = ImGui::GetWindowPos();
        min_bound.x += viewport_offset.x;
        min_bound.y += viewport_offset.y;

        ImVec2 max_bound = { min_bound.x + window_size.x, min_bound.y + window_size.y };
        m_viewport_bounds[0] = { min_bound.x, min_bound.y };
        m_viewport_bounds[1] = { max_bound.x, max_bound.y };

        Entity selected = m_hierarchy.get_selected();
        if (selected) {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            float window_width = (float)ImGui::GetWindowWidth();
            float window_height = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_width, window_height);

            /*auto camera_entity = m_active_scene->get_primary_camera();
            const auto& camera = camera_entity.get_component<CameraComponent>().camera;
            const glm::mat4& camera_projection = camera.get_projection();
            glm::mat4& camera_view = glm::inverse(camera_entity.get_component<TransformComponent>().get_transform());*/

            const glm::mat4& camera_projection = m_editor_camera.get_projection();
            glm::mat4 camera_view = m_editor_camera.get_view_matrix();

            auto& transform_component = selected.get_component<TransformComponent>();
            glm::mat4 transform = transform_component.get_transform();

            bool snap = Input::is_key_pressed(IV_KEY_LEFT_CONTROL);
            float snap_value = m_gizmo == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f;

            float snap_values[3] = { snap_value, snap_value, snap_value };

            ImGuizmo::Manipulate(glm::value_ptr(camera_view), glm::value_ptr(camera_projection), (ImGuizmo::OPERATION)m_gizmo, ImGuizmo::LOCAL, glm::value_ptr(transform)
                , nullptr, snap ? snap_values : nullptr);

            if (ImGuizmo::IsUsing()) {
                m_using_gizmo = true;
                glm::vec3 translation, rotation, scale;
                decompose_transform(transform, translation, rotation, scale);
                transform_component.translation = translation;
                transform_component.rotation = rotation;
                transform_component.scale = scale;
            }
            else
                m_using_gizmo = false;


        }

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
        m_editor_camera.on_event(e);

        EventDispatcher dispatcher(e);
        dispatcher.dispatch<KeyPressedEvent>(IV_BIND_EVENT_FN(EditorLayer::on_key_pressed));
        dispatcher.dispatch<MouseButtonPressedEvent>(IV_BIND_EVENT_FN(EditorLayer::on_mouse_button_pressed));
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

        case IV_KEY_Q:
            if(!m_using_gizmo)
                m_gizmo = ImGuizmo::OPERATION::TRANSLATE;
            break;
        case IV_KEY_W:
            if (!m_using_gizmo)
                m_gizmo = ImGuizmo::OPERATION::ROTATE;
            break;
        case IV_KEY_E:
            if (!m_using_gizmo)
                m_gizmo = ImGuizmo::OPERATION::SCALE;
            break;
        case IV_KEY_R:
            if (!m_using_gizmo)
                m_gizmo = -1;
            break;
        }
    }

    bool EditorLayer::on_mouse_button_pressed(MouseButtonPressedEvent& e) {
        if (e.get_mouse_button() == IV_MOUSE_BUTTON_1 && m_viewport_hovered && m_entity_hovered && !ImGuizmo::IsOver()) {
            m_hierarchy.set_selected(m_entity_hovered);
        }
        return false;
    }

    void EditorLayer::open_scene() {
        std::string file_path; FileDialogs::open_file("", file_path);
        if (file_path != "") {
            m_active_scene = std::make_shared<Scene>();
            m_active_scene->on_viewport_resize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
            m_hierarchy.set_context(m_active_scene);

            SceneSerializer serializer(m_active_scene);
            serializer.deserialize(file_path);

            current_scene_file = file_path;
        }
    }
    void EditorLayer::save_scene() {
        if (current_scene_file.empty()) {
            FileDialog::file_dialog_open = true;
            m_willopen_scene = false;
            m_willsave_scene = true;
        }
        else {
            SceneSerializer serializer(m_active_scene);
            serializer.serialize(current_scene_file);
        }
    }
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
