#include "pch.h"
#include "Renderer2D.h"
#include "Buffer.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "RenderCommand.h"

namespace Ivory {
	struct QuadVertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texture_coord;
		float texture_index;
	};

	struct Renderer2DStorage {
		const uint32_t max_quads = 10000;
		const uint32_t max_vertices = 10000 * 4;
		const uint32_t max_indices = 10000 * 6;
		static const uint32_t max_texture_slots = 32;

		std::shared_ptr<VertexArray> quad_vertex_array;
		std::shared_ptr<VertexBuffer> quad_vertex_buffer;
		std::shared_ptr<Shader> texture_shader;
		std::shared_ptr<Texture2D> white_texture;

		uint32_t quad_index_count = 0;
		QuadVertex* quad_vertex_buffer_base = nullptr;
		QuadVertex* quad_vertex_buffer_ptr = nullptr;

		std::array<std::shared_ptr<Texture2D>, max_texture_slots> texture_slots;
		uint32_t texture_slot_index = 1;
	};

	static Renderer2DStorage s_data;


	void Renderer2D::init() {
		s_data.quad_vertex_array = VertexArray::create_array();

		s_data.quad_vertex_buffer = VertexBuffer::create_buffer(s_data.max_vertices * sizeof(QuadVertex));

		BufferLayout solid_layout = {
			{ShaderDataType::Vector3, "a_position"},
			{ShaderDataType::Vector4, "a_color"},
			{ShaderDataType::Vector2, "a_texture_coord"},
			{ShaderDataType::Float, "a_texture_index"},
		};
		s_data.quad_vertex_buffer->set_layout(solid_layout);
		s_data.quad_vertex_array->add_vertex_buffer(s_data.quad_vertex_buffer);

		s_data.quad_vertex_buffer_base = new QuadVertex[s_data.max_vertices];

		uint32_t* quad_indices = new uint32_t[s_data.max_indices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_data.max_indices; i += 6) {
			quad_indices[i] = offset;
			quad_indices[i + 1] = offset + 1;
			quad_indices[i + 2] = offset + 2;
			quad_indices[i + 3] = offset + 2;
			quad_indices[i + 4] = offset + 3;
			quad_indices[i + 5] = offset + 0;

			offset += 4;
		}

		std::shared_ptr<IndexBuffer> quad_IB(IndexBuffer::create_buffer(quad_indices, s_data.max_indices));
		s_data.quad_vertex_array->set_index_buffer(quad_IB);
		delete[] quad_indices;

		s_data.white_texture = Texture2D::create(1, 1);
		uint32_t white_data = 0xffffffff;
		s_data.white_texture->set_data(&white_data, sizeof(uint32_t));
		
		int32_t samplers[s_data.max_texture_slots];
		for (int i = 0; i < s_data.max_texture_slots; i++)
			samplers[i] = i;

		s_data.texture_shader = Shader::create("C:/Projects/Ivory-Engine/Editor/Assets/shaders/shader.glsl");
		s_data.texture_shader->bind();
		s_data.texture_shader->set_int_array("u_textures", samplers, s_data.max_texture_slots);

		s_data.texture_slots[0] = s_data.white_texture;
	}
	void Renderer2D::shutdown() {
	}

	void Renderer2D::begin_scene(const OrthographicCamera& camera) {
		//s_data->color_shader->bind();
		//s_data->color_shader->set_mat4("u_view_projection", camera.get_vp_matrix());
		
		s_data.texture_shader->bind();
		s_data.texture_shader->set_mat4("u_view_projection", camera.get_vp_matrix());

		s_data.quad_index_count = 0;
		s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;

		s_data.texture_slot_index = 1;
	}
	void Renderer2D::end_scene()
	{
		uint32_t data_size = (uint8_t*)s_data.quad_vertex_buffer_ptr - (uint8_t*)s_data.quad_vertex_buffer_base;
		s_data.quad_vertex_buffer->set_data(s_data.quad_vertex_buffer_base, data_size);

		flush();
	}

	void Renderer2D::flush() {
		for (uint32_t i = 0; i < s_data.texture_slot_index; i++) {
			s_data.texture_slots[i]->bind(i);
		}

		RenderCommand::draw_indexed(s_data.quad_vertex_array, s_data.quad_index_count);
	}
	/*void Renderer2D::draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		draw_quad({ position.x, position.y, 0.0f }, size, color);
	}
	

	void Renderer2D::draw_rotated_quad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		//s_data->color_shader->bind();
		s_data->texture_shader->set_vec4("u_color", color);
		s_data->texture_shader->set_float("u_tiling_factor", 1.0f);
		s_data->white_texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->texture_shader->set_mat4("u_transform", transform);

		s_data->vertex_array->bind();
		RenderCommand::draw_indexed(s_data->vertex_array);
	}

	void Renderer2D::draw_quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture) {
		draw_quad(glm::vec3{ position.x, position.y, 0.0f }, size, texture);
	}
	void Renderer2D::draw_quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture) {
		s_data->texture_shader->set_vec4("u_color", {1.0f, 1.0f, 1.0f, 1.0f});
		s_data->texture_shader->set_float("u_tiling_factor", 1.0f);
		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->texture_shader->set_mat4("u_transform", transform);

		s_data->vertex_array->bind();
		RenderCommand::draw_indexed(s_data->vertex_array);
	}

	

	void Renderer2D::draw_quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, const glm::vec4& color) {
		draw_quad(glm::vec3{ position.x, position.y, 0.0f }, size, texture, color);
	}
	void Renderer2D::draw_quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, const glm::vec4& color) {
		s_data->texture_shader->set_vec4("u_color", color);
		s_data->texture_shader->set_float("u_tiling_factor", 1.0f);
		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->texture_shader->set_mat4("u_transform", transform);

		s_data->vertex_array->bind();
		RenderCommand::draw_indexed(s_data->vertex_array);
	}

	void Renderer2D::draw_rotated_quad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, const glm::vec4& color) {
		s_data->texture_shader->set_vec4("u_color", color);
		s_data->texture_shader->set_float("u_tiling_factor", 1.0f);
		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->texture_shader->set_mat4("u_transform", transform);

		s_data->vertex_array->bind();
		RenderCommand::draw_indexed(s_data->vertex_array);
	}*/

	void Renderer2D::draw_quad(const Quad& quad) {
		float texture_index = 0.0f;

		if (quad.texture) {
			for (uint32_t i = 1; i < s_data.texture_slot_index; i++) {
				if (*s_data.texture_slots[i].get() == *quad.texture.get()) {
					texture_index = (float)i;
					break;
				}
			}

			if (texture_index == 0.0f) {
				texture_index = s_data.texture_slot_index;
				s_data.texture_slots[s_data.texture_slot_index] = quad.texture;
				s_data.texture_slot_index++;
			}
		}


		s_data.quad_vertex_buffer_ptr->position = quad.position;
		s_data.quad_vertex_buffer_ptr->color = quad.color;
		s_data.quad_vertex_buffer_ptr->texture_coord = { 0.0f, 0.0f };
		s_data.quad_vertex_buffer_ptr->texture_index = texture_index;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = { quad.position.x + quad.size.x, quad.position.y, 0.0f };
		s_data.quad_vertex_buffer_ptr->color = quad.color;
		s_data.quad_vertex_buffer_ptr->texture_coord = { 1.0f, 0.0f };
		s_data.quad_vertex_buffer_ptr->texture_index = texture_index;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = { quad.position.x + quad.size.x, quad.position.y + quad.size.y, 0.0f };
		s_data.quad_vertex_buffer_ptr->color = quad.color;
		s_data.quad_vertex_buffer_ptr->texture_coord = { 1.0f, 1.0f };
		s_data.quad_vertex_buffer_ptr->texture_index = texture_index;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = { quad.position.x, quad.position.y + quad.size.y, 0.0f };
		s_data.quad_vertex_buffer_ptr->color = quad.color;
		s_data.quad_vertex_buffer_ptr->texture_coord = { 0.0f, 1.0f };
		s_data.quad_vertex_buffer_ptr->texture_index = texture_index;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_index_count += 6;

		/*s_data.texture_shader->set_float("u_tiling_factor", quad.texture_info.tiling_factor);
		quad.texture ? quad.texture->bind() : s_data.white_texture->bind();

		glm::mat4 transform(1.0f);
		quad.rotation ?
			transform = glm::translate(glm::mat4(1.0f), quad.position) * glm::rotate(glm::mat4(1.0f), quad.rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { quad.size.x, quad.size.y, 1.0f })
			: transform = glm::translate(glm::mat4(1.0f), quad.position) * glm::scale(glm::mat4(1.0f), { quad.size.x, quad.size.y, 1.0f });
		s_data.texture_shader->set_mat4("u_transform", transform);

		s_data.quad_vertex_array->bind();
		RenderCommand::draw_indexed(s_data.quad_vertex_array);*/
	}

	/*void Renderer2D::draw_quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		s_data.quad_vertex_buffer_ptr->position = position;
		s_data.quad_vertex_buffer_ptr->color = color;
		s_data.quad_vertex_buffer_ptr->texture_coord = { 0.0f, 0.0f };
		s_data.quad_vertex_buffer_ptr->texture_index = 0.0f;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = { position.x + size.x, position.y, 0.0f };
		s_data.quad_vertex_buffer_ptr->color = color;
		s_data.quad_vertex_buffer_ptr->texture_coord = { 1.0f, 0.0f };
		s_data.quad_vertex_buffer_ptr->texture_index = 0.0f;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = { position.x + size.x, position.y + size.y, 0.0f };
		s_data.quad_vertex_buffer_ptr->color = color;
		s_data.quad_vertex_buffer_ptr->texture_coord = { 1.0f, 1.0f };
		s_data.quad_vertex_buffer_ptr->texture_index = 0.0f;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = { position.x, position.y + size.y, 0.0f };
		s_data.quad_vertex_buffer_ptr->color = color;
		s_data.quad_vertex_buffer_ptr->texture_coord = { 0.0f, 1.0f };
		s_data.quad_vertex_buffer_ptr->texture_index = 0.0f;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_index_count += 6;
	}

	void Renderer2D::draw_quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture) {
		float texture_index = 0.0f;

		for (uint32_t i = 1; i < s_data.texture_slot_index; i++) {
			if (*s_data.texture_slots[i].get() == *texture.get()) {
				texture_index = (float)i;
				break;
			}
		}

		if (texture_index == 0.0f) {
			texture_index = s_data.texture_slot_index;
			s_data.texture_slots[s_data.texture_slot_index] = texture;
			s_data.texture_slot_index++;
		}


		s_data.quad_vertex_buffer_ptr->position = position;
		s_data.quad_vertex_buffer_ptr->color = { 1.0f, 1.0f, 1.0f, 1.0f, };
		s_data.quad_vertex_buffer_ptr->texture_coord = { 0.0f, 0.0f };
		s_data.quad_vertex_buffer_ptr->texture_index = texture_index;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = { position.x + size.x, position.y, 0.0f };
		s_data.quad_vertex_buffer_ptr->color = { 1.0f, 1.0f, 1.0f, 1.0f, };
		s_data.quad_vertex_buffer_ptr->texture_coord = { 1.0f, 0.0f };
		s_data.quad_vertex_buffer_ptr->texture_index = texture_index;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = { position.x + size.x, position.y + size.y, 0.0f };
		s_data.quad_vertex_buffer_ptr->color = { 1.0f, 1.0f, 1.0f, 1.0f, };
		s_data.quad_vertex_buffer_ptr->texture_coord = { 1.0f, 1.0f };
		s_data.quad_vertex_buffer_ptr->texture_index = texture_index;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_vertex_buffer_ptr->position = { position.x, position.y + size.y, 0.0f };
		s_data.quad_vertex_buffer_ptr->color = { 1.0f, 1.0f, 1.0f, 1.0f, };
		s_data.quad_vertex_buffer_ptr->texture_coord = { 0.0f, 1.0f };
		s_data.quad_vertex_buffer_ptr->texture_index = texture_index;
		s_data.quad_vertex_buffer_ptr++;

		s_data.quad_index_count += 6;
	}*/
}