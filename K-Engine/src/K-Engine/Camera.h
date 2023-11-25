#pragma once
#include "build.h"
#include "Window.h"
#include "GameObject.h"
#include "Component.h"
#include "Shader.h"

namespace K
{
	class K_API Camera : public K::Component
	{
		bool isEditorCamera = false;
		bool isActive = false;
		K::Material* material = nullptr;
		K::Window* window = nullptr;
		float nearPlane = 0.9f;
		float farPlane = 1000.0f;
		float FOV = 60.0f;
		float orthoSize = 1.0f;
		enum class CameraType { Perspective = 0, Orthographic = 1 };
		CameraType cameraType = CameraType::Perspective;
		K::Matrix4x4 projectionMatrix;
		K::Matrix4x4 viewMatrix;
		std::string properties;

		float movementSpeed = 10.0f;
		float rotationSpeed = 100.0f;
	public:

		K::Vector3 GetMousePosition(K::Window* window);

		Camera();

		virtual ~Camera();

		float GetFOV();

		float GetNearPlane();

		float GetFarPlane();

		K::Shader* GetShader();

		void SetActiveState(bool state);

		void SetEditorState(bool state);

		void SetMaterial(K::Material* material);

		void SetFOV(float newFOV);

		void SetWindow(K::Window* window);

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