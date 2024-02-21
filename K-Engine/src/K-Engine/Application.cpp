#include "Application.h"

namespace K 
{
	K::Window* window;

	Application::Application() 
	{
		
	}
	Application::~Application()
	{

	}

	void Application::Run(const char* windowName)
	{
		window = new K::Window(windowName);

		K::Material* material = new K::Material();

		K::SceneManager* manager = new K::SceneManager(BUILD_SCENES);

		K::Editor* editor = new K::Editor(window, manager, material);

		K::SceneManager::LoadScene(0);

		K::SceneManager::Update();

		std::cout << "Number Of Scenes in Build :" << manager->GetNumberOfScenes() << std::endl;

		while (!glfwWindowShouldClose(window->window))
		{
			int esc = glfwGetKey(window->window, GLFW_KEY_ESCAPE);

			if (esc == GLFW_PRESS)
			{
				break;
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(material->GetShader()->shader);

			K::SceneManager::currentScene->Render();

			#if _DEBUG
			if (editor->Render())
			{
				break;
			}
			#endif

			K::SceneManager::Update();

			glfwSwapBuffers(window->window);

			glfwPollEvents();
		}
		delete editor;
		delete manager;
	}
}