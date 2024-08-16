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
		void on_viewport_resize(uint32_t width, uint32_t height);

		void on_update(Timestep dt);
	private:
		entt::registry m_registry;
		uint32_t m_vp_width{ 0 }, m_vp_height{ 0 };

		friend class Entity;
	};
}