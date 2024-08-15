#include "pch.h"
#include "Scene.h"
#include "Rendering/Renderer2D.h"
#include "Entity.h"

namespace Ivory {
	Scene::Scene() {
		entt::entity entity = m_registry.create();

	}

	Entity Scene::create_entity(const std::string& name) {
		Entity e{ m_registry.create(), this };
		e.add_component<TransformComponent>();
		e.add_component<TagComponent>(name.empty() ? "New Entity" : name);
		return e;
	}

	void Scene::on_update(Timestep dt) {
		Camera* main_camera = nullptr;
		glm::mat4* camera_transform = nullptr;
		auto view = m_registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view) {
			auto&[transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

			if (camera.active) {
				main_camera = &camera.camera;
				camera_transform = &transform.transform;
				break;
			}
		}

		if (main_camera) {
			Renderer2D::begin_scene(main_camera->get_projection(), *camera_transform);

			auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group) {
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Quad quad{};
				quad.color = sprite.color;
				quad.transform = transform;
				Renderer2D::draw_quad(quad);
			}

			Renderer2D::end_scene();
		}
	}
}