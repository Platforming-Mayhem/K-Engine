#include "K-Engine/Application.h"
#include <K_Engine.h>

namespace K 
{
	Application::Application() 
	{
		
	}
	Application::~Application()
	{

	}

	void Application::Run(const char* windowName)
	{
		new K::Window(windowName);

		#if _DEBUG
		K::editorMat = new Material(EDITOR_SHADER);
		K::renderTex = new K::RenderTexture(K::window->width, K::window->height, GL_TEXTURE_2D, GL_R32I, GL_RED_INTEGER);
		#endif

		K::SceneManager* manager = new K::SceneManager("");

		K::Editor* editor = new K::Editor(K::window, manager);

		K::SceneManager::LoadScene(0);

		K::SceneManager::Update();

		std::cout << "Number Of Scenes in Build :" << manager->GetNumberOfScenes() << std::endl;

		while (true)
		{
			#if _DEBUG
			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR)
			{
				std::cout << gluErrorString(err) << std::endl;
			}
			#endif

			#if _DEBUG
			editor->GetViewport()->Bind();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			K::SceneManager::currentScene->Render();

			editor->GetViewport()->Unbind();

			if (editor->Render())
			{
				break;
			}
			#else

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			K::SceneManager::currentScene->Render();

			#endif

			int esc = glfwGetKey(K::window->window, GLFW_KEY_ESCAPE);

			if (esc == GLFW_PRESS || glfwWindowShouldClose(K::window->window))
			{
				K::SceneManager::Quit();
			}

			glfwSwapBuffers(K::window->window);

			glfwPollEvents();

			if (K::SceneManager::Update())
			{
				break;
			}
		}
		#if _DEBUG
		delete K::editorMat;
		delete K::renderTex;
		#endif
		delete manager;
		delete editor;
	}
}