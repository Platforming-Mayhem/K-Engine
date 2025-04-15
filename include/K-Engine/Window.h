#pragma once
#include "build.h"
#include "K-Engine/Audio.h"

namespace K
{
	class K_API Window
	{
	public:
		GLFWwindow* window;
		ma_engine miniAudioEngine;
		K::Audio* startUpSFX;
		float width = 1920.0f;
		float height = 1080.0f;
		int refreshRate = 60;
		Window(const char* windowName);
		virtual ~Window();
	};

	K_API extern K::Window* window;
}