#pragma once
#include <string>
#include <glm/glm.hpp>

namespace Ivory {
	class Shader {
	public:
		Shader(const std::string& vertex_src, const std::string& fragment_src);
		~Shader();

		void bind() const;
		void unbind() const;

		void upload_uniform_mat4(const std::string& name, const glm::mat4& matrix);
	private:
		uint32_t m_rendererID;
	};
}