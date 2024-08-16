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
	private:
		void draw_entity_node(Entity entity);

		std::shared_ptr<Scene> m_context;
		Entity m_selection_context;
	};
}