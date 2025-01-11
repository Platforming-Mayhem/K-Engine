#pragma once
#include "K-Engine/Component.h"
#include "Camera.h"
#include "Mesh.h"
#include "K-Engine/InputManager.h"
#include "K-Engine/SceneManager.h"

namespace K 
{
	KC_API class Quit : public K::Component
	{
	private:
		K::Camera* mainCamera;
		K::Mesh* mesh;
		bool selected = false;
		std::string properties;
	public:
		Quit();

		~Quit();

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void SetPropertyValues(const char* value, int valueIndex) override;

		const char* GetPropertyValues() override;
	};
}