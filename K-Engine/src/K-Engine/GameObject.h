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
		K::Material* material;
	public:

		GameObject(const char* name, K::Transform* transform, K::Material* material);

		~GameObject();

		void Update();

		void UpdateEditor();

		void PassTransformationMatrix();

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
			this->components.push_back(component);
			component->Init();
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

		void SetMaterial(K::Material* material) 
		{
			this->material = material;
		}

		K::Component* GetComponent(int index) 
		{
			return this->components[index];
		}

		K::Component* GetComponentOfType(const char* component) 
		{
			for (int i = 0; i < this->GetNumberOfComponents(); i++) 
			{
				if (this->GetComponent(i)->GetName() == component) 
				{
					return this->GetComponent(i);
				}
			}
			return nullptr;
		}

		K::Material* GetMaterial() 
		{
			return this->material;
		}

		int GetNumberOfComponents() 
		{
			return this->components.size();
		}

		void Bind();

		void Unbind();
	};
}