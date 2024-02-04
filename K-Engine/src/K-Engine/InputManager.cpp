#include "InputManager.h"

namespace K 
{
	std::unordered_map<int, int> InputManager::keys;

	void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto it = keys.find(key);
		switch (action) 
		{
		case GLFW_PRESS:
			keys.insert({ key, action });
			break;
		case GLFW_RELEASE:
			it->second = action;
			break;
		case GLFW_REPEAT:
			it->second = action;
			break;
		}
	}

	int InputManager::GetKey(int key) 
	{
		auto it = keys.find(key);
		if (it != keys.end()) 
		{
			return it->second;
		}
		else 
		{
			return GLFW_KEY_UNKNOWN;
		}
	}

	bool InputManager::IsKeyPressed(int key)
	{
		if (glfwGetKey(K::window->window, key) == GLFW_PRESS || glfwGetKey(K::window->window, key) == GLFW_REPEAT)
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	bool InputManager::IsKeyPressedDown(int key) 
	{
		if (InputManager::GetKey(key) == GLFW_PRESS)
		{
			InputManager::keys.find(key)->second = GLFW_REPEAT;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool InputManager::IsKeyReleased(int key)
	{
		if (InputManager::GetKey(key) == GLFW_RELEASE)
		{
			InputManager::keys.erase(key);
			return true;
		}
		else
		{
			return false;
		}
	}

	K::Vector3 InputManager::GetMousePosition()
	{
		double x, y;
		glfwGetCursorPos(window->window, &x, &y);
		K::Vector3 clipPosition = K::Vector3(2.0f * ((x / window->width) - 0.5f), -2.0f * ((y / window->height) - 0.5f), 0.0f);
		//MultiplyMatrixVector(screenPosition, worldPosition, K::Camera::translationMatrix);
		//MultiplyMatrixVector(translatedPosition, worldPosition, K::Camera::projectionMatrix);
		return clipPosition;
	}
}