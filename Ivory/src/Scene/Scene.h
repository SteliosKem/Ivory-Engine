#pragma once
#include "entt.hpp"
#include "Core/Timestep.h"
#include "Components.h"

namespace Ivory {
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene() {}

		Entity create_entity(const std::string& name = "");

		void on_update(Timestep dt);
	private:
		entt::registry m_registry;

		friend class Entity;
	};
}