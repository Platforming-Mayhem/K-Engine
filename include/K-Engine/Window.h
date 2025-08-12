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
		int offsetX = 0, offsetY = 0;
		Window(const char* windowName);
		virtual ~Window();

		static void window_size_callback(GLFWwindow* window, int width, int height);

		static void window_pos_callback(GLFWwindow* window, int xpos, int ypos);
	};

	K_API extern K::Window* window;
}