#include "K-Engine/Window.h"
#include "K-Engine/InputManager.h"

namespace K 
{
	Window::Window(const char* windowName)
	{
		if (!glfwInit())
			std::cout << "Failed to initialise GLFW" << std::endl;

		/* Create a windowed mode window and its OpenGL context */
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		#if _DEBUG
			glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		#endif

		this->width = mode->width;
		this->height = mode->height;
		this->refreshRate = mode->refreshRate;
		glViewport(0, 0, this->width, this->height);
		#if _DEBUG
			this->window = glfwCreateWindow(this->width, this->height, windowName, NULL, NULL);
		#else
			this->window = glfwCreateWindow(this->width, this->height, windowName, monitor, NULL);
		#endif

		std::cout << this->width << " " << this->height << std::endl;

		if (!this->window)
		{
			glfwTerminate();
			std::cout << "Failed to create window" << std::endl;
		}

		glfwMakeContextCurrent(this->window);

		glfwSwapInterval(1);

		if (glewInit() != GLEW_OK)
		{
			std::cout << "Error GLEW" << std::endl;
		}

		std::cout << glGetString(GL_VERSION) << std::endl;

		glPolygonMode(GL_FRONT, GL_FILL);

		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
		glDepthRange(0.0f, 1.0f);

		glfwSetKeyCallback(this->window, K::InputManager::key_callback);
		#if _DEBUG

		#else
			//glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		#endif
	}

	Window::~Window()
	{
		glfwDestroyWindow(this->window);
		glfwTerminate();
	}
}