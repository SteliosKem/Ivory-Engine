#pragma once
#include "Scene.h"
#include "entt.hpp"

namespace Ivory {
	class Entity {
	public:
		Entity(entt::entity handle, Scene* scene) : m_entity_handle(handle), m_scene(scene) {}
		Entity(const Entity& other) = default;
		Entity() = default;

		template<typename T, typename... Args>
		T& add_component(Args&&... args) {
			IV_CORE_ASSERT(has_component<T>(), "Entity does not have component");

			return m_scene->m_registry.emplace<T>(m_entity_handle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& get_component() {
			IV_CORE_ASSERT(!has_component<T>(), "Entity already has component");

			return m_scene->m_registry.get<T>(m_entity_handle);
		}

		template<typename T>
		bool has_component() {
			return m_scene->m_registry.all_of<T>(m_entity_handle);
		}

		template<typename T>
		bool remove_component() {
			IV_CORE_ASSERT(has_component<T>(), "Entity does not have component");

			return m_scene->m_registry.remove<T>(m_entity_handle);
		}

		operator bool() const { return m_entity_handle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_entity_handle; }
		bool operator==(const Entity& other) const { return m_entity_handle == other.m_entity_handle && m_scene == other.m_scene; }
		bool operator!=(const Entity& other) const { return (*this == other); }
	private:
		entt::entity m_entity_handle{ entt::null };
		Scene* m_scene{ nullptr };
	};

}