#include "ContentBrowser.h"
#include <string>

#include "imgui.h"

namespace Ivory {
	

	void ContentBrowser::on_imgui_render() {
		ImGui::Begin("Content Browser");

		if (m_current_dir != m_assets_dir) {
			if (ImGui::Button("Back")) {
				m_current_dir = m_current_dir.parent_path();
			}
		}

		for (auto& p : std::filesystem::directory_iterator(m_current_dir)) {
			//std::string path = p.path().string().c_str();
			auto relative_path = std::filesystem::relative(p.path(), m_assets_dir);
			std::string name = relative_path.filename().string();

			if (p.is_directory()) {
				if (ImGui::Button(name.c_str())) {
					m_current_dir /= p.path().filename();
				}
			}
			else {
				if (ImGui::Button(name.c_str()));
			}
		}
		ImGui::End();
	}
}