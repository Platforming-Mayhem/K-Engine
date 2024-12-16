#include <K-Engine/Application.h>

int main(int argc, char** argv)
{
	K::Application* app = new K::Application();
	app->Run("K-Engine Editor");
	delete app;
}