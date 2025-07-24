#include "K-Engine/InputManager.h"

namespace K 
{
	std::unordered_map<int, int> InputManager::keys;
	K::RenderTexture* renderTex;
	K::Material* editorMat;

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

	bool InputManager::IsMouseKeyPressed(int key)
	{
		if (glfwGetMouseButton(K::window->window, key) == GLFW_PRESS)
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
		glfwGetCursorPos(K::window->window, &x, &y);
		K::Vector3 position = K::Vector3(std::clamp(((float)x - K::Editor::offsetX) / K::Editor::windowScaleFactor, 0.0f, K::window->width), std::clamp(((float)y - K::Editor::offsetY) / K::Editor::windowScaleFactor, 0.0f, K::window->height), 0.0f);
		return position;
	}

	K::Vector3 InputManager::ConvertToClipPosition(K::Vector3 position) 
	{
		K::Vector3 clipPosition = K::Vector3(2.0f * ((position.x / (K::window->width)) - 0.5f), -2.0f * ((position.y / (K::window->height)) - 0.5f), 0.0f);
		return clipPosition;
	}

	K::Vector3 InputManager::GetWorldMouseDirection()
	{
		//variables
		K::Vector3 mousePosition, rawClipPosition, clipPositionStart, worldSpaceNearPosition, worldSpaceDirection;
		K::Matrix4x4 viewMatrix, projectionMatrix, viewProjectionMatrix, invVPMatrix;

		mousePosition = K::InputManager::GetMousePosition();
		rawClipPosition = K::InputManager::ConvertToClipPosition(mousePosition);
		clipPositionStart = K::Vector3(rawClipPosition.x, rawClipPosition.y, -1.0f);

		viewMatrix = *K::Editor::viewMatrix;
		projectionMatrix = *K::Editor::projectionMatrix;

		viewProjectionMatrix = K::Matrix4x4::Matrix_MultiplyMatrix(viewMatrix, projectionMatrix);
		invVPMatrix = K::QuickInverse(viewProjectionMatrix);

		K::MultiplyMatrixVector(clipPositionStart, worldSpaceNearPosition, invVPMatrix);

		K::Vector3 camPosition = *K::Editor::cameraPosition;

		worldSpaceDirection = (worldSpaceNearPosition - camPosition).normalise();
		return worldSpaceDirection;
	}

	K::GameObject* InputManager::PickGameObject() 
	{
		K::GameObject* temp = nullptr;
		K::Vector3 mousePosition = K::InputManager::GetMousePosition();
		K::renderTex->Bind();
		glClearColor(-1, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		K::SceneManager::currentScene->RenderLoopNewMaterial(K::editorMat);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		unsigned int pixel;
		glReadPixels(mousePosition.x, K::window->height - mousePosition.y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel);
		glReadBuffer(GL_NONE);
		temp = K::SceneManager::currentScene->GetGameObjects()[pixel];
		K::renderTex->Unbind();
		return temp;
	}
}
