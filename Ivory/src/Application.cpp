#include "pch.h"
#include "Application.h"
#include "WindowEvent.h"
#include "Log.h"

namespace Ivory {
	void Application::run() {
		WindowResizeEvent e(1280, 720);

		IV_TRACE(e.to_string());

		while (true) {

		}
	}
}