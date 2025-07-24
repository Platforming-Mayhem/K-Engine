#include <K_Engine.h>

int main(int argc, char** argv)
{
	K::Application* app = new K::Application();
	app->Run("Editor");
	delete app;
	return 0;
}
