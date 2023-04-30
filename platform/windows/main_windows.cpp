#if MAIN_ENABLED
#include <core/language_features.hpp>

#if SYSTEM_WINDOWS
#include <main/main.hpp>
#include <platform/windows/os_windows.hpp>

#pragma comment(linker, "/manifestdependency:\"name='binaries' version='1.0.0.0' type='win32'\"")

int _main()
{
	using namespace pn;

	OS_Windows os{ (HINSTANCE)nullptr };

	switch (Main::setup(__argv[0], __argc, __argv)) {
	case Error_OK: break;
	/* TODO: additional error checking */
	case Error_Unknown:
	default: {
		CRASH("An unknown error occurred during setup. The program was unable to start.");
	} break;
	}

	if (Main::start()) { os.run(); }

	Main::cleanup();

	return os.get_exit_code();
}

int main(int argc, char * argv[])
{
	return _main();
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	return main(0, nullptr);
}

#endif // !SYSTEM_WINDOWS
#endif // !MAIN_ENABLED