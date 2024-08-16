#pragma once
#include "build.h"
#include "Component.h"
#include "SceneManager.h"
#include "Camera.h"
#include "InputManager.h"

namespace K 
{
	class K_API ButtonLoadScene : public K::Component
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