#pragma once
#include "Rendering/FrameBuffer.h"

namespace Ivory {
	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		void bind() override;
		void unbind() override;

		uint32_t get_color_attachment_rendererID() const override { return m_color_attachment; }

		const FrameBufferSpecification& get_spec() const override { return m_specification; }
		FrameBufferSpecification& get_spec() override { return m_specification; }
	private:
		void resize();

		uint32_t m_color_attachment, m_depth_attachment;
		uint32_t m_rendererID;
		FrameBufferSpecification m_specification;
	};
}