#include "InputManager.h"

namespace K 
{
	bool InputManager::IsKeyPressed(int key, GLFWwindow* window)
	{
		if (glfwGetKey(window, key) == GLFW_PRESS || glfwGetKey(window, key) == GLFW_REPEAT)
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	K::Vector3 InputManager::GetMousePosition(K::Window* window)
	{
		double x, y;
		glfwGetCursorPos(window->window, &x, &y);
		K::Vector3 clipPosition = K::Vector3(2.0f * ((x / window->width) - 0.5f), -2.0f * ((y / window->height) - 0.5f), 0.0f);
		//MultiplyMatrixVector(screenPosition, worldPosition, K::Camera::translationMatrix);
		//MultiplyMatrixVector(translatedPosition, worldPosition, K::Camera::projectionMatrix);
		return clipPosition;
	}
}