#include "pch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Ivory {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_path(path) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* image_data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		IV_CORE_ASSERT(image_data, "Failed to load image " + path);
		m_width = width;
		m_height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, GL_RGBA8, m_width, m_height);

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		stbi_set_flip_vertically_on_load(0);
		stbi_image_free(image_data);
	}
	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &m_rendererID);
	}

	void OpenGLTexture2D::bind(uint32_t slot) const {
		glBindTextureUnit(slot, m_rendererID);
	}
}