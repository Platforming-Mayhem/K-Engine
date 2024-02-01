#include "Camera.h"
#include "InputManager.h"
#include "Time.h"
#include "Editor.h"

namespace K 
{
	Camera::Camera()
	{
		
	}

	Camera::~Camera() 
	{
		this->isActive = false;
		this->player = nullptr;
		std::cout << "Camera Destructor..." << std::endl;
	}

	float Camera::GetFOV() 
	{
		return this->FOV;
	}

	float Camera::GetNearPlane()
	{
		return this->nearPlane;
	}

	float Camera::GetFarPlane()
	{
		return this->farPlane;
	}

	const char* Camera::GetName() 
	{
		return typeid(K::Camera).name();
	}

	K::Shader* Camera::GetShader() 
	{
		return this->material->GetShader();
	}

	void Camera::SetMaterial(K::Material* material) 
	{
		this->material = material;
	}

	void Camera::SetFOV(float newFOV) 
	{
		this->FOV = newFOV;
	}

	void Camera::SetWindow(K::Window* window)
	{
		this->window = window;
	}
	
	void Camera::SetActiveState(bool state) 
	{
		this->isActive = state;
	}

	void Camera::SetEditorState(bool state) 
	{
		this->isEditorCamera = state;
	}

	K::Vector3 Camera::GetMousePosition(K::Window* window)
	{
		double x, y;
		glfwGetCursorPos(window->window, &x, &y);
		K::Vector3 clipPosition = K::Vector3(2.0f * ((x / window->width) - 0.5f), -2.0f * ((y / window->height) - 0.5f), 0.0f);
		K::Matrix4x4 pv = K::Matrix4x4::Matrix_MultiplyMatrix(this->projectionMatrix, this->viewMatrix);
		K::Matrix4x4 mvp = K::Matrix4x4::Matrix_MultiplyMatrix(pv, this->parent->GetTransform()->modelMatrix);
		mvp = K::QuickInverse(mvp);
		K::Vector3 worldPosition = K::Vector3(0.0f, 0.0f, 0.0f);
		//K::MultiplyMatrixVector(clipPosition, worldPosition, mvp);
		return worldPosition;
	}

	void Camera::Bind()
	{
		if (this->isActive) 
		{
			glClearColor(this->backgroundColour[0], this->backgroundColour[1], this->backgroundColour[2], this->backgroundColour[3]);
			//Projection Matrix
			float deltaZ, aspect;
			float radians = ((3.14159f * 2.0f) / 360.0f) * this->FOV / 2.0f;
			float fFovRad = tanf(this->FOV * 0.5f / 180.0f * 3.14159f);
			deltaZ = this->farPlane - this->nearPlane;
			aspect = this->window->width / this->window->height;

			if (this->cameraType == CameraType::Perspective)
			{
				K::Camera::projectionMatrix.m[0][0] = 1.0f / (aspect * fFovRad);
				K::Camera::projectionMatrix.m[1][1] = 1.0f / fFovRad;
				K::Camera::projectionMatrix.m[2][2] = -((this->farPlane + this->nearPlane) / deltaZ);
				K::Camera::projectionMatrix.m[3][2] = -((2.0f * this->farPlane * this->nearPlane) / deltaZ);
				K::Camera::projectionMatrix.m[2][3] = -1.0f;
				K::Camera::projectionMatrix.m[3][3] = 0.0f;
			}
			else if (this->cameraType == CameraType::Orthographic)
			{
				float t = this->orthoSize, b = -this->orthoSize, l = -this->orthoSize * aspect, r = this->orthoSize * aspect;
				K::Camera::projectionMatrix = K::Camera::projectionMatrix.IdentityMatrix();
				K::Camera::projectionMatrix.m[0][0] = 2.0f / (r - l);
				K::Camera::projectionMatrix.m[1][1] = 2.0f / (t - b);
				K::Camera::projectionMatrix.m[2][2] = -2.0f / (this->farPlane - this->nearPlane);
				K::Camera::projectionMatrix.m[3][0] = -(r + l) / (r - l);
				K::Camera::projectionMatrix.m[3][1] = -(t + b) / (t - b);
				K::Camera::projectionMatrix.m[3][2] = -(this->farPlane + this->nearPlane) / (this->farPlane - this->nearPlane);
			}

			glUniformMatrix4fv(glGetUniformLocation(this->material->GetShader()->shader, "projectionMatrix"), 1, GL_FALSE, &this->projectionMatrix.m[0][0]);

			//View Matrix
			K::Vector3 forward = K::Vector3(0.0f, 0.0f, 1.0f);
			K::Vector3 up = K::Vector3(0.0f, 1.0f, 0.0f);

			K::Vector3 rotatedForward = K::Vector3(0.0f, 0.0f, 0.0f);
			K::Vector3 rotatedUp = K::Vector3(0.0f, 0.0f, 0.0f);

			K::Quaternion* quat = K::Quaternion::Euler(new K::Vector3(this->parent->GetTransform()->rotation->x, this->parent->GetTransform()->rotation->y, this->parent->GetTransform()->rotation->z));

			MultiplyMatrixVector(forward, rotatedForward, *quat->QuaternionToMatrix());
			MultiplyMatrixVector(up, rotatedUp, *quat->QuaternionToMatrix());

			K::Vector3 pos = *this->parent->GetTransform()->position;
			K::Vector3 target = pos + rotatedForward;
			K::Matrix4x4* lookAtMatrix = K::LookAt(pos, target, rotatedUp);
			K::Camera::viewMatrix = K::QuickInverse(*lookAtMatrix);
			glUniformMatrix4fv(glGetUniformLocation(this->material->GetShader()->shader, "viewMatrix"), 1, GL_FALSE, &this->viewMatrix.m[0][0]);
		}
	}

