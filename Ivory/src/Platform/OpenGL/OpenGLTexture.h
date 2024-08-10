#pragma once

#include "Rendering/Texture.h"

namespace Ivory {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D();

		uint32_t get_width() const override { return m_width; }
		uint32_t get_height() const override { return m_height; }

		void bind(uint32_t slot = 0) const override;
	private:
		std::string m_path;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_rendererID;
	};
}