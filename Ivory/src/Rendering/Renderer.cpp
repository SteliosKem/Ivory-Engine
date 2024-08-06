#include "pch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Ivory {
	std::shared_ptr<Renderer::SceneData> Renderer::s_scene_data = std::make_shared<Renderer::SceneData>();

	void Renderer::begin_scene(OrthographicCamera& camera) {
		s_scene_data->vp_matrix = camera.get_vp_matrix();
	}

	void Renderer::end_scene() {

	}

	void Renderer::submit(const std::shared_ptr<VertexArray>& vertex_array, const std::shared_ptr<Shader>& shader, const glm::mat4& transform) {
		shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->upload_uniform_mat4("u_view_projection", s_scene_data->vp_matrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->upload_uniform_mat4("u_transform", transform);

		vertex_array->bind();
		RenderCommand::draw_indexed(vertex_array);
	}
}