#pragma once
#include <string>
#include "ImGui/L2DFileDialog.h"

namespace Ivory {
	class FileDialogs {
	public:
		static void open_file(const std::string& _path, std::string& out) {

			static char path[500];
			if (_path.empty()) {

				strcpy_s(path, _path.c_str());
				FileDialog::ShowFileDialog(&FileDialog::file_dialog_open, path, sizeof(path), FileDialog::FileDialogType::OpenFile);
				out = path;
			}
		}
		static void save_file(const std::string& _path, std::string& out) {
			static char path[500];
			if (_path.empty()) {
				
				strcpy_s(path, _path.c_str());
				FileDialog::ShowFileDialog(&FileDialog::file_dialog_open, path, sizeof(path), FileDialog::FileDialogType::SelectFolder);
				out = path;
			}
		}
	};
}