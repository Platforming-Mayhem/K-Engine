#pragma once
#include "build.h"
#include "Transform.h"
#include "Window.h"

namespace K 
{
	extern K::Window* window;
	class K_API InputManager 
	{
	private:
		static std::unordered_map<int, int> keys;
	public:
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		static int GetKey(int key);

		static bool IsKeyPressed(int key);

		static bool IsKeyPressedDown(int key);

		static bool IsKeyReleased(int key);
		//GET THIS WORKING ASAP!!!
		static K::Vector3 GetMousePosition();
	};
}