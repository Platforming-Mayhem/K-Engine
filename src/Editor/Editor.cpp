#include <K_Engine.h>
#ifdef _WIN32
	#include <Windows.h>
#elif __unix__
	#include <dlfcn.h>
#endif

void main(int argc, char** argv)
{
	K::Application* app = new K::Application();

	std::system("\"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\Tools\\VsDevCmd.bat\" && cd .. && echo %cd% && msbuild Components.vcxproj");

	#ifdef _WIN32
	HINSTANCE hInst = LoadLibrary(TEXT("Components"));

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

	#ifdef _WIN32
	FreeLibrary(hInst);
	#elif __unix__
	dlclose(handle);
	#endif
	delete app;
}