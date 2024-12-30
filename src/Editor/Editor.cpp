#include <K_Engine.h>

void main(int argc, char** argv)
{
	K::Application* app = new K::Application();
	std::string name = argv[0];
	app->Run(name.substr(name.find_last_of('\\') + 1).c_str());
	delete app;
}