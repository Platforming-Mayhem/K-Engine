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
		K::Material* material;
		float screenWidth = 1920.0f;
		float screenHeight = 1080.0f;
		float nearPlane = 0.9f;
		float farPlane = 1000.0f;
		float FOV = 60.0f;
		float orthoSize = 1.0f;
		enum CameraType { Perspective, Orthographic };
		CameraType cameraType = CameraType::Perspective;
		K::Matrix4x4 projectionMatrix;
		K::Matrix4x4 viewMatrix;
	public:
		float movementSpeed = 10.0f;
		float rotationSpeed = 100.0f;

		K::Vector3 GetMousePosition(K::Window* window);

		Camera(K::Material* material);

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

		void SetPropertyValues(const char* value) override;

		const char* GetName() override;

		const char* GetPropertyValues() override;
	};
}