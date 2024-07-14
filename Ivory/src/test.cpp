#include <iostream>
#include "test.h"

namespace Ivory {
	__declspec(dllexport) void out(const char* message) {
		std::cout << message;
	}
}