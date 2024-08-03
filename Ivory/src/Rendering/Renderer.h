#pragma once

namespace Ivory {
	enum class RendererAPI {
		None = 0,
		OpenGL = 1,
		Direct12 = 2,
		Vulkan = 3,
		Metal = 4
	};

	class Renderer {
	public:
		inline static RendererAPI get_api() { return s_renderer_api; }
		inline static void set_api(RendererAPI api) { s_renderer_api = api; }
	private:
		static RendererAPI s_renderer_api;
	};
}