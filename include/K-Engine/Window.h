#pragma once
#include "build.h"

namespace K
{
	class K_API Window
	{
	public:
		GLFWwindow* window;
		ALCcontext* audioContext;
		ALCdevice* audioDevice;
		float width = 1920.0f;
		float height = 1080.0f;
		int refreshRate = 60;
		Window(const char* windowName);
		virtual ~Window();
	};
}