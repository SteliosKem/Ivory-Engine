#include "pch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Ivory {
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec) :m_specification(spec) {
		validate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer() {
		glDeleteFramebuffers(1, &m_rendererID);
		glDeleteTextures(1, &m_color_attachment);
		glDeleteTextures(1, &m_depth_attachment);
	}

	void OpenGLFrameBuffer::validate() {
		if (m_rendererID) {
			glDeleteFramebuffers(1, &m_rendererID);
			glDeleteTextures(1, &m_color_attachment);
			glDeleteTextures(1, &m_depth_attachment);
		}
		glCreateFramebuffers(1, &m_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_color_attachment);
		glBindTexture(GL_TEXTURE_2D, m_color_attachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specification.width, m_specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_depth_attachment);
		glBindTexture(GL_TEXTURE_2D, m_depth_attachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment, 0);
		IV_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		glViewport(0, 0, m_specification.width, m_specification.height);
	}

	void OpenGLFrameBuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::resize(uint32_t width, uint32_t height) {
		m_specification.width = width;
		m_specification.height = height;
		validate();
	}
}