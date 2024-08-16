#pragma once
#include "Rendering/Camera.h"

namespace Ivory {
	class SceneCamera : public Camera {
	public:
		SceneCamera();
		~SceneCamera() = default;

		void set_orthographic(float size, float near_clip, float far_clip);
		void set_viewport_size(uint32_t width, uint32_t height);
	private:
		void recalculate_projection();

		float m_orthographic_size = 10.0f;
		float m_orthographic_near = -1.0f;
		float m_orthographic_far = 1.0f;

		float m_aspect_ratio{ 0.0f };
	};
}