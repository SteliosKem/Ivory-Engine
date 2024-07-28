#include "pch.h"
#include "Application.h"
#include "WindowEvent.h"
#include "KeyEvent.h"
#include "Log.h"

namespace Ivory {
	void Application::run() {
		WindowResizeEvent e(1280, 720);
		KeyPressedEvent _e(2, 1);

		IV_TRACE(e.to_string());
		IV_WARN(_e.to_string());

		while (true) {

		}
	}
}