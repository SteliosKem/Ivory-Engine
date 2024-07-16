#pragma once

#ifdef IV_PLATFORM_WINDOWS

extern Ivory::Application* Ivory::create_application();

int main(int argc, char** argv) {
	std::cout << "Starting Engine";
	auto app = Ivory::create_application();
	app->run();
	delete app;
}

#endif