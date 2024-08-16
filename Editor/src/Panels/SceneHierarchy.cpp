#include "SceneHierarchy.h"
#include "imgui.h"
#include "Scene/Components.h"
#include <glm/gtc/type_ptr.hpp>
#include "imgui_internal.h"

namespace Ivory {
	SceneHierarchy::SceneHierarchy(const std::shared_ptr<Scene>& scene) {
		set_context(scene);
	}
	void SceneHierarchy::set_context(const std::shared_ptr<Scene>& scene) {
		m_context = scene;
	}

	void SceneHierarchy::on_imgui_render() {
		ImGui::Begin("Scene Hierarchy");
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_selection_context = {};
		auto view = m_context->m_registry.view<TagComponent>();
		for (auto entity : view) {
			draw_entity_node({entity, m_context.get()});
		}

		

		ImGui::End();

		ImGui::Begin("Inspector");
		if (m_selection_context)
			draw_components(m_selection_context);
		else
			ImGui::Text("Select a scene entity to inspect its elements");
		ImGui::End();
	}

	void SceneHierarchy::draw_entity_node(Entity entity) {
		auto& tag = entity.get_component<TagComponent>();

		ImGuiTreeNodeFlags flags = ((m_selection_context == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint32_t)entity, flags, tag.tag.c_str());

		if (ImGui::IsItemClicked()) {
			m_selection_context = entity;
		}

		if (opened) {
			ImGui::TreePop();
		}
	}

	static void draw_vec3_control(const std::string& label, glm::vec3& values, float speed,  float reset_value = 0.0f, float column_width = 100.0f) {
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, column_width);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

		float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 button_size = { line_height + 3.0f, line_height };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.6f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.6f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.6f, 0.2f, 0.2f, 1.0f });
		if (ImGui::Button("X", button_size))
			values.x = reset_value;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.6f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.6f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.6f, 0.2f, 1.0f });
		if (ImGui::Button("Y", button_size))
			values.y = reset_value;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.6f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.6f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.2f, 0.6f, 1.0f });
		if (ImGui::Button("Z", button_size))
			values.z = reset_value;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();

		ImGui::Columns(1);
		ImGui::PopID();
	}

	void SceneHierarchy::draw_components(Entity entity) {
		if (entity.has_component<TagComponent>()) {
			auto& tag = entity.get_component<TagComponent>();
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.tag.c_str());
			ImGui::Text("Name");
			if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
				tag.tag = std::string(buffer);
			}
		}


		if (entity.has_component<TransformComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform Component")) {
				auto& transform = entity.get_component<TransformComponent>();
				float speed = 0.1f;

				draw_vec3_control("Translation", transform.translation, speed);
				glm::vec3 rotation = glm::degrees(transform.rotation);
				draw_vec3_control("Rotation", rotation, speed * 5);
				transform.rotation = glm::radians(rotation);
				draw_vec3_control("Scale", transform.scale, speed / 5, 1.0f);
				
				ImGui::TreePop();
			}
		}
		if (entity.has_component<CameraComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera Component")) {
				auto& camera_component = entity.get_component<CameraComponent>();
				auto& camera = camera_component.camera;
				
				ImGui::Checkbox("Is Primary", &camera_component.active);
				const char* projection_types[2] = {"Perspective", "Orthographic"};
				const char* projection_string = projection_types[(int)camera.get_projection_type()];
				if (ImGui::BeginCombo("Projection Type", projection_string)) {
					for (int i = 0; i < 2; i++) {
						bool is_selected = projection_string == projection_types[i];
						if (ImGui::Selectable(projection_types[i], is_selected)) {
							camera.set_projection_type((SceneCamera::ProjectionType)i);
						}

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.get_projection_type() == SceneCamera::ProjectionType::Perspective) {
					float fov = glm::degrees(camera.get_vertical_fov());
					if (ImGui::DragFloat("Vertical FOV", &fov))
						camera.set_vertical_fov(glm::radians(fov));

					float near_clip = camera.get_perspective_near_clip();
					if (ImGui::DragFloat("Near Clip", &near_clip))
						camera.set_perspective_near_clip(near_clip);

					float far_clip = camera.get_perspective_far_clip();
					if (ImGui::DragFloat("Far Clip", &far_clip))
						camera.set_perspective_far_clip(far_clip);
				}
				else if (camera.get_projection_type() == SceneCamera::ProjectionType::Orthographic) {
					float ortho_size = camera.get_ortho_size();
					if (ImGui::DragFloat("Size", &ortho_size))
						camera.set_ortho_size(ortho_size);

					float near_clip = camera.get_ortho_near_clip();
					if (ImGui::DragFloat("Near Clip", &near_clip))
						camera.set_ortho_near_clip(near_clip);

					float far_clip = camera.get_ortho_far_clip();
					if (ImGui::DragFloat("Far Clip", &far_clip))
						camera.set_ortho_far_clip(far_clip);

					ImGui::Checkbox("Has Fixed Aspect Ratio", &camera_component.fixed_aspect_ratio);
				}
				
				ImGui::TreePop();
			}
		}

		if (entity.has_component<SpriteRendererComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer Component")) {
				auto& sprite_renderer = entity.get_component<SpriteRendererComponent>();
				ImGui::ColorPicker4("Color", glm::value_ptr(sprite_renderer.color));
				
				ImGui::TreePop();
			}
		}
	}
}