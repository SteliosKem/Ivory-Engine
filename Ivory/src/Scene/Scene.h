#pragma once
#include "entt.hpp"
#include "Core/Timestep.h"
#include "Components.h"

namespace Ivory {
	class Scene {
	public:
		Scene();
		~Scene() {}

		entt::entity create_entity();

		entt::registry& get_reg() { return m_registry; }

		void on_update(Timestep dt);
	private:
		entt::registry m_registry;
	};
}