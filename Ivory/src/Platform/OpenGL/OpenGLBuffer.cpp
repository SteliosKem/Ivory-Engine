#include "pch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Ivory {
	static GLenum shader_to_opengl_type(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Vector3: return GL_FLOAT;
		case ShaderDataType::Vector4: return GL_FLOAT;
		}
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
		glCreateBuffers(1, &m_rendererID);
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1, &m_rendererID);
	}

	void OpenGLVertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	}

	void OpenGLVertexBuffer::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// INDEX BUFFER
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indeces, uint32_t count) : m_count(count) {
		glCreateBuffers(1, &m_rendererID);
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indeces, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		glDeleteBuffers(1, &m_rendererID);
	}

	void OpenGLIndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
	}

	void OpenGLIndexBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// VERTEX ARRAY

	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(2, &m_rendererID);
	}

	void OpenGLVertexArray::bind() const {
		glBindVertexArray(m_rendererID);
	}

	void OpenGLVertexArray::unbind() const {
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vertex_buffer) {
		IV_CORE_ASSERT(vertex_buffer->get_layout().get_elements().size(), "Vertex buffer has no layout");

		glBindVertexArray(m_rendererID);
		vertex_buffer->bind();

		uint32_t i = 0;
		for (const BufferElement& element : vertex_buffer->get_layout()) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.get_count(), shader_to_opengl_type(element.shader_type),
				element.normalized ? GL_TRUE : GL_FALSE, vertex_buffer->get_layout().get_stride(), (const void*)element.offset);
			i++;
		}
		m_vertex_buffers.push_back(vertex_buffer);
	}

	void OpenGLVertexArray::set_index_buffer(const std::shared_ptr<IndexBuffer>& index_buffer) {
		glBindVertexArray(m_rendererID);
		index_buffer->bind();

		m_index_buffer = index_buffer;
	}
}