	const char* Camera::GetPropertyValues()
	{
		this->properties = std::to_string(this->nearPlane) + ",";
		this->properties += std::to_string(this->farPlane) + ",";
		this->properties += std::to_string(this->FOV) + ",";
		this->properties += std::to_string(this->orthoSize) + ",";
		this->properties += std::to_string(this->movementSpeed) + ",";
		this->properties += std::to_string(this->rotationSpeed) + ",";
		std::string temp;
		if (this->isActive) 
		{
			temp = "true";
		}
		else 
		{
			temp = "false";
		}
		this->properties += temp + ",";
		temp = std::to_string((int)this->cameraType);
		this->properties += temp + ",";
		this->properties += std::to_string(this->backgroundColour[0]) + ",";
		this->properties += std::to_string(this->backgroundColour[1]) + ",";
		this->properties += std::to_string(this->backgroundColour[2]) + ",";
		this->properties += std::to_string(this->backgroundColour[3]);
		return this->properties.c_str();
	}

	void Camera::SetPropertyValues(const char* value, int valueIndex)
	{
		if (value[0] != '\0' && value != nullptr) 
		{
			std::string temp = value;
			switch (valueIndex)
			{
			case 0:
				this->nearPlane = std::stof(temp);
				break;
			case 1:
				this->farPlane = std::stof(temp);
				break;
			case 2:
				this->FOV = std::stof(temp);
				break;
			case 3:
				this->orthoSize = std::stof(temp);
				break;
			case 4:
				this->movementSpeed = std::stof(temp);
				break;
			case 5:
				this->rotationSpeed = std::stof(temp);
				break;
			case 6:
				if (temp == "true")
				{
					this->SetActiveState(true);
				}
				else if (temp == "false")
				{
					this->SetActiveState(false);
				}
				break;
			case 7:
				if (temp == std::to_string((int)CameraType::Perspective)) 
				{
					this->cameraType = CameraType::Perspective;
				}
				else if (temp == std::to_string((int)CameraType::Orthographic))
				{
					this->cameraType = CameraType::Orthographic;
				}
				break;
			case 8:
				this->backgroundColour[0] = std::stof(temp);
				break;
			case 9:
				this->backgroundColour[1] = std::stof(temp);
				break;
			case 10:
				this->backgroundColour[2] = std::stof(temp);
				break;
			case 11:
				this->backgroundColour[3] = std::stof(temp);
				break;
			}
		}
	}

	void Camera::Init() 
	{
		SetWindow(K::window);
		SetMaterial(this->parent->GetMaterial());
		if (!this->isEditorCamera) 
		{
			for (int i = 0; i < K::Editor::GetCurrentScene()->GetNumberOfObjects(); i++) 
			{
				K::GameObject* temp = K::Editor::GetCurrentScene()->GetGameObjects()[i];
				if (temp->GetComponentOfType(typeid(K::Player).name()) != nullptr) 
				{
					player = temp->GetTransform();
					break;
				}
			}
		}
	}

	void Camera::Unbind()
	{
		
	}

