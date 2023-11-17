#pragma once
#include "build.h"
#include "Transform.h"
#include "Window.h"

namespace K 
{
	extern K::Window* window;
	class K_API InputManager 
	{
	public:
		static bool IsKeyPressed(int key);
		//GET THIS WORKING ASAP!!!
		static K::Vector3 GetMousePosition();
	};
}