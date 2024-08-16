#include "SceneHierarchy.h"
#include "imgui.h"
#include "Scene/Components.h"
#include <glm/gtc/type_ptr.hpp>

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

				if (ImGui::DragFloat3("Position", glm::value_ptr(transform.transform[3]), 0.1f)) {

				}
				if (ImGui::DragFloat3("Scale", glm::value_ptr(transform.transform[0]), 0.1f)) {

				}
				if (ImGui::DragFloat3("Rotation", glm::value_ptr(transform.transform[3]), 0.1f)) {

				}
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
	}
}