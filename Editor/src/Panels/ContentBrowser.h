#pragma once
#include <filesystem>

namespace Ivory {
	class ContentBrowser {
	public:
		ContentBrowser() = default;
		void on_imgui_render();
	private:
		std::filesystem::path m_assets_dir = "Assets";
		std::filesystem::path m_current_dir = m_assets_dir;
		
	};
}