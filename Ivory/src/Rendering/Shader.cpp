#include "pch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Ivory {
	Shader* Shader::create(const std::string& file_path) {
		switch (Renderer::get_api()) {
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(file_path);
		case RendererAPI::API::None:
		default:
			IV_CORE_ASSERT("Rendering API not supported");
		}
		return nullptr;
	}

	Shader* Shader::create(const std::string& vertex_src, const std::string& fragment_src) {
		switch (Renderer::get_api()) {
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertex_src, fragment_src);
		case RendererAPI::API::None:
		default:
			IV_CORE_ASSERT("Rendering API not supported");
		}
		return nullptr;
	}
}