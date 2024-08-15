#include "pch.h"
#include "Scene.h"
#include "Rendering/Renderer2D.h"

namespace Ivory {
	Scene::Scene() {
		entt::entity entity = m_registry.create();

	}

	entt::entity Scene::create_entity() {
		return m_registry.create();
	}

	void Scene::on_update(Timestep dt) {
		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) {
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Quad quad{};
			quad.color = sprite.color;
			quad.transform = transform;
			Renderer2D::draw_quad(quad);
		}
	}
}