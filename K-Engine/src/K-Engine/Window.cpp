#include "Window.h"

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
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
		this->width = mode->width;
		this->height = mode->height;
		//Set fullscreen: window = glfwCreateWindow(screenWidth, screenHeight, "Rasterizer", monitor, NULL);
		//glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
		glViewport(0, 0, this->width, this->height);
		this->window = glfwCreateWindow(this->width, this->height, windowName, NULL, NULL);

		if (!this->window)
		{
			glfwTerminate();
			std::cout << "Failed to create window" << std::endl;
		}
		glfwMakeContextCurrent(this->window);
		glfwSwapInterval(0);

		if (glewInit() != GLEW_OK)
		{
			std::cout << "Error GLEW" << std::endl;
		}

		glPolygonMode(GL_FRONT, GL_FILL);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
		glDepthRange(0.0f, 1.0f);
	}

	Window::~Window()
	{

	}
}