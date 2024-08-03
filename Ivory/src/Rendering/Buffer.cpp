#include "pch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Ivory {
	VertexBuffer* VertexBuffer::create_buffer(float* vertices, uint32_t size) {
		switch (Renderer::get_api()) {
		case RendererAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		case RendererAPI::None:
		default:
			IV_CORE_ASSERT("Rendering API not supported");
		}
		return nullptr;
	}

	IndexBuffer* IndexBuffer::create_buffer(uint32_t* indeces, uint32_t size) {
		switch (Renderer::get_api()) {
		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indeces, size);
		case RendererAPI::None:
		default:
			IV_CORE_ASSERT("Rendering API not supported");
		}
		return nullptr;
	}

	VertexArray* VertexArray::create_array() {
		switch (Renderer::get_api()) {
		case RendererAPI::OpenGL:
			return new OpenGLVertexArray();
		case RendererAPI::None:
		default:
			IV_CORE_ASSERT("Rendering API not supported");
		}
		return nullptr;
	}
}