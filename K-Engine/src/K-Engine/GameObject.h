#pragma once
#include "build.h"
#include "Material.h"
#include "Mesh.h"
#include "Component.h"
#include "Window.h"

namespace K 
{
	class K_API GameObject
	{
	private:
		std::string name;
		K::Transform transform = K::Transform(new Vector3(0.0f, 0.0f, 0.0f), new K::Vector3(0.0f, 0.0f, 0.0f), new K::Vector3(1.0f, 1.0f, 1.0f));
		std::vector<K::Component*> components;
	public:

		GameObject(const char* name, Transform* transform);

		virtual ~GameObject();

		void Update();

		void UpdateEditor();

		void PassTransformationMatrix(K::Material* mat);

		const char* GetName() 
		{
			return this->name.c_str();
		}

		void SetName(std::string name) 
		{
			this->name = name;
		}

		K::Transform* GetTransform() 
		{
			return &this->transform;
		}

		void AddComponent(K::Component* component)
		{
			component->SetParent(this);
			component->Init();
			this->components.push_back(component);
		}

		void SetPosition(K::Vector3 position)
		{
			GetTransform()->position = &position;
		}

		void SetRotation(K::Vector3 rotation)
		{
			GetTransform()->rotation = &rotation;
		}

		void SetScale(K::Vector3 scale)
		{
			GetTransform()->scale = &scale;
		}

		K::Component* GetComponent(int index) 
		{
			return this->components[index];
		}

		int GetNumberOfComponents() 
		{
			return this->components.size();
		}

		void Bind();

		void Unbind();
	};
}