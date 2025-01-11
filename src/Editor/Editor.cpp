#include <K_Engine.h>
#ifdef _WIN32 || _WIN64
	#include <Windows.h>
#elif __unix__
	#include <dlfcn.h>
#endif

void main(int argc, char** argv)
{
	K::Application* app = new K::Application();

	#ifdef _WIN32 || _WIN64
	HINSTANCE hInst = LoadLibrary(TEXT("Components.dll"));

	if (hInst != NULL)
	{
		std::cout << "Found Library" << std::endl;
	}
	#elif __unix__
	void* handle = dlopen("Components.so", RTLD_NOW)
	if (handle) 
	{
		std::cout << "Found Library" << std::endl;
	}
	#endif

	std::string name = argv[0];
	app->Run(name.substr(name.find_last_of('\\') + 1).c_str());

	#ifdef _WIN32 || _WIN64
	FreeLibrary(hInst);
	#elif __unix__
	dlclose(handle);
	#endif
	delete app;
}