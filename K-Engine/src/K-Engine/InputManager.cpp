#include "InputManager.h"

namespace K 
{
	std::map<int, int> K::InputManager::keys;

	int InputManager::GetKey(int key) 
	{
		auto search = K::InputManager::keys.find(key);
		if (search != K::InputManager::keys.end()) 
		{
			int val = search->second;
			return val;
		}
		return GLFW_RELEASE;
	}

	bool InputManager::IsKeyPressed(int key)
	{
		if (K::InputManager::GetKey(key) == GLFW_PRESS || K::InputManager::GetKey(key) == GLFW_REPEAT)
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
		if (K::InputManager::GetKey(key) == GLFW_PRESS)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool InputManager::IsKeyReleased(int key)
	{
		if (K::InputManager::GetKey(key) == GLFW_RELEASE)
		{
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

	void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto search = K::InputManager::keys.find(key);
		if (search != K::InputManager::keys.end()) 
		{
			if (action == GLFW_RELEASE)
			{
				K::InputManager::keys.erase(search);
			}
			else 
			{
				search->second = action;
			}
		}
		else 
		{
			K::InputManager::keys.insert({ key, action });
		}
	}
}