#pragma once
#include "entt.hpp"

namespace Ivory {
	class Scene {
	public:
		Scene();
		~Scene();
	private:
		entt::registry m_registry;
	};
}