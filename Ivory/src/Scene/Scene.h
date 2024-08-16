#pragma once
#include "entt.hpp"
#include "Core/Timestep.h"

namespace Ivory {
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene() {}

		Entity create_entity(const std::string& name = "");
		void destroy_entity(Entity entity);
		void on_viewport_resize(uint32_t width, uint32_t height);

		void on_update(Timestep dt);
	private:
		template<typename T>
		void on_component_add(Entity entity, T& component);

		entt::registry m_registry;
		uint32_t m_vp_width{ 1 }, m_vp_height{ 1 };

		friend class Entity;
		friend class SceneHierarchy;
	};
}