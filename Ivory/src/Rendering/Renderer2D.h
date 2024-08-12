#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"

namespace Ivory {
	struct TextureInfo {
		float tiling_factor = 1.0f;
	};

	struct Quad {
		glm::vec3 position{0.0f, 0.0f, 0.0f};
		glm::vec2 size{ 1.0f, 1.0f };
		float rotation{ 0.0f };
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		std::shared_ptr<Texture2D> texture{ nullptr };
		TextureInfo texture_info;
	};

	class Renderer2D {
	public:
		static void init();
		static void shutdown();

		static void begin_scene(const OrthographicCamera& camera);
		static void end_scene();

		/*static void draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void draw_quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void draw_quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture);
		static void draw_quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture);
		static void draw_quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, const glm::vec4& color);
		static void draw_quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, const glm::vec4& color);

		static void draw_rotated_quad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture);
		static void draw_rotated_quad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void draw_rotated_quad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, const glm::vec4& color);*/

		static void draw_quad(const Quad& quad);
	};
}