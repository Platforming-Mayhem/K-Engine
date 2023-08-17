#include "Camera.h"

namespace K 
{
	Camera::Camera(K::Shader* shader)
	{
		this->shader = shader;
	}

	Camera::~Camera() 
	{

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
		return this->shader;
	}

	void Camera::SetFOV(float newFOV) 
	{
		this->FOV = newFOV;
	}

	void Camera::SetWindowSize(K::Window window) 
	{
		this->screenWidth = window.width;
		this->screenHeight = window.height;
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

	K::Matrix4x4 K::Camera::projectionMatrix;
	K::Matrix4x4 K::Camera::viewMatrix;

	void Camera::Bind()
	{
		//Projection Matrix
		float deltaZ, aspect;
		float radians = ((3.14159f * 2.0f) / 360.0f) * this->FOV / 2.0f;
		float fFovRad = tanf(this->FOV * 0.5f / 180.0f * 3.14159f);
		deltaZ = this->farPlane - this->nearPlane;
		aspect = this->screenWidth / this->screenHeight;

		/*float t = this->orthoSize, b = -this->orthoSize, l = -this->orthoSize * aspect, r = this->orthoSize * aspect;
		K::Camera::projectionMatrix = K::Camera::projectionMatrix.IdentityMatrix();
		K::Camera::projectionMatrix.m[0][0] = 2.0f / (r - l);
		K::Camera::projectionMatrix.m[1][1] = 2.0f / (t - b);
		K::Camera::projectionMatrix.m[2][2] = -2.0f / (this->farPlane - this->nearPlane);
		K::Camera::projectionMatrix.m[3][0] = -(r+l)/ (r-l);
		K::Camera::projectionMatrix.m[3][1] = -(t + b) / (t - b);
		K::Camera::projectionMatrix.m[3][2] = -(this->farPlane + this->nearPlane) / (this->farPlane - this->nearPlane);*/
		K::Camera::projectionMatrix.m[0][0] = 1.0f / (aspect * fFovRad);
		K::Camera::projectionMatrix.m[1][1] = 1.0f / fFovRad;
		K::Camera::projectionMatrix.m[2][2] = -((this->farPlane + this->nearPlane) / deltaZ);
		K::Camera::projectionMatrix.m[3][2] = -((2.0f * this->farPlane * this->nearPlane) / deltaZ);
		K::Camera::projectionMatrix.m[2][3] = -1.0f;
		K::Camera::projectionMatrix.m[3][3] = 0.0f;

		glUniformMatrix4fv(glGetUniformLocation(this->shader->shader, "projectionMatrix"), 1, GL_FALSE, &K::Camera::projectionMatrix.m[0][0]);

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
		glUniformMatrix4fv(glGetUniformLocation(this->shader->shader, "viewMatrix"), 1, GL_FALSE, &K::Camera::viewMatrix.m[0][0]);
	}

	void Camera::Init() 
	{

	}

	void Camera::Unbind()
	{
		
	}

	void Camera::Update() 
	{
		if (canMove) 
		{
			/*if (InputManager::IsKeyPressed(GLFW_KEY_RIGHT, window->window))
			{
				*(this->parent->GetTransform()->rotation) += new K::Vector3(0.0f, 0.0f, -K::Time::deltaTime() * this->rotationSpeed);
			}
			else if (InputManager::IsKeyPressed(GLFW_KEY_LEFT, window->window))
			{
				*(this->parent->GetTransform()->rotation) += new K::Vector3(0.0f, 0.0f, K::Time::deltaTime() * this->rotationSpeed);
			}
			if (InputManager::IsKeyPressed(GLFW_KEY_UP, window->window) && this->parent->GetTransform()->rotation->x < 135.0f)
			{
				*(this->parent->GetTransform()->rotation) += new K::Vector3(K::Time::deltaTime() * this->rotationSpeed, 0.0f, 0.0f);
			}
			else if (InputManager::IsKeyPressed(GLFW_KEY_DOWN, window->window) && this->parent->GetTransform()->rotation->x > 45.0f)
			{
				*(this->parent->GetTransform()->rotation) += new K::Vector3(-K::Time::deltaTime() * this->rotationSpeed, 0.0f, 0.0f);
			}*/
			K::Quaternion* quat = K::Quaternion::Euler(new K::Vector3(90.0f, 0.0f, this->parent->GetTransform()->rotation->z));
			/*if (InputManager::IsKeyPressed(GLFW_KEY_D, window->window))
			{
				K::Vector3* right = new K::Vector3(K::Time::deltaTime() * this->movementSpeed, 0.0f, 0.0f);
				K::Vector3* rotatedRight = new K::Vector3(0.0f, 0.0f, 0.0f);
				MultiplyMatrixVector(*right, *rotatedRight, *quat->QuaternionToMatrix());
				*(this->parent->GetTransform()->position) += rotatedRight;
			}
			else if (InputManager::IsKeyPressed(GLFW_KEY_A, window->window))
			{
				K::Vector3* left = new K::Vector3(-K::Time::deltaTime() * this->movementSpeed, 0.0f, 0.0f);
				K::Vector3* rotatedLeft = new K::Vector3(0.0f, 0.0f, 0.0f);
				MultiplyMatrixVector(*left, *rotatedLeft, *quat->QuaternionToMatrix());
				*(this->parent->GetTransform()->position) += rotatedLeft;
			}
			if (InputManager::IsKeyPressed(GLFW_KEY_W, window->window))
			{
				K::Vector3* forward = new K::Vector3(0.0f, 0.0f, -K::Time::deltaTime() * this->movementSpeed);
				K::Vector3* rotatedForward = new K::Vector3(0.0f, 0.0f, 0.0f);
				MultiplyMatrixVector(*forward, *rotatedForward, *quat->QuaternionToMatrix());
				*(this->parent->GetTransform()->position) += rotatedForward;
			}
			else if (InputManager::IsKeyPressed(GLFW_KEY_S, window->window))
			{
				K::Vector3* back = new K::Vector3(0.0f, 0.0f, K::Time::deltaTime() * this->movementSpeed);
				K::Vector3* rotatedBack = new K::Vector3(0.0f, 0.0f, 0.0f);
				MultiplyMatrixVector(*back, *rotatedBack, *quat->QuaternionToMatrix());
				*(this->parent->GetTransform()->position) += rotatedBack;
			}*/
		}
	}

	void Camera::UpdateEditor()
	{
		if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow)) 
		{
			this->canMove = false;
		}
		else 
		{
			this->canMove = true;
		}
		if (ImGui::CollapsingHeader("Camera Settings")) 
		{
			ImGui::Text("These are the Camera settings");
			ImGui::DragFloat("Camera Near Plane", &this->nearPlane);
			ImGui::DragFloat("Camera Far Plane", &this->farPlane);
			ImGui::SliderFloat("Camera FOV", &this->FOV, 30.0f, 180.0f);
			ImGui::SliderFloat("Camera Size", &this->orthoSize, 1.0f, 100.0f);
			ImGui::DragFloat("Movement Speed", &this->movementSpeed);
			ImGui::DragFloat("Rotation Speed", &this->rotationSpeed);
		}
	}
}