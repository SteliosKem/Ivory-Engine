#pragma once

namespace Ivory {
	class VertexBuffer {
	public:
		virtual ~VertexBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static VertexBuffer* create_buffer(float* vertices, uint32_t size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t get_count() = 0;

		static IndexBuffer* create_buffer(uint32_t* indeces, uint32_t size);
	};
}