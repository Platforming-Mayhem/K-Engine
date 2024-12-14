#pragma once
#include "Component.h"
#include "Camera.h"
#include "../InputManager.h"
#include "../SceneManager.h"

namespace K 
{
	class K_API Quit : public K::Component
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