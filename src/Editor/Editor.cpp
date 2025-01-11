#include <K_Engine.h>
#include <Windows.h>

void main(int argc, char** argv)
{
	K::Application* app = new K::Application();

	HINSTANCE hInst = LoadLibrary(TEXT("Components.dll"));

	if (hInst != NULL) 
	{
		std::cout << "Found Library" << std::endl;
	}

	std::string name = argv[0];
	app->Run(name.substr(name.find_last_of('\\') + 1).c_str());
	FreeLibrary(hInst);
	delete app;
}