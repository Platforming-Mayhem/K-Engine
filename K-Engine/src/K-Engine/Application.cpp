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
			glfwPollEvents();

			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR)
			{
				std::cout << gluErrorString(err) << std::endl;
			}

			int esc = glfwGetKey(window->window, GLFW_KEY_ESCAPE);

			if (esc == GLFW_PRESS)
			{
				break;
			}

			#if _DEBUG
			editor->GetViewport()->Bind();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(material->GetShader()->shader);

			K::SceneManager::currentScene->Render();

			editor->GetViewport()->Unbind();

			if (editor->Render())
			{
				break;
			}
			#else
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(material->GetShader()->shader);

			K::SceneManager::currentScene->Render();

			glUseProgram(0);
			#endif

			K::SceneManager::Update();

			glfwSwapBuffers(window->window);
		}
		delete editor;
		delete manager;
	}
}