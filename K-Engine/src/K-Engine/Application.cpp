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

		K::SceneManager manager = K::SceneManager();

		K::Editor* editor = new K::Editor(window, manager.currentScene, material);

		#if _DEBUG
			K::Transform* camPosition = new K::Transform(new Vector3(0.0f, -10.0f, 0.0f), new K::Vector3(90.0f, 0.0f, 0.0f), new K::Vector3(1.0f, 1.0f, 1.0f));

			K::GameObject* cam = new K::GameObject("Editor Camera", camPosition);

			K::Camera* camera = new K::Camera();
			camera->SetActiveState(true);
			camera->SetEditorState(true);
			cam->AddComponent(camera);
			manager.currentScene->Init();
		#else
			K::Deserializer deserialize = K::Deserializer(manager.currentScene, TEST_SCENE, editor);
		#endif

		std::cout << "Scene has been deserialized" << std::endl;

		while (!glfwWindowShouldClose(window->window))
		{
			int esc = glfwGetKey(window->window, GLFW_KEY_ESCAPE);

			if (esc == GLFW_PRESS)
			{
				break;
			}

			glClearColor(19.0f / 255.0f, 18.0f / 255.0f, 15.0f / 255.0f, 1.0f);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(material->GetShader()->shader);

			manager.currentScene->Render();
			#if _DEBUG
			if (editor->Render()) 
			{
				break;
			}
			#endif

			glfwSwapBuffers(window->window);

			glfwPollEvents();
		}
		delete editor;
	}
}