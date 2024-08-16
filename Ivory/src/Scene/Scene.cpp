#include "pch.h"
#include "Scene.h"
#include "Rendering/Renderer2D.h"
#include "Entity.h"
#include "Components.h"

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
		m_registry.view<CScriptComponent>().each([=](auto entity, auto& cscript_component) {
			if (!cscript_component.instance) {
				cscript_component.instance = cscript_component.instantiate_script();
				cscript_component.instance->m_entity = Entity{ entity, this };
				cscript_component.instance->on_create();
			}

			cscript_component.instance->on_update(dt);
		});

		Camera* main_camera = nullptr;
		glm::mat4* camera_transform = nullptr;
		auto view = m_registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view) {
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

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

	void Scene::on_viewport_resize(uint32_t width, uint32_t height) {
		m_vp_height = height;
		m_vp_width = width;

		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& camera_component = view.get<CameraComponent>(entity);
			if (!camera_component.fixed_aspect_ratio) {
				camera_component.camera.set_viewport_size(width, height);
			}
		}
	}
}