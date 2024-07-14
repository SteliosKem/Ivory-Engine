namespace Ivory {
	__declspec(dllimport) void out(const char* message);
}

void main() {
	Ivory::out("Starting up");
}