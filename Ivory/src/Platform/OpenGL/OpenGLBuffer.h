#pragma once
#include "Rendering/Buffer.h"
#include <cstdint>

namespace Ivory {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		void bind() const;
		void unbind() const;
	private:
		uint32_t m_rendererID;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indeces, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		uint32_t get_count() { return m_count; }

		void bind() const;
		void unbind() const;
	private:
		uint32_t m_rendererID;
		uint32_t m_count;
	};
}