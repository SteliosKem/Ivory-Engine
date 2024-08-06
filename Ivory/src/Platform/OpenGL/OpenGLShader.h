#pragma once
#include "Rendering/Shader.h"

namespace Ivory {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertex_src, const std::string& fragment_src);
		~OpenGLShader() {}

		void bind() const override;
		void unbind() const override;

		void upload_uniform_mat4(const std::string& name, const glm::mat4& matrix);
		void upload_uniform_mat3(const std::string& name, const glm::mat3& matrix);

		void upload_uniform_int(const std::string& name, int num);

		void upload_uniform_vec4(const std::string& name, const glm::vec4& vector);
		void upload_uniform_vec3(const std::string& name, const glm::vec3& vector);
		void upload_uniform_vec2(const std::string& name, const glm::vec2& vector);
		void upload_uniform_float(const std::string& name, float num);
	private:
		uint32_t m_rendererID;
	};
}