#pragma once
#include "build.h"
#include "Transform.h"
#include "Window.h"

namespace K 
{
	class K_API InputManager 
	{
	public:
		static bool IsKeyPressed(int key, GLFWwindow* window);
		//GET THIS WORKING ASAP!!!
		static K::Vector3 GetMousePosition(K::Window* window);
	};
}