#pragma once
#include <glm/glm.hpp>
#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Ivory {
	struct TagComponent {
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& str) : tag(str) {}
	};

	struct TransformComponent {
		glm::vec3 translation{ 0.0f };
		glm::vec3 rotation{ 0.0f };
		glm::vec3 scale{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation) : translation(translation) {}

		glm::mat4 get_transform() const {
			glm::mat4 _rotation = glm::toMat4(glm::quat(rotation));

			return glm::translate(glm::mat4{ 1.0f }, translation) * _rotation * glm::scale(glm::mat4{ 1.0f }, scale);
		}
	};

	struct SpriteRendererComponent {
		glm::vec4 color{ 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : color(color) {}
	};

	struct CameraComponent {
		SceneCamera camera;
		bool active = true;
		bool fixed_aspect_ratio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct CScriptComponent {
		ScriptableEntity* instance = nullptr;

		ScriptableEntity* (*instantiate_script)();
		void (*destroy_script)(CScriptComponent*);

		template<typename T>
		void bind() {
			instantiate_script = []() {return static_cast<ScriptableEntity*>(new T()); };
			destroy_script = [](CScriptComponent* script_component) {delete script_component->instance; script_component->instance = nullptr; };
		}
	};
}