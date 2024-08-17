#pragma once
#include <memory>
#include "Logging/Log.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"

namespace Ivory {
	class SceneHierarchy {
	public:
		SceneHierarchy() = default;
		SceneHierarchy(const std::shared_ptr<Scene>& scene);
		void set_context(const std::shared_ptr<Scene>& scene);

		void on_imgui_render();
		Entity get_selected() const { return m_selection_context; }
	private:
		void draw_entity_node(Entity entity);
		void draw_components(Entity entity);

		std::shared_ptr<Scene> m_context;
		Entity m_selection_context;
	};
}