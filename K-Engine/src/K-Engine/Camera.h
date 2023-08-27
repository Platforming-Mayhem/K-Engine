#pragma once
#include "build.h"
#include "Window.h"
#include "GameObject.h"
#include "Component.h"
#include "Shader.h"

namespace K
{
	extern K::Window* window;

	class K_API Camera : public K::Component
	{
		K::Shader* shader;
		float screenWidth = 1920.0f;
		float screenHeight = 1080.0f;
		float nearPlane = 0.9f;
		float farPlane = 1000.0f;
		float FOV = 60.0f;
		float orthoSize = 1.0f;
	public:
		static K::Matrix4x4 projectionMatrix;
		static K::Matrix4x4 viewMatrix;

		float movementSpeed = 10.0f;
		float rotationSpeed = 100.0f;
		bool canMove = true;

		K::Vector3 GetMousePosition(K::Window* window);

		Camera(K::Shader* shader);

		virtual ~Camera();

		float GetFOV();

		float GetNearPlane();

		float GetFarPlane();

		K::Shader* GetShader();

		void SetFOV(float newFOV);

		void SetWindowSize(K::Window window);

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void Bind() override;

		void Unbind()  override;

		const char* GetName() override;
	};
}