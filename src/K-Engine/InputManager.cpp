#include "K-Engine/InputManager.h"

namespace K 
{
	std::unordered_map<int, int> InputManager::keys;
	K::Window* K::window;
	K::RenderTexture* K::renderTex;
	K::Material* K::editorMat;

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
		//variables
		/*K::Vector3 mousePosition, rawClipPosition, clipPositionStart, worldSpaceNearPosition, worldSpaceDirection;
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

		float distance = FLT_MAX;

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
		}*/

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