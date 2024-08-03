#pragma once
#include "Rendering/Buffer.h"
#include <cstdint>

namespace Ivory {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		void bind() const override;
		void unbind() const override;

		void set_layout(const BufferLayout& layout) { m_layout = layout; }
		const BufferLayout& get_layout() { return m_layout; }
	private:
		uint32_t m_rendererID;
		BufferLayout m_layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indeces, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		uint32_t get_count() { return m_count; }

		void bind() const override;
		void unbind() const override;
	private:
		uint32_t m_rendererID;
		uint32_t m_count;
	};
}