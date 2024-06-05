#include "Camera.h"
#include "InputManager.h"
#include "Editor.h"

namespace K 
{
	Camera::Camera()
	{
		
	}

	Camera::~Camera() 
	{
		//std::cout << "Camera Destructor..." << std::endl;
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

	void Camera::CameraMatrix() 
	{
		if (this->isActive && !this->isEditorCamera)
		{
			//Projection Matrix
			float aspect = this->window->width / this->window->height;

			if (this->cameraType == CameraType::Perspective)
			{
				float radians = ((3.14159f * 2.0f) / 360.0f) * this->FOV / 2.0f;
				float fFovRad = tanf(this->FOV * 0.5f / 180.0f * 3.14159f);
				float deltaZ = this->farPlane - this->nearPlane;
				this->projectionMatrix = K::Matrix4x4::IdentityMatrix();
				this->projectionMatrix.m[0][0] = 1.0f / (aspect * fFovRad);
				this->projectionMatrix.m[1][1] = 1.0f / fFovRad;
				this->projectionMatrix.m[2][2] = -((this->farPlane + this->nearPlane) / deltaZ);
				this->projectionMatrix.m[3][2] = -((2.0f * this->farPlane * this->nearPlane) / deltaZ);
				this->projectionMatrix.m[2][3] = -1.0f;
				this->projectionMatrix.m[3][3] = 0.0f;
			}
			else if (this->cameraType == CameraType::Orthographic)
			{
				float t = this->orthoSize, b = -this->orthoSize, l = -this->orthoSize * aspect, r = this->orthoSize * aspect;
				this->projectionMatrix = K::Matrix4x4::IdentityMatrix();
				this->projectionMatrix.m[0][0] = 2.0f / (r - l);
				this->projectionMatrix.m[1][1] = 2.0f / (t - b);
				this->projectionMatrix.m[2][2] = -2.0f / (this->farPlane - this->nearPlane);
				this->projectionMatrix.m[3][0] = -(r + l) / (r - l);
				this->projectionMatrix.m[3][1] = -(t + b) / (t - b);
				this->projectionMatrix.m[3][2] = -(this->farPlane + this->nearPlane) / (this->farPlane - this->nearPlane);
			}
			//View Matrix
			this->viewMatrix = K::QuickInverse(this->parent->GetTransform()->modelMatrix);
			if (this->parent->parent != nullptr)
			{
				K::Matrix4x4 scalingMatrix = this->parent->GetTransform()->LocalScaleMatrix(this->parent->parent->GetTransform());
				K::Matrix4x4 invertedScalingMatrix = K::QuickInverse(scalingMatrix);
				this->viewMatrix = K::Matrix4x4::Matrix_MultiplyMatrix(this->viewMatrix, invertedScalingMatrix);
			}
			else
			{
				K::Matrix4x4 scalingMatrix = this->parent->GetTransform()->ScaleMatrix();
				K::Matrix4x4 invertedScalingMatrix = K::QuickInverse(scalingMatrix);
				this->viewMatrix = K::Matrix4x4::Matrix_MultiplyMatrix(this->viewMatrix, invertedScalingMatrix);
			}

			glViewport(0, 0, this->window->width, this->window->height);

			glUniformMatrix4fv(glGetUniformLocation(this->material->GetShader()->shader, "viewMatrix"), 1, GL_FALSE, &this->viewMatrix.m[0][0]);

			glUniformMatrix4fv(glGetUniformLocation(this->material->GetShader()->shader, "projectionMatrix"), 1, GL_FALSE, &this->projectionMatrix.m[0][0]);
		}
	}

	void Camera::Bind()
	{
		
	}

	void Camera::RenderBind() 
	{
		glClearColor(this->backgroundColour[0], this->backgroundColour[1], this->backgroundColour[2], this->backgroundColour[3]);
		glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "fogColour"), this->backgroundColour[0], this->backgroundColour[1], this->backgroundColour[2]);
		this->CameraMatrix();
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
		
	}

	void Camera::RenderInit() 
	{
		SetWindow(K::window);
		SetMaterial(this->parent->GetMaterial());
	}

	void Camera::Unbind()
	{
		
	}

	void Camera::Render()
	{
		
	}

	void Camera::Update() 
	{
		
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