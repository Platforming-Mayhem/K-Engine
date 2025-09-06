#include "K-Engine/Window.h"
#include "K-Engine/InputManager.h"

namespace K 
{
	K::Window* window;

	Window::Window(const char* windowName)
	{
		K::window = this;
		ma_result result = ma_engine_init(NULL, &this->miniAudioEngine);

		if (result != MA_SUCCESS)
		{
			std::cout << "Failed to initialize audio engine" << std::endl;
		}

		if (glfwInit())
			std::cout << "Initialised GLFW" << std::endl;
		else
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
		#if _DEBUG
		this->window = glfwCreateWindow(this->width, this->height, windowName, NULL, NULL);
		#else
		this->window = glfwCreateWindow(this->width, this->height, windowName, monitor, NULL);
		#endif

		glViewport(0, 0, this->width, this->height);

		std::cout << this->width << " " << this->height << std::endl;

		if (!this->window)
		{
			std::cout << "Failed to create window" << std::endl;
			glfwTerminate();
		}
		else 
		{
			std::cout << "Window created successfully" << std::endl;
		}

		glfwMakeContextCurrent(this->window);

		GLenum err = glewInit();
		std::cout << glewGetErrorString(err) << std::endl;


		std::cout << glGetString(GL_VERSION) << std::endl;

		glPolygonMode(GL_FRONT, GL_FILL);

		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
		glDepthRange(0.0f, 1.0f);

		glfwSetKeyCallback(window, K::InputManager::key_callback);
		glfwSetCursorPosCallback(window, K::InputManager::cursor_position_callback);
		glfwSetWindowSizeCallback(window, K::Window::window_size_callback);
		glfwSetWindowPosCallback(window, K::Window::window_pos_callback);
		//glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		#if _DEBUG
		this->startUpSFX = new K::Audio("../../assets/audios/StartUp.wav");
		this->startUpSFX->Play();
		glfwSwapInterval(1);
		#else
		glfwSwapInterval(1);
		#endif
	}

	void Window::window_size_callback(GLFWwindow* window, int width, int height)
	{
		std::cout << width << "," << height << std::endl;
	}

	void Window::window_pos_callback(GLFWwindow* window, int xpos, int ypos)
	{
		K::window->offsetX = xpos;
		K::window->offsetY = ypos;
	}

	Window::~Window()
	{
		#if _DEBUG
		delete this->startUpSFX;
		#endif
		glfwDestroyWindow(this->window);
		glfwTerminate();
		ma_engine_uninit(&this->miniAudioEngine);
	}
}
