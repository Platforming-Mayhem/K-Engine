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
		K::window = new K::Window(windowName);

		K::editorMat = new Material("shaders/editor.shader");

		K::renderTex = new K::RenderTexture(K::window->width, K::window->height, GL_TEXTURE_2D, GL_R32I, GL_RED_INTEGER);

		K::SceneManager* manager = new K::SceneManager(BUILD_SCENES);

		K::Editor* editor = new K::Editor(K::window, manager);

		K::SceneManager::LoadScene(0);

		K::SceneManager::Update();

		std::cout << "Number Of Scenes in Build :" << manager->GetNumberOfScenes() << std::endl;

		while (!glfwWindowShouldClose(K::window->window))
		{
			#if _DEBUG
			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR)
			{
				std::cout << gluErrorString(err) << std::endl;
			}
			#endif

			int esc = glfwGetKey(K::window->window, GLFW_KEY_ESCAPE);

			if (esc == GLFW_PRESS)
			{
				break;
			}

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

			if (K::SceneManager::Update()) 
			{
				break;
			}

			glfwSwapBuffers(K::window->window);

			glfwPollEvents();
		}
		delete K::editorMat;
		delete K::renderTex;
		delete manager;
		delete editor;
	}
}