#pragma once
#include "build.h"

namespace K
{
	class K_API Window
	{
	public:
		GLFWwindow* window;
		float width = 1920.0f;
		float height = 1080.0f;
		Window(const char* windowName);
		virtual ~Window();
	};
}