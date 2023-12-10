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

		K::Scene* newScene = new Scene("Test");

		K::Editor* editor = new K::Editor(window, newScene, material);

		#if _DEBUG
			K::Transform* camPosition = new K::Transform(new Vector3(0.0f, -10.0f, 0.0f), new K::Vector3(90.0f, 0.0f, 0.0f), new K::Vector3(1.0f, 1.0f, 1.0f));

			K::GameObject* cam = new K::GameObject("Editor Camera", camPosition, material);

			K::Camera* camera = new K::Camera();
			camera->SetActiveState(true);
			camera->SetEditorState(true);
			cam->AddComponent(camera);

			newScene->Attach(cam);
		#endif

		K::Deserializer deserialize = K::Deserializer(newScene, TEST_SCENE, editor);

		std::cout << "Scene has been deserialized" << std::endl;

		while (!glfwWindowShouldClose(window->window))
		{
			int esc = glfwGetKey(window->window, GLFW_KEY_ESCAPE);

			if (esc == GLFW_PRESS)
			{
				break;
			}

			glClearColor(29.0f / 255.0f, 143.0f / 255.0f, 224.0f / 255.0f, 1.0f);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glUseProgram(material->GetShader()->shader);

			newScene->Render();
			#if _DEBUG
			if (editor->Render()) 
			{
				break;
			}
			#endif

			glFlush();

			glfwSwapBuffers(window->window);

			glfwPollEvents();
		}
		glfwDestroyWindow(window->window);
		glfwTerminate();
	}
}