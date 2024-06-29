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
		//variables
		K::Vector3 mousePosition, rawClipPosition, clipPositionStart, worldSpaceNearPosition, worldSpaceDirection;
		K::Matrix4x4 viewMatrix, projectionMatrix, viewProjectionMatrix, invVPMatrix;

		mousePosition = K::InputManager::GetMousePosition();
		rawClipPosition = K::InputManager::ConvertToClipPosition(mousePosition);
		clipPositionStart = K::Vector3(rawClipPosition.x, rawClipPosition.y, -1.0f);

		viewMatrix = camera->GetViewMatrix();
		projectionMatrix = camera->GetProjectionMatrix();

		viewProjectionMatrix = K::Matrix4x4::Matrix_MultiplyMatrix(viewMatrix, projectionMatrix);
		invVPMatrix = K::QuickInverse(viewProjectionMatrix);

		K::MultiplyMatrixVector(clipPositionStart, worldSpaceNearPosition, invVPMatrix);

		K::Vector3 camPosition = camera->GetPosition();

		worldSpaceDirection = (worldSpaceNearPosition - camPosition).normalise();

		float distance = FLT_MAX;
		K::GameObject* temp = nullptr;

		for (auto gameObject : K::SceneManager::currentScene->GetGameObjects()) 
		{
			if (gameObject.second->GetComponentOfType(typeid(K::Mesh).name()) != nullptr) 
			{
				K::Mesh* mesh = (K::Mesh*)gameObject.second->GetComponentOfType(typeid(K::Mesh).name());
				for (int j = 0; j < mesh->indices.size() / 3; j++) 
				{
					K::Vector3 vertices[3];
					K::Vector3 normal;
					int index = 0;
					for (int i = 0 + (j*3); i < 3 + (j * 3); i++)
					{
						K::MultiplyMatrixVector(mesh->vertices[mesh->indices[i]].position, vertices[index], mesh->parent->GetTransform()->modelMatrix);
						K::Quaternion* quat = K::Quaternion::Euler(mesh->parent->GetTransform()->rotation);
						K::Matrix4x4 rotationMatrix = quat->QuaternionToMatrix();
						K::MultiplyMatrixVector(mesh->vertices[mesh->indices[i]].normal, normal, rotationMatrix);
						index++;
					}
					//Triangle vertices[3]
					//Test Against Ray
					K::Vector3 A = vertices[1] - vertices[0];
					K::Vector3 B = vertices[2] - vertices[1];
					K::Vector3 C = vertices[0] - vertices[2];

					float D = -K::Vector3::DotProduct(normal, vertices[0]);
					float t = -(K::Vector3::DotProduct(normal, camPosition) + D) / K::Vector3::DotProduct(normal, worldSpaceDirection);

					if (t > 0.0f) 
					{
						K::Vector3 P = (camPosition + (worldSpaceDirection * t));
						K::Vector3 C0 = P - vertices[0];
						K::Vector3 C1 = P - vertices[1];
						K::Vector3 C2 = P - vertices[2];

						K::Vector3 N0 = K::Vector3::CrossProduct(A, C0);
						K::Vector3 N1 = K::Vector3::CrossProduct(B, C1);
						K::Vector3 N2 = K::Vector3::CrossProduct(C, C2);
						float dot0 = K::Vector3::DotProduct(normal, N0);
						float dot1 = K::Vector3::DotProduct(normal, N1);
						float dot2 = K::Vector3::DotProduct(normal, N2);
						if (dot0 > 0.0f && dot1 > 0.0f && dot2 > 0.0f) 
						{
							if (t < distance) 
							{
								temp = mesh->parent;
								distance = t;
							}
						}
					}
				}
			}
		}

		return temp;
	}
}