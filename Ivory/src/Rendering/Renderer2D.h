#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture.h"

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
		std::shared_ptr<SubTexture2D> sub_texture{ nullptr };
		TextureInfo texture_info;
	};

	class Renderer2D {
	public:
		static void init();
		static void shutdown();

		static void begin_scene(const OrthographicCamera& camera);
		static void end_scene();

		static void flush();

		static void draw_quad(const Quad& quad);


		struct Statistics {
			uint32_t draw_calls = 0;
			uint32_t quad_count = 0;

			uint32_t get_vertex_count() { return quad_count * 4; }
			uint32_t get_index_count() { return quad_count * 6; }
		};
		static void reset_stats();
		static const Statistics& get_stats();
	private:
		static void new_batch();
	};
}