	void Camera::Update() 
	{
		//Camera Move WASD
		#if _DEBUG
		if (this->isActive && this->isEditorCamera) 
		{
			K::Quaternion* quat = K::Quaternion::Euler(new K::Vector3(90.0f, 0.0f, this->parent->GetTransform()->rotation->z));
			if (InputManager::IsKeyPressed(GLFW_KEY_D))
			{
				K::Vector3* right = new K::Vector3(K::Time::deltaTime() * this->movementSpeed, 0.0f, 0.0f);
				K::Vector3* rotatedRight = new K::Vector3(0.0f, 0.0f, 0.0f);
				MultiplyMatrixVector(*right, *rotatedRight, *quat->QuaternionToMatrix());
				*(this->parent->GetTransform()->position) += rotatedRight;
			}
			else if (InputManager::IsKeyPressed(GLFW_KEY_A))
			{
				K::Vector3* left = new K::Vector3(-K::Time::deltaTime() * this->movementSpeed, 0.0f, 0.0f);
				K::Vector3* rotatedLeft = new K::Vector3(0.0f, 0.0f, 0.0f);
				MultiplyMatrixVector(*left, *rotatedLeft, *quat->QuaternionToMatrix());
				*(this->parent->GetTransform()->position) += rotatedLeft;
			}
			if (InputManager::IsKeyPressed(GLFW_KEY_W))
			{
				K::Vector3* forward = new K::Vector3(0.0f, 0.0f, -K::Time::deltaTime() * this->movementSpeed);
				K::Vector3* rotatedForward = new K::Vector3(0.0f, 0.0f, 0.0f);
				MultiplyMatrixVector(*forward, *rotatedForward, *quat->QuaternionToMatrix());
				*(this->parent->GetTransform()->position) += rotatedForward;
			}
			else if (InputManager::IsKeyPressed(GLFW_KEY_S))
			{
				K::Vector3* back = new K::Vector3(0.0f, 0.0f, K::Time::deltaTime() * this->movementSpeed);
				K::Vector3* rotatedBack = new K::Vector3(0.0f, 0.0f, 0.0f);
				MultiplyMatrixVector(*back, *rotatedBack, *quat->QuaternionToMatrix());
				*(this->parent->GetTransform()->position) += rotatedBack;
			}
		}
		#endif
		if (!this->isEditorCamera && this->player != nullptr)
		{
			K::Vector3 offset = *player->position - K::Vector3(0.0f, 1.0f, 0.0f);
			K::Vector3 position = K::Vector3::Lerp(*this->parent->GetTransform()->position, offset, K::Time::deltaTime() * this->movementSpeed);
			*this->parent->GetTransform()->position = position;
		}
	}

	void Camera::UpdateEditor()
	{
		//IMGUI
		if (ImGui::CollapsingHeader("Camera Settings")) 
		{
			ImGui::Text("These are the Camera settings");
			if (this->cameraType == CameraType::Perspective)
			{
				if (ImGui::BeginCombo("Camera Type", "Perspective"))
				{
					if (ImGui::Selectable("Perspective"))
					{
						this->cameraType = CameraType::Perspective;
					}
					else if (ImGui::Selectable("Orthographic"))
					{
						this->cameraType = CameraType::Orthographic;
					}
					ImGui::EndCombo();
				}
				ImGui::DragFloat("Camera Near Plane", &this->nearPlane);
				ImGui::DragFloat("Camera Far Plane", &this->farPlane);
				ImGui::SliderFloat("Camera FOV", &this->FOV, 30.0f, 180.0f);
			}
			else if (this->cameraType == CameraType::Orthographic)
			{
				if (ImGui::BeginCombo("Camera Type", "Orthographic"))
				{
					if (ImGui::Selectable("Perspective"))
					{
						this->cameraType = CameraType::Perspective;
					}
					else if (ImGui::Selectable("Orthographic"))
					{
						this->cameraType = CameraType::Orthographic;
					}
					ImGui::EndCombo();
				}
				ImGui::SliderFloat("Camera Size", &this->orthoSize, 1.0f, 100.0f);
			}
			ImGui::DragFloat("Movement Speed", &this->movementSpeed);
			ImGui::DragFloat("Rotation Speed", &this->rotationSpeed);
			ImGui::Checkbox("is Active", &this->isActive);
			ImGui::ColorPicker4("Background Colour", this->backgroundColour);
		}
	}
}