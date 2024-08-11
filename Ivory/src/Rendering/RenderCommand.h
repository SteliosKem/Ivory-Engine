#pragma once
#include "RendererAPI.h"

namespace Ivory {
	class RenderCommand {
	public:
		inline static void init() { s_rendererAPI->init(); }
		inline static void set_clear_color(const glm::vec4& color) { s_rendererAPI->set_clear_color(color); }
		inline static void clear() { s_rendererAPI->clear(); }

		inline static void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_rendererAPI->set_viewport(x, y, width, height); }

		inline static void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array) {
			s_rendererAPI->draw_indexed(vertex_array);
		}
	private:
		static std::shared_ptr<RendererAPI> s_rendererAPI;
	};
}