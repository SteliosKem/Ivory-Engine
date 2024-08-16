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
				if (ImGui::DragFloat3("Scale", glm::value_ptr(transform.transform[3]), 0.1f)) {

				}
				if (ImGui::DragFloat3("Position", glm::value_ptr(transform.transform[3]), 0.1f)) {

				}
				ImGui::TreePop();
			}
		}
	}
}