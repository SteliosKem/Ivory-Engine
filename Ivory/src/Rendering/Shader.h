#pragma once
#include <string>
#include <glm/glm.hpp>

namespace Ivory {
	class Shader {
	public:
		virtual ~Shader() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static Shader* create(const std::string& file_path);
		static Shader* create(const std::string& vertex_src, const std::string& fragment_src);
	private:
		uint32_t m_rendererID;
	};
}