#pragma once
#include <string>
#include "ImGui/L2DFileDialog.h"

namespace Ivory {
	class FileDialogs {
		static std::string open_file(const std::string& _path) {
			char path[500];
			strcpy_s(path, _path.c_str());
			FileDialog::ShowFileDialog(&FileDialog::file_dialog_open, path, sizeof(path), FileDialog::FileDialogType::OpenFile);
			return std::string(path);
		}
		static std::string save_file(const std::string& _path) {
			char path[500];
			strcpy_s(path, _path.c_str());
			FileDialog::ShowFileDialog(&FileDialog::file_dialog_open, path, sizeof(path), FileDialog::FileDialogType::SelectFolder);
			return std::string(path);
		}
	};
}