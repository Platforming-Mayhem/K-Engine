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

		#if _DEBUG
			K::Transform* camPosition = new K::Transform(new Vector3(0.0f, -10.0f, 0.0f), new K::Vector3(90.0f, 0.0f, 0.0f), new K::Vector3(1.0f, 1.0f, 1.0f));

			K::GameObject* cam = new K::GameObject("Editor Camera", camPosition);

			K::Camera* camera = new K::Camera();
			//camera->SetActiveState(true);
			camera->SetEditorState(true);
			cam->AddComponent(camera);
		#endif
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

			glfwSwapBuffers(window->window);

			K::SceneManager::Update();

			glfwPollEvents();
		}
		delete editor;
		delete manager;
	}
}