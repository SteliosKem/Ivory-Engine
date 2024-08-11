#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "Buffer.h"

namespace Ivory {
	class RendererAPI {
	public:
		enum class API {
			None = 0,
			OpenGL = 1
		};

		virtual void init() = 0;

		virtual void set_clear_color(const glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array) = 0;

		inline static API get_api() { return s_api; }
	private:
		static API s_api;
	};
}