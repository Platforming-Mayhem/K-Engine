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
		float offsetX = std::roundf(ImGui::GetItemRectMin().x);
		float offsetY = std::roundf(ImGui::GetItemRectMin().y);
		float sizeX = ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x;
		float sizeY = ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y;
		K::Vector3 position = K::Vector3(std::clamp(((ImGui::GetMousePos().x - offsetX) / sizeX) * window->width, 0.0f, window->width), std::clamp(((ImGui::GetMousePos().y - offsetY) / sizeY) * window->height, 0.0f, window->height), 0.0f);
		return position;
	}

	K::Vector3 InputManager::ConvertToClipPosition(K::Vector3 position) 
	{
		K::Vector3 clipPosition = K::Vector3(2.0f * ((position.x / (window->width)) - 0.5f), -2.0f * ((position.y / (window->height)) - 0.5f), 0.0f);
		return clipPosition;
	}

	K::GameObject* InputManager::PickGameObject(K::Camera* camera) 
	{
		K::Vector3 mousePosition = K::InputManager::GetMousePosition();
		K::Vector3 clipPosition = K::InputManager::ConvertToClipPosition(mousePosition);
		//std::cout << clipPosition.x << "," << clipPosition.y << std::endl;
		K::Vector3 clipPositionNearWithZ = K::Vector3(clipPosition.x, clipPosition.y, 0.0f);
		K::Vector3 worldSpaceNearPosition;

		K::Vector3 clipPositionFarWithZ = K::Vector3(clipPosition.x, clipPosition.y, 0.0f);
		K::Vector3 worldSpaceFarPosition;

		K::Transform nearPlane = K::Transform(new K::Vector3(0.0f, 0.0f, camera->GetNearPlane()), new K::Vector3(), new K::Vector3(1.0f, 1.0f, 1.0f));
		nearPlane.PassModelMatrix(camera->parent->GetTransform());
		K::Matrix4x4 modelMatrix = nearPlane.modelMatrix;
		K::Matrix4x4 viewMatrix = camera->GetViewMatrix();
		K::Matrix4x4 projectionMatrix = camera->GetProjectionMatrix();

		K::Matrix4x4 VP = K::Matrix4x4::Matrix_MultiplyMatrix(viewMatrix, projectionMatrix);
		K::Matrix4x4 MVP = K::Matrix4x4::Matrix_MultiplyMatrix(modelMatrix, VP);
		K::Matrix4x4 invPV = K::QuickInverse(MVP);

		K::MultiplyMatrixVector(clipPositionNearWithZ, worldSpaceNearPosition, invPV);
		//K::MultiplyMatrixVector(clipPositionFarWithZ, worldSpaceFarPosition, invPV);
		//worldSpacePosition is the mouse Position in worldspace

		return nullptr;
	}
}