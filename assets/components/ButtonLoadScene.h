#pragma once
#include "K-Engine/Component.h"
#include "K-Engine/SceneManager.h"
#include "Camera.h"
#include "Mesh.h"
#include "K-Engine/InputManager.h"

namespace K 
{
	class ButtonLoadScene : public K::Component
	{
	private:
		K::Camera* mainCamera;
		K::Mesh* mesh;
		float selectedTint[3];
		bool selected = false;
		std::string properties;
	public:
		ButtonLoadScene();

		~ButtonLoadScene();

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void SetPropertyValues(const char* value, int valueIndex) override;

		const char* GetPropertyValues() override;
	};
}