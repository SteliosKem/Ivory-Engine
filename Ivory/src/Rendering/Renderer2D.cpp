#include "pch.h"
#include "Renderer2D.h"
#include "Buffer.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "RenderCommand.h"

namespace Ivory {
	struct Renderer2DStorage {
		std::shared_ptr<VertexArray> vertex_array;
		std::shared_ptr<Shader> shader;
	};

	static Renderer2DStorage* s_data;

	void Renderer2D::init() {
		s_data = new Renderer2DStorage;
		s_data->vertex_array = VertexArray::create_array();

		float solid_vertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<VertexBuffer> solid_square_VB = std::shared_ptr<VertexBuffer>
			(VertexBuffer::create_buffer(solid_vertices, sizeof(solid_vertices)));

		BufferLayout solid_layout = {
			{ShaderDataType::Vector3, "a_position"},
		};
		solid_square_VB->set_layout(solid_layout);
		s_data->vertex_array->add_vertex_buffer(solid_square_VB);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> solid_square_IB(IndexBuffer::create_buffer(indices, sizeof(indices) / sizeof(uint32_t)));

		s_data->vertex_array->set_index_buffer(solid_square_IB);

		s_data->shader = Shader::create("C:/Projects/Ivory-Engine/Editor/Assets/shaders/flat.glsl");
	}
	void Renderer2D::shutdown() {
		delete s_data;
	}

	void Renderer2D::begin_scene(const OrthographicCamera& camera) {
		std::dynamic_pointer_cast<OpenGLShader>(s_data->shader)->bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_data->shader)->upload_uniform_mat4("u_view_projection", camera.get_vp_matrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_data->shader)->upload_uniform_mat4("u_transform", glm::mat4(1.0f));
	}
	void Renderer2D::end_scene()
	{
	}
	void Renderer2D::draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		draw_quad({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer2D::draw_quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		std::dynamic_pointer_cast<OpenGLShader>(s_data->shader)->bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_data->shader)->upload_uniform_vec4("u_color", color);

		s_data->vertex_array->bind();
		RenderCommand::draw_indexed(s_data->vertex_array);
	}
}