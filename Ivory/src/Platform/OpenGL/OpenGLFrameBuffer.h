#pragma once
#include "Rendering/FrameBuffer.h"

namespace Ivory {
	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		void bind() override;
		void unbind() override;

		void resize(uint32_t width, uint32_t height) override;

		uint32_t get_color_attachment_rendererID() const override { return m_color_attachment; }

		const FrameBufferSpecification& get_spec() const override { return m_specification; }
		FrameBufferSpecification& get_spec() override { return m_specification; }
		void validate();
	private:
		

		uint32_t m_color_attachment{ 0 }, m_depth_attachment{ 0 };
		uint32_t m_rendererID{ 0 };
		FrameBufferSpecification m_specification;
	};
}