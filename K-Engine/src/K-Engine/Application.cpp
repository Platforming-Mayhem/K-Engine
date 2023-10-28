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

		K::Scene* newScene = new Scene();

		K::Transform* camPosition = new K::Transform(new Vector3(0.0f, -10.0f, 0.0f), new K::Vector3(90.0f, 0.0f, 0.0f), new K::Vector3(1.0f, 1.0f, 1.0f));

		K::GameObject* cam = new K::GameObject("Camera", camPosition);

		K::Camera* camera = new K::Camera(material->GetShader());

		#if _DEBUG
			K::Editor* editor = new K::Editor(window, newScene, material->GetShader());
		#endif

		camera->SetWindowSize(*window);
		cam->AddComponent(camera);

		newScene->Attach(cam);

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

			K::Time::startTime = glfwGetTime();

			newScene->Render(material);

			#if _DEBUG
				editor->Render();
			#endif

			K::Time::endTime = K::Time::startTime;

			glFlush();

			glfwSwapBuffers(window->window);

			glfwPollEvents();
		}
		glfwDestroyWindow(window->window);
		glfwTerminate();
	}
}