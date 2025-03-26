#include <K_Engine.h>
#ifdef _WIN32
	#include <Windows.h>
#elif __unix__
	#include <dlfcn.h>
#endif

void main(int argc, char** argv)
{
	#ifdef _WIN32
	HINSTANCE hInst = LoadLibrary(TEXT("Components"));

	if (hInst != NULL)
	{
		std::cout << "Found Library" << std::endl;
	}
	else 
	{
		std::cout << "No Library Found" << std::endl;
	}

	#elif __unix__
	void* handle = dlopen("Components.so", RTLD_NOW)
	if (handle) 
	{
		std::cout << "Found Library" << std::endl;
	}
	#endif

	K::Application* app = new K::Application();
	app->Run("Editor");

	#ifdef _WIN32
	FreeLibrary(hInst);
	#elif __unix__
	dlclose(handle);
	#endif
	delete app;
}