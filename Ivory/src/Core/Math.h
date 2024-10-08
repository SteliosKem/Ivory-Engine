#pragma once
#include <glm/glm.hpp>

namespace Ivory {
	bool decompose_transform(const glm::mat4& transform, glm::vec3& out_translation, glm::vec3& out_rotation, glm::vec3& out_size);
}