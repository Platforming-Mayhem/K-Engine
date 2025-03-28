#include <K_Engine.h>

void main(int argc, char** argv)
{
	K::Application* app = new K::Application();
	app->Run("Editor");
	delete app;
